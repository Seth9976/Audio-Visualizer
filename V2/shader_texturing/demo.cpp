#define GLFW_INCLUDE_ES2 1
#define GLFW_DLL 1
//#define GLFW_EXPOSE_NATIVE_WIN32 1
//#define GLFW_EXPOSE_NATIVE_EGL 1

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include <GLFW/glfw3.h>
//#include <GLFW/glfw3native.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream> 
#include "bitmap.h"
#include <vector>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <time.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define SPECTRUM_SIZE 256
#define TEXTURE_COUNT 2
//#define MAX_UNIFORM_VARIABLES 15
const int VISUALIZER_SIZE = 256;

GLint GprogramID = -1;
GLuint GtextureID[TEXTURE_COUNT];
GLFWwindow* window;

FMOD::System* m_fmodSystem;
FMOD::Sound* m_music;
FMOD::Channel* m_musicChannel;

float m_spectrumLeft[SPECTRUM_SIZE];
float m_spectrumRight[SPECTRUM_SIZE];
float spectrumAverage;

void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}
}
void initFmod()
{
	FMOD_RESULT result;
	unsigned int version;

	result = FMOD::System_Create(&m_fmodSystem);
	ERRCHECK(result);

	result = m_fmodSystem->getVersion(&version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
		printf("FMOD Error! You are using an old version of FMOD.", version, FMOD_VERSION);

	//initialize fmod system
	result = m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);

	//load and set up music
	result = m_fmodSystem->createStream("../media/Ritt Momney - Put Your Records On.mp3", FMOD_SOFTWARE, 0, &m_music);
	//Ritt Momney - Put Your Records On.mp3
	//The Weeknd - Blinding Lights.mp3
	//Jackson Wang & Galantis - Pretty Please.mp3
	//Hwasa - Maria.mp3
	//PNAU - Solid Gold feat. Kira Divine & Marques Toliver.mp3
	ERRCHECK(result);

	//play the loaded mp3 music
	result = m_fmodSystem->playSound(FMOD_CHANNEL_FREE, m_music, false, &m_musicChannel);
	ERRCHECK(result);

	//set sound channel loop count
	m_musicChannel->setLoopCount(10);
	m_musicChannel->setVolume(0.2f);
}
void updateFmod()
{
	m_fmodSystem->update();

	//Get spectrum far left and right stereo channels
	m_musicChannel->getSpectrum(m_spectrumLeft, SPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_RECT);
	m_musicChannel->getSpectrum(m_spectrumRight, SPECTRUM_SIZE, 1, FMOD_DSP_FFT_WINDOW_RECT);
	std::cout << m_spectrumLeft[0] << ", " << m_spectrumRight[0] << std::endl;
}
static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}
GLuint LoadShader(GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;
   
   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   	return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );
   
   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled ) 
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
		 char infoLog[4096];
         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         printf ( "Error compiling shader:\n%s\n", infoLog );            
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;
}
GLuint LoadShaderFromFile(GLenum shaderType, std::string path)
{
    GLuint shaderID = 0;
    std::string shaderString;
    std::ifstream sourceFile( path.c_str() );

    if( sourceFile )
    {
        shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );
        const GLchar* shaderSource = shaderString.c_str();

		return LoadShader(shaderType, shaderSource);
    }
    else
        printf( "Unable to open file %s\n", path.c_str() );

    return shaderID;
}
void loadTexture(const char* path, GLuint textureID)
{
	CBitmap bitmap(path);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

	// bilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());
}
int Init ( void )
{
   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   //load textures
   glGenTextures(TEXTURE_COUNT, GtextureID);
   loadTexture("../media/Flower.bmp", GtextureID[0]); // Flower // GoldFlower
   loadTexture("../media/background.bmp", GtextureID[1]);
   //====

   fragmentShader = LoadShaderFromFile(GL_VERTEX_SHADER, "../vertexShader1.vert" );
   vertexShader = LoadShaderFromFile(GL_FRAGMENT_SHADER, "../fragmentShader1.frag" );

   // Create the program object
   programObject = glCreateProgram ( );
   
   if ( programObject == 0 )
      return 0;

   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   glBindAttribLocation ( programObject, 0, "vPosition" );
   glBindAttribLocation ( programObject, 1, "vColor" );
   glBindAttribLocation ( programObject, 2, "vTexCoord" );

   // Link the program
   glLinkProgram ( programObject );

   // Check the link status
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked ) 
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
		 char infoLog[1024];
         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         printf ( "Error linking program:\n%s\n", infoLog );            
      }

      glDeleteProgram ( programObject );
      return 0;
   }

   // Store the program object
   GprogramID = programObject;

   glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


   initFmod();
   return 1;
}
//GLint uniformVariables[MAX_UNIFORM_VARIABLES];
//float factors[MAX_UNIFORM_VARIABLES];

static float factor1 = 0.0f;
static float factor2 = 200.0f;
static float factor3 = 200.0f;
static float factor4 = 0.0f;
static float factor5 = 0.0f;
static float factor6 = 0.0f;
static float factor7 = 0.0f;
static float factor8 = 0.0f;
static float factor9 = 0.0f;
static float factor10 = 0.0f;
static float factor11 = 0.0f;
static float factor12 = 0.0f;
static float factor13 = 0.0f;
static float factor14 = 0.0f;
static float limit = 400.0;
static float limit2 = 725.0;

bool f6;
bool f7;
bool f8;
bool f9;
bool f10;

void Draw(void)
{
	updateFmod();
	spectrumAverage = ((m_spectrumLeft[0] + m_spectrumRight[0] / 2) * 2);
#pragma region MyRegion
	// set the sampler2D varying variable to the first texture unit(index 0)
	glUniform1i(glGetUniformLocation(GprogramID, "sampler2d"), 0);
	// Declare Factors
	
#pragma endregion
	// Factor Values
	factor1 = spectrumAverage;
	factor11 = ((m_spectrumLeft[1] + m_spectrumRight[1] / 2) * 3); // SOLID GOLD VALUE 4 // EVERYTHING ELSE 1
	factor12 = ((m_spectrumLeft[3] + m_spectrumRight[3] / 2) * 3); // SOLID GOLD VALUE 4 // EVERYTHING ELSE 3
	factor13 = ((m_spectrumLeft[5] + m_spectrumRight[5] / 2) * 1);
	factor2 -= 0.78; // BLINDING LIGHTS VALUE 0.293 // PRETTY PLEASE VALUE 0.86 // MARIA VALUE 0.43 // SOLID GOLD VALUE 0.445 // PUT YOUR RECORDS ON VALUE 0.78
	factor3 -= 0.072; // BLINDING LIGHTS VALUE 0.2 // PRETTY PLEASE VALUE 0.1 // MARIA VALUE 0.225 // SOLID GOLD VALUE 0.175 // PUT YOUR RECORDS ON VALUE 0.072
	factor14 += 0.1;
	if (factor2 < 0.0)
	{
		factor4 += 0.15; // BLINDING LIGHTS VALUE 0.5 // PRETTY PLEASE VALUE 0.175 // MARIA VALUE 0.5 // SOLID GOLD VALUE 0.4 // PUT YOUR RECORDS ON VALUE 0.15
	}
	if (factor3 < 0.0)
	{
		if (factor5 < limit)
			factor5 += 1.0f;
		if (factor6 > limit)
			f6 = true;
		if (factor6 < 0)
			f6 = false;
		if (f6)
			factor6 -= 1.5 + (spectrumAverage * 5.0); // 5.0
		else
			factor6 += 0.1 + (spectrumAverage * 2.5); // 2.5
		if (factor7 > limit2)
			f7 = true;
		if (factor7 < 0)
			f7 = false;
		if (f7)
			factor7 -= 1.5 + (spectrumAverage * 5.0);
		else
			factor7 += 0.1 + (spectrumAverage * 2.5);
		if (factor8 > limit2)
			f8 = true;
		if (factor8 < 0)
			f8 = false;
		if (f8)
			factor8 -= 1.5 + (factor12 * 5.0);
		else
			factor8 += 0.15 + (factor12 * 2.5);
		if (factor9 > limit2)
			f9 = true;
		if (factor9 < -10.0)
			f9 = false;
		if (f9)
			factor9 -= 1.5 + (factor12 * 12.0);
		else
			factor9 += 0.25 + (factor12 * 10.0);
		if (factor10 > limit2)
			f10 = true;
		if (factor10 < 0)
			f10 = false;
		if (f10)
			factor10 -= 1.5 + (factor11 * 10.0);
		else
			factor10 += 0.15 + (factor11 * 5.0);
	}
	

	// Uniform
	GLint factor1Loc = glGetUniformLocation(GprogramID, "Factor1");
	if(factor1Loc != -1)
	   glUniform1f(factor1Loc, factor1);
	GLint factor2Loc = glGetUniformLocation(GprogramID, "Factor2");
	if (factor2Loc != -1)
		glUniform1f(factor2Loc, factor2);
	GLint factor3Loc = glGetUniformLocation(GprogramID, "Factor3");
	if (factor3Loc != -1)
		glUniform1f(factor3Loc, factor3);
	GLint factor4Loc = glGetUniformLocation(GprogramID, "Factor4");
	if (factor4Loc != -1)
		glUniform1f(factor4Loc, factor4);
	GLint factor5Loc = glGetUniformLocation(GprogramID, "Factor5");
	if (factor5Loc != -1)
		glUniform1f(factor5Loc, factor5);
	GLint factor6Loc = glGetUniformLocation(GprogramID, "Factor6");
	if (factor6Loc != -1)
		glUniform1f(factor6Loc, factor6);
	GLint factor7Loc = glGetUniformLocation(GprogramID, "Factor7");
	if (factor7Loc != -1)
		glUniform1f(factor7Loc, factor7);
	GLint factor8Loc = glGetUniformLocation(GprogramID, "Factor8");
	if (factor8Loc != -1)
		glUniform1f(factor8Loc, factor8);
	GLint factor9Loc = glGetUniformLocation(GprogramID, "Factor9");
	if (factor9Loc != -1)
		glUniform1f(factor9Loc, factor9);
	GLint factor10Loc = glGetUniformLocation(GprogramID, "Factor10");
	if (factor10Loc != -1)
		glUniform1f(factor10Loc, factor10);
	GLint factor11Loc = glGetUniformLocation(GprogramID, "Factor11");
	if (factor11Loc != -1)
		glUniform1f(factor11Loc, factor11);
	GLint factor12Loc = glGetUniformLocation(GprogramID, "Factor12");
	if (factor12Loc != -1)
		glUniform1f(factor12Loc, factor12);
	GLint factor13Loc = glGetUniformLocation(GprogramID, "Factor13");
	if (factor13Loc != -1)
		glUniform1f(factor13Loc, factor13);
	GLint factor14Loc = glGetUniformLocation(GprogramID, "Factor14");
	if (factor14Loc != -1)
		glUniform1f(factor14Loc, factor14);

	//===================


   /*static GLfloat vVertices[] = {-0.5f, 0.5f, 0.0f,
								  -0.5f, -0.5f, 0.0f,
								  0.5f, -0.5f,  0.0f,
								  0.5f, -0.5f, 0.0f,
								  0.5f, 0.5f, 0.0f,
								  -0.5f, 0.5f,  0.0f};*/
   static GLfloat vVertices[] = { -1.0f,  1.0f, 0.0f,
								  -1.0f, -1.0f, 0.0f,
								  1.0f, -1.0f,  0.0f,
								  1.0f,  -1.0f, 0.0f,
								  1.0f, 1.0f, 0.0f,
								  -1.0f, 1.0f,  0.0f };
	/*
   static GLfloat vColors[] = {1.0f,  0.0f, 0.0f, 1.0f,
								0.0f, 1.0f, 0.0f, 1.0f,
								0.0f, 0.0f,  1.0f, 1.0f,
								0.0f,  0.0f, 1.0f, 1.0f,
								1.0f, 1.0f, 0.0f, 1.0f,
								1.0f, 0.0f,  0.0f, 1.0f};*/
   static GLfloat vColors[] = { 1.0f,  1.0f, 1.0f, 1.0f,
							   1.0f,  1.0f, 1.0f, 1.0f,
							   1.0f,  1.0f, 1.0f, 1.0f,
							   1.0f,  1.0f, 1.0f, 1.0f,
							   1.0f,  1.0f, 1.0f, 1.0f,
							   1.0f,  1.0f, 1.0f, 1.0f };

   static GLfloat vTexCoords[] = {0.0f,  1.0f,
									0.0f, 0.0f,
									1.0f, 0.0f,
									1.0f,  0.0f,
									1.0f, 1.0f,
									0.0f, 1.0f};

   glBindTexture(GL_TEXTURE_2D, GtextureID[0]);
   // Set the viewport
   glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
   // Clear the color buffer
   glClear(GL_COLOR_BUFFER_BIT);
   // Use the program object
   glUseProgram(GprogramID);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vColors);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, vTexCoords);
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);
   glDrawArrays(GL_TRIANGLES, 0, 6);
   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);
   glDisableVertexAttribArray(2);

}

int main(void)
{
  glfwSetErrorCallback(error_callback);

  // Initialize GLFW library
  if (!glfwInit())
    return -1;

  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create and open a window
  window = glfwCreateWindow(WINDOW_WIDTH,
                            WINDOW_HEIGHT,
                            "MERCURY",
                            NULL,
                            NULL);

  if (!window)
  {
    glfwTerminate();
    printf("glfwCreateWindow Error\n");
    exit(1);
  }

  glfwMakeContextCurrent(window);

  Init();

  // Repeat
  while (!glfwWindowShouldClose(window)) {


	Draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
