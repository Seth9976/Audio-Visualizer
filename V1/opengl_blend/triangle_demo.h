#ifndef _TRIANGLE_DEMO_H
#define _TRIANGLE_DEMO_H

#include "demo_base.h"
#include "bitmap.h"
#include <vector>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <time.h>

#define TEXTURE_COUNT 4

#define SPECTRUM_SIZE 256

#define RECT_VERTICE_W 50
#define RECT_VERTICE_H 50

const int RECT_VERTEX_COUNT = (RECT_VERTICE_W - 1) * (RECT_VERTICE_H - 1) * 6;
const int RECT_VERTEX_ARRAY_SIZE = RECT_VERTEX_COUNT * 3;
const int RECT_UV_ARRAY_SIZE = RECT_VERTEX_COUNT * 2;
const int VISUALIZER_SIZE = 256;

void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}
}

class TriangleDemo : public DemoBase
{

private:
	float mWaveRadianOffset;
	GLfloat mRectVertices[RECT_VERTEX_ARRAY_SIZE];
	GLfloat mRectUV[RECT_UV_ARRAY_SIZE];

	GLuint mTextureID[TEXTURE_COUNT];
	FMOD::System* m_fmodSystem;
	FMOD::Sound* m_music;
	FMOD::Channel* m_musicChannel;

	float m_spectrumLeft[SPECTRUM_SIZE];
	float m_spectrumRight[SPECTRUM_SIZE];
	float spectrumAverage;

	void loadTexture(const char* path, GLuint textureID)
	{
		CBitmap bitmap(path);

		// Create Linear Filtered Texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // apply texture wrapping along horizontal part.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // apply texture wrapping along vertical part.
		/*
		// old school (minecraft) filtering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // near filtering. (For when texture needs to scale up on screen)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // far filtering. (For when texture need to scale down on screen)
		*/
		
		// bilinear filtering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // near filtering. (For when texture needs to scale up on screen)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // far filtering. (For when texture need to scale down on screen)
		

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());
	}

public:
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
		result = m_fmodSystem->createStream("../media/Remi Wolf - Woo!.mp3", FMOD_SOFTWARE, 0, &m_music);
		//The Grateful 7 - Get My Head on Straight.mp3
		//Naruto Shippuden Full Opening 16.mp3
		//Giant Steps.mp3
		//Life Is Strange OST - End Credits [Max & Chloe].mp3
		//Life Is Strange Soundtrack - Golden hour By Johnathan Morali.mp3
		//Love On Top.mp3
		//Remi Wolf - Woo!.mp3
		//Tkay Maidza - You Sad.mp3
		//Superorganism - Something For Your M.I.N.D.mp3
		//Pokémon!- GS Red Lance Battle Theme.mp3
		ERRCHECK(result);

		//play the loaded mp3 music
		result = m_fmodSystem->playSound(FMOD_CHANNEL_FREE, m_music, false, &m_musicChannel);
		ERRCHECK(result);

		//set sound channel loop count
		m_musicChannel->setLoopCount(10);
		m_musicChannel->setVolume(0.2f);
	}
	void init()
	{
		glGenTextures(TEXTURE_COUNT, mTextureID);
		loadTexture("../media/Glow.bmp", mTextureID[1]);
		loadTexture("../media/Diamond.bmp", mTextureID[2]);
		loadTexture("../media/water.bmp", mTextureID[3]);
		loadTexture("../media/Glow.bmp", mTextureID[4]);
		initFmod();
	}

	void deinit()
	{
	}

	void drawWorldOriginLines(const Matrix& viewMatrix)
	{
		//draw axis,
		glLoadMatrixf((GLfloat*)viewMatrix.mVal);
		glBegin(GL_LINES);
			glColor3f(1.0f, 0.3f, 0.3f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);

			glColor3f(0.3f, 1.0f, 0.3f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);

			glColor3f(0.3f, 0.3f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 1.0f);
		glEnd();
	}

	void updateFmod()
	{
		m_fmodSystem->update();

		//Get spectrum far left and right stereo channels
		m_musicChannel->getSpectrum(m_spectrumLeft, SPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_RECT);
		m_musicChannel->getSpectrum(m_spectrumRight, SPECTRUM_SIZE, 1, FMOD_DSP_FFT_WINDOW_RECT);
		std::cout << m_spectrumLeft[0] << ", " << m_spectrumRight[0] << std::endl;
	}
	class Vertex
	{
	public:
		float x;
		float y;
		float z;

		Vertex()
		{
			x = 0;
			y = 0;
			z = 0;
		}

		Vertex(float a, float b, float c)
		{
			x = a;
			y = b;
			z = c;
		}
	};
	//color
	float rHue(float r, float g, float b, float U, float W)
	{
		float red;
		red = (.299 + .701 * U + .168 * W) * r
			+ (.587 - .587 * U + .330 * W) * g
			+ (.114 - .114 * U - .497 * W) * b;
		return red;
	}
	float gHue(float r, float g, float b, float U, float W)
	{
		float green;
		green = (.299 - .299 * U - .328 * W) * r
			+ (.587 + .413 * U + .035 * W) * g
			+ (.114 - .114 * U + .292 * W) * b;
		return green;
	}
	float bHue(float r, float g, float b, float U, float W)
	{
		float blue;
		blue = (.299 - .3 * U + 1.25 * W) * r
			+ (.587 - .588 * U - 1.05 * W) * g
			+ (.114 + .886 * U - .203 * W) * b;
		return blue;
	}
	//
	void drawCylinder(float radius, float height, int triangleAmount)
	{
		GLfloat twicePi = 2.0f * M_PI;
		glBegin(GL_TRIANGLES);
		for (int i = 0; i <= triangleAmount; i++)
		{
			float index = i / (float)triangleAmount;
			glColor3f(index, index, index); // for color

			glVertex3f(0.0f, height, 0.0f);
			glVertex3f(radius * cos(i * twicePi / triangleAmount), height, radius * sin(i * twicePi / triangleAmount));
			glVertex3f(radius * cos((i + 1) * twicePi / triangleAmount), height, radius * sin((i + 1) * twicePi / triangleAmount));

			glVertex3f(0.0f, -height, 0.0f);
			glVertex3f(radius * cos(i * twicePi / triangleAmount), -height, radius * sin(i * twicePi / triangleAmount));
			glVertex3f(radius * cos((i + 1) * twicePi / triangleAmount), -height, radius * sin((i + 1) * twicePi / triangleAmount));

			glVertex3f(radius * cos(i * twicePi / triangleAmount), -height, radius * sin(i * twicePi / triangleAmount));
			glVertex3f(radius * cos((i + 1) * twicePi / triangleAmount), -height, radius * sin((i + 1) * twicePi / triangleAmount));
			glVertex3f(radius * cos((i + 1) * twicePi / triangleAmount), height, radius * sin((i + 1) * twicePi / triangleAmount));

			glVertex3f(radius * cos(i * twicePi / triangleAmount), height, radius * sin(i * twicePi / triangleAmount));
			glVertex3f(radius * cos((i + 1) * twicePi / triangleAmount), height, radius * sin((i + 1) * twicePi / triangleAmount));
			glVertex3f(radius * cos(i * twicePi / triangleAmount), -height, radius * sin(i * twicePi / triangleAmount));
		}
		glEnd();
	}
	void genPlane(float offsetRadian)
	{
		const float width = 5.0f;
		const float height = 5.0f;
		const float halfWidth = width * 0.5f;
		const float halfHeight = height * 0.5f;

		const float waveAmpMul = 0.1f;
		const float xToRadianMul = 5.0f;

		const float texMul = 10.0f;

		int currentVert = -1;
		int currentIndex = -1;
		for (int h = 0; h < RECT_VERTICE_H - 1; h++)
		{
			for (int w = 0; w < RECT_VERTICE_W - 1; w++)
			{
				//========= 6 vertices to form one sub-rectangle

				//1st vertex
				int vertex1 = ++currentVert;
				mRectVertices[vertex1 * 3] = -halfWidth + (float)(w) / (float)RECT_VERTICE_W * width;
				mRectVertices[vertex1 * 3 + 1] = waveAmpMul * cos(offsetRadian + mRectVertices[vertex1 * 3] * xToRadianMul);
				mRectVertices[vertex1 * 3 + 2] = -halfHeight + (float)(h) / (float)RECT_VERTICE_H * height;
				mRectUV[vertex1 * 2] = (float)(w) / (float)RECT_VERTICE_W * texMul;
				mRectUV[vertex1 * 2 + 1] = (float)(h) / (float)RECT_VERTICE_H * texMul;

				//2nd vertex
				int vertex2 = ++currentVert;
				mRectVertices[vertex2 * 3] = -halfWidth + (float)(w) / (float)RECT_VERTICE_W * width;
				mRectVertices[vertex2 * 3 + 1] = waveAmpMul * cos(offsetRadian + mRectVertices[vertex2 * 3] * xToRadianMul);
				mRectVertices[vertex2 * 3 + 2] = -halfHeight + (float)(h + 1) / (float)RECT_VERTICE_H * height;
				mRectUV[vertex2 * 2] = (float)(w) / (float)RECT_VERTICE_W * texMul;
				mRectUV[vertex2 * 2 + 1] = (float)(h + 1) / (float)RECT_VERTICE_H * texMul;


				//3rd vertex
				int vertex3 = ++currentVert;
				mRectVertices[vertex3 * 3] = -halfWidth + (float)(w + 1) / (float)RECT_VERTICE_W * width;
				mRectVertices[vertex3 * 3 + 1] = waveAmpMul * cos(offsetRadian + mRectVertices[vertex3 * 3] * xToRadianMul);
				mRectVertices[vertex3 * 3 + 2] = -halfHeight + (float)(h + 1) / (float)RECT_VERTICE_H * height;
				mRectUV[vertex3 * 2] = (float)(w + 1) / (float)RECT_VERTICE_W * texMul;
				mRectUV[vertex3 * 2 + 1] = (float)(h + 1) / (float)RECT_VERTICE_H * texMul;


				//4th vertex
				int vertex4 = ++currentVert;
				mRectVertices[vertex4 * 3] = mRectVertices[vertex3 * 3];
				mRectVertices[vertex4 * 3 + 1] = mRectVertices[vertex3 * 3 + 1];
				mRectVertices[vertex4 * 3 + 2] = mRectVertices[vertex3 * 3 + 2];
				mRectUV[vertex4 * 2] = mRectUV[vertex3 * 2];
				mRectUV[vertex4 * 2 + 1] = mRectUV[vertex3 * 2 + 1];


				//5th vertex
				int vertex5 = ++currentVert;
				mRectVertices[vertex5 * 3] = -halfWidth + (float)(w + 1) / (float)RECT_VERTICE_W * width;
				mRectVertices[vertex5 * 3 + 1] = waveAmpMul * cos(offsetRadian + mRectVertices[vertex5 * 3] * xToRadianMul);
				mRectVertices[vertex5 * 3 + 2] = -halfHeight + (float)(h) / (float)RECT_VERTICE_H * height;
				mRectUV[vertex5 * 2] = (float)(w + 1) / (float)RECT_VERTICE_W * texMul;
				mRectUV[vertex5 * 2 + 1] = (float)(h) / (float)RECT_VERTICE_H * texMul;

				//6th vertex
				int vertex6 = ++currentVert;
				mRectVertices[vertex6 * 3] = mRectVertices[vertex1 * 3];
				mRectVertices[vertex6 * 3 + 1] = mRectVertices[vertex1 * 3 + 1];
				mRectVertices[vertex6 * 3 + 2] = mRectVertices[vertex1 * 3 + 2];
				mRectUV[vertex6 * 2] = mRectUV[vertex1 * 2];
				mRectUV[vertex6 * 2 + 1] = mRectUV[vertex1 * 2 + 1];
			}
		}
	}
	void drawWave()
	{
		mWaveRadianOffset += 0.01f;
		if (mWaveRadianOffset > 2.0f * M_PI)
			mWaveRadianOffset -= 2.0f * M_PI;
		genPlane(mWaveRadianOffset);

		glColor3f(1.0f, 1.0f, 1.0f);
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, mTextureID[2]);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, mRectVertices);
		glTexCoordPointer(2, GL_FLOAT, 0, mRectUV);

		glDrawArrays(GL_TRIANGLES, 0, RECT_VERTEX_COUNT);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	void drawLine(const Vector& pos1, const Vector& pos2)
	{
		glBegin(GL_LINES);
		glVertex3f(pos1.mVal[0], pos1.mVal[1], pos1.mVal[2]);
		glVertex3f(pos2.mVal[0], pos2.mVal[1], pos2.mVal[2]);
		glEnd();
	}
	// Pokemon
	float scalar, rotation, pokemonRotation;
	void drawSphere(float width, float height, float radius, float minDelta, float maxDelta, float minTheta, float maxTheta)
	{
		// vertices
		const float PI = acos(-1);
		struct Vertex
		{
			float x, y, z, s, t;
		};
		std::vector<Vertex> tmpVertices;

		float widthStep = 2 * PI / width;
		float heightStep = PI / height;
		float widthAngle, heightAngle;

		for (int i = 0; i <= height; ++i)
		{
			heightAngle = PI / 2 - i * heightStep;
			float xy = radius * cosf(heightAngle);
			float z = radius * sinf(heightAngle);

			for (int j = 0; j <= width; ++j)
			{
				widthAngle = j * widthStep;

				Vertex vertex;
				vertex.x = xy * cosf(widthAngle);
				vertex.y = xy * sinf(widthAngle);
				vertex.z = z;
				vertex.s = (float)j / width;
				vertex.t = (float)i / height;
				tmpVertices.push_back(vertex);
			}
		}

		// draw triangles
		Vertex v1, v2, v3, v4;
		std::vector<float> n;
		int i, j, k, vi1, vi2;
		int index = 0;
		for (i = 0; i < height; ++i)
		{
			vi1 = i * (width + 1);
			vi2 = (i + 1) * (width + 1);

			for (j = 0; j < width; ++j, ++vi1, ++vi2)
			{
				v1 = tmpVertices[vi1];
				v2 = tmpVertices[vi2];
				v3 = tmpVertices[vi1 + 1];
				v4 = tmpVertices[vi2 + 1];
				if (i >= minDelta && i <= maxDelta && j >= minTheta && j <= maxTheta)
				{
					if (i == 0)
					{
						glBegin(GL_TRIANGLES);
						glVertex3f(v1.x, v1.y, v1.z);
						glVertex3f(v2.x, v2.y, v2.z);
						glVertex3f(v4.x, v4.y, v4.z);
						glEnd();
						index += 3;
					}
					else if (i == (height - 1))
					{
						glBegin(GL_TRIANGLES);
						glVertex3f(v1.x, v1.y, v1.z);
						glVertex3f(v2.x, v2.y, v2.z);
						glVertex3f(v3.x, v3.y, v3.z);
						glEnd();
						index += 3;
					}
					else
					{
						glBegin(GL_TRIANGLES);
						glVertex3f(v1.x, v1.y, v1.z);
						glVertex3f(v2.x, v2.y, v2.z);
						glVertex3f(v3.x, v3.y, v3.z);
						glVertex3f(v2.x, v2.y, v2.z);
						glVertex3f(v3.x, v3.y, v3.z);
						glVertex3f(v4.x, v4.y, v4.z);
						glEnd();

						index += 4;
					}
				}
			}
		}
	}
	void drawHead()
	{
		// the head
		glColor3f(0.933, 0.870, 0.431); // yellow body color
		drawSphere(20, 20, 10, 0, 20, 0, 20);

		// head stripe
		glColor3f(0.153, 0.376, 0.372); // blue stripe color
		drawSphere(20, 20, 10, 6, 13, 3, 15);

		// eyes
		glColor3f(0, 0, 0); // black eye color
		drawSphere(20, 20, 10, 17, 17, 6, 15);
		drawSphere(20, 20, 10, 3, 3, 6, 15);
	}
	void drawSnout()
	{
		float width1 = 1.7;
		float width2 = 2.4;
		float width3 = 3;
		glColor3f(0.933, 0.870, 0.431); // yellow body color
		glBegin(GL_TRIANGLES);
		// left
		glVertex3f(-1, -0.61, -5.42);
		glVertex3f(-width1, 0.67, -3.42);
		glVertex3f(-1, -1.57, -4.86);
		glVertex3f(-1, -1.57, -4.86);
		glVertex3f(-width1, 0.67, -3.42);
		glVertex3f(-width1, -1.15, -2.5);

		glVertex3f(-width1, 0.67, -3.42);
		glVertex3f(-width1, -1.15, -2.5);
		glVertex3f(-width2, 1.79, -0.86);
		glVertex3f(-width2, 1.79, -0.86);
		glVertex3f(-width2, -1.05, 0.28);
		glVertex3f(-width1, -1.15, -2.5);

		glVertex3f(-width2, -1.05, 0.28);
		glVertex3f(-width2, 1.79, -0.86);
		glVertex3f(-width3, 2.75, 2.18);
		glVertex3f(-width3, 2.75, 2.18);
		glVertex3f(-width2, -1.05, 0.28);
		glVertex3f(-width3, -1.01, 3.26);
		// right
		glVertex3f(1, -0.61, -5.42);
		glVertex3f(width1, 0.67, -3.42);
		glVertex3f(1, -1.57, -4.86);
		glVertex3f(1, -1.57, -4.86);
		glVertex3f(width1, 0.67, -3.42);
		glVertex3f(width1, -1.15, -2.5);

		glVertex3f(width1, 0.67, -3.42);
		glVertex3f(width1, -1.15, -2.5);
		glVertex3f(width2, 1.79, -0.86);
		glVertex3f(width2, 1.79, -0.86);
		glVertex3f(width2, -1.05, 0.28);
		glVertex3f(width1, -1.15, -2.5);

		glVertex3f(width2, -1.05, 0.28);
		glVertex3f(width2, 1.79, -0.86);
		glVertex3f(width3, 2.75, 2.18);
		glVertex3f(width3, 2.75, 2.18);
		glVertex3f(width2, -1.05, 0.28);
		glVertex3f(width3, -1.01, 3.26);

		//bottom
		glVertex3f(-1, -1.57, -4.86);
		glVertex3f(1, -1.57, -4.86);
		glVertex3f(-width1, -1.15, -2.5);
		glVertex3f(1, -1.57, -4.86);
		glVertex3f(-width1, -1.15, -2.5);
		glVertex3f(width1, -1.15, -2.5);

		glVertex3f(-width1, -1.15, -2.5);
		glVertex3f(width1, -1.15, -2.5);
		glVertex3f(-width2, -1.05, 0.28);
		glVertex3f(-width2, -1.05, 0.28);
		glVertex3f(width1, -1.15, -2.5);
		glVertex3f(width2, -1.05, 0.28);

		glVertex3f(-width2, -1.05, 0.28);
		glVertex3f(width2, -1.05, 0.28);
		glVertex3f(-width3, -1.01, 3.26);
		glVertex3f(width2, -1.05, 0.28);
		glVertex3f(-width3, -1.01, 3.26);
		glVertex3f(width3, -1.01, 3.26);

		//snout
		glVertex3f(-1, -0.61, -5.42);
		glVertex3f(1, -0.61, -5.42);
		glVertex3f(-1, -1.57, -4.86);
		glVertex3f(1, -1.57, -4.86);
		glVertex3f(-1, -1.57, -4.86);
		glVertex3f(1, -0.61, -5.42);

		glColor3f(0.153, 0.376, 0.372); // blue stripe color
		//top
		glVertex3f(-1, -0.61, -5.42);
		glVertex3f(-width1, 0.67, -3.42);
		glVertex3f(1, -0.61, -5.42);
		glVertex3f(1, -0.61, -5.42);
		glVertex3f(-width1, 0.67, -3.42);
		glVertex3f(width1, 0.67, -3.42);

		glVertex3f(-width1, 0.67, -3.42);
		glVertex3f(width1, 0.67, -3.42);
		glVertex3f(-width2, 1.79, -0.86);
		glVertex3f(width1, 0.67, -3.42);
		glVertex3f(-width2, 1.79, -0.86);
		glVertex3f(width2, 1.79, -0.86);

		glVertex3f(-width2, 1.79, -0.86);
		glVertex3f(width2, 1.79, -0.86);
		glVertex3f(-width3, 2.75, 2.18);
		glVertex3f(width3, 2.75, 2.18);
		glVertex3f(width2, 1.79, -0.86);
		glVertex3f(-width3, 2.75, 2.18);

		glEnd();

	}
	void drawBody()
	{
		glColor3f(0.903, 0.830, 0.401); // yellow body color but darker
		drawSphere(20, 20, 10, 0, 20, 0, 20);

		glColor3f(0.153, 0.376, 0.372); // blue stripe color
		//drawSphere(20, 20, 10, 6, 13, 3, 14);
		drawSphere(20, 20, 10, 0, 20, 6, 13);
	}
	void drawTriangle(float f)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(0.0f, f, 0.0f);
		glEnd();
	}
	void drawPokemon(const Matrix& viewMatrix, float size, float scaleAmp)
	{
		glLoadMatrixf((GLfloat*)viewMatrix.mVal);
		Matrix modelMatrix, viewSpaceMatrix, scale, scale2, scale3, translate, rotate, rotate2, rotate3, rotate4, rotate5;
		rotate5 = Matrix::makeRotateMatrix(pokemonRotation, Vector(0.0f, 1.0f, 0.0f));
		scale3 = Matrix::makeScaleMatrix(size + (spectrumAverage * scaleAmp), size + (spectrumAverage * scaleAmp), size + (spectrumAverage * scaleAmp));
		// head 
		scale = Matrix::makeScaleMatrix(0.1f, 0.1f, 0.1f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.0f, 1.0f, 1.0f);
		rotate = Matrix::makeRotateMatrix(90, Vector(1.0f, 0.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(-90, Vector(0.0f, 1.0f, 0.0f));
		modelMatrix = scale3 * rotate5 * translate * rotate * rotate2 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawHead();

		scale = Matrix::makeScaleMatrix(0.205f, 0.25f, 0.22f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.0f, 0.15f, 1.95f);
		rotate = Matrix::makeRotateMatrix(180, Vector(0.0f, 1.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(25, Vector(1.0f, 0.0f, 0.0f));
		modelMatrix = scale3 * rotate5 * translate * rotate * rotate2 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawSnout();

		// body
		scale = Matrix::makeScaleMatrix(0.1f, 0.1f, 0.1f);
		scale2 = Matrix::makeScaleMatrix(1.0f, 1.0f, 1.5f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.0f, -0.2f, -0.45f);
		rotate = Matrix::makeRotateMatrix(45, Vector(1.0f, 0.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(0, Vector(0.0f, 1.0f, 0.0f));
		rotate3 = Matrix::makeRotateMatrix(90, Vector(0.0f, 0.0f, 1.0f));
		modelMatrix = scale3 * rotate5 * translate * rotate * rotate2 * rotate3 * scale * scale2;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawBody();

		// arms
		scale = Matrix::makeScaleMatrix(0.04f, 0.04f, 0.04f);
		scale2 = Matrix::makeScaleMatrix(0.8f, 1.95f, 0.85f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.8f, -0.4f, 0.3f);
		rotate = Matrix::makeRotateMatrix(55, Vector(1.0f, 0.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(45, Vector(0.0f, 1.0f, 0.0f));
		rotate3 = Matrix::makeRotateMatrix(115, Vector(0.0f, 0.0f, 1.0f));
		rotate4 = Matrix::makeRotateMatrix((-15 * sin((rotation - 1) / 3) - 25), Vector(0.15f, 0.0f, 1.0f));
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate2 * rotate * rotate3 * scale * scale2;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.933, 0.870, 0.431); // yellow body color
		drawSphere(20, 20, 10, 0, 20, 0, 20);

		// other arm
		translate = Matrix::Matrix::makeTranslationMatrix(-0.8f, -0.4f, 0.3f);
		rotate2 = Matrix::makeRotateMatrix(-45, Vector(0.0f, 1.0f, 0.0f));
		rotate3 = Matrix::makeRotateMatrix(65, Vector(0.0f, 0.0f, 1.0f));
		rotate4 = Matrix::makeRotateMatrix((15 * sin((rotation - 1) / 3) + 25), Vector(0.15f, 0.0f, 1.0f));
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate2 * rotate * rotate3 * scale * scale2;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawSphere(20, 20, 10, 0, 20, 0, 20);

		//legs
		scale = Matrix::makeScaleMatrix(0.06f, 0.06f, 0.06f);
		scale2 = Matrix::makeScaleMatrix(0.8f, 1.0f, 1.0f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.75f, -0.9f, -0.85f);
		modelMatrix = scale3 * rotate5 * translate * scale * scale2;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.933, 0.870, 0.431); // yellow body color
		drawSphere(20, 20, 10, 0, 20, 0, 20);

		scale2 = Matrix::makeScaleMatrix(0.3f, 0.3f, 0.8f);
		rotate = Matrix::makeRotateMatrix(25, Vector(1.0f, 0.0f, 0.0f));
		translate = Matrix::Matrix::makeTranslationMatrix(0.75f, -1.4f, -0.85f);
		modelMatrix = scale3 * rotate5 * translate * rotate * scale * scale2;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawSphere(20, 20, 10, 0, 20, 0, 20);

		scale2 = Matrix::makeScaleMatrix(0.2f, 0.2f, 0.5f);
		rotate = Matrix::makeRotateMatrix(-45, Vector(1.0f, 0.0f, 0.0f));
		translate = Matrix::Matrix::makeTranslationMatrix(0.75f, -1.73f, -1.06f);
		modelMatrix = scale3 * rotate5 * translate * rotate * scale * scale2;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawSphere(20, 20, 10, 0, 20, 0, 20);

		glColor3f(1.0, 1.0, 1.0); // toe color
		drawSphere(20, 20, 10, 0, 7, 0, 20);

		scale = Matrix::makeScaleMatrix(0.06f, 0.06f, 0.06f);
		scale2 = Matrix::makeScaleMatrix(0.8f, 1.0f, 1.0f);
		translate = Matrix::Matrix::makeTranslationMatrix(-0.75f, -0.9f, -0.85f);
		modelMatrix = scale3 * rotate5 * translate * scale * scale2;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.933, 0.870, 0.431); // yellow body color
		drawSphere(20, 20, 10, 0, 20, 0, 20);

		scale2 = Matrix::makeScaleMatrix(0.3f, 0.3f, 0.8f);
		rotate = Matrix::makeRotateMatrix(25, Vector(1.0f, 0.0f, 0.0f));
		translate = Matrix::Matrix::makeTranslationMatrix(-0.75f, -1.4f, -0.85f);
		modelMatrix = scale3 * rotate5 * translate * rotate * scale * scale2;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawSphere(20, 20, 10, 0, 20, 0, 20);

		scale2 = Matrix::makeScaleMatrix(0.2f, 0.2f, 0.5f);
		rotate = Matrix::makeRotateMatrix(-45, Vector(1.0f, 0.0f, 0.0f));
		translate = Matrix::Matrix::makeTranslationMatrix(-0.75f, -1.73f, -1.06f);
		modelMatrix = scale3 * rotate5 * translate * rotate * scale * scale2;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawSphere(20, 20, 10, 0, 20, 0, 20);

		glColor3f(1.0, 1.0, 1.0); // toe color
		drawSphere(20, 20, 10, 0, 7, 0, 20);

		//fire
		//left
		scale = Matrix::makeScaleMatrix(0.8f, 0.5f, 0.8f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.8f, 0.6f, -0.65f);
		rotate = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(-35, Vector(0.0f, 0.0f, 1.0f));
		rotate3 = Matrix::makeRotateMatrix(30, Vector(1.0f, 0.0f, 0.0f));
		rotate4 = Matrix::makeRotateMatrix(-5, Vector(0.0f, 1.0f, 0.0f));
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate * rotate2 * rotate3 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.871, 0.251, 0.016); // outer fire
		drawTriangle(-2 * sin((scalar - 1) / 5) + 5);

		scale = Matrix::makeScaleMatrix(0.6f, 0.3f, 0.6f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.81f, 0.6f, -0.65f);
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate * rotate2 * rotate3 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(1.0, 0.875, 0.196); // inner fire
		drawTriangle(-2 * sin((scalar - 1) / 5) + 5);
		//2
		scale = Matrix::makeScaleMatrix(0.8f, 0.35f, 0.8f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.8f, -0.0f, -1.15f);
		rotate = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(-65, Vector(0.0f, 0.0f, 1.0f));
		rotate3 = Matrix::makeRotateMatrix(25, Vector(1.0f, 0.0f, 0.0f));
		rotate4 = Matrix::makeRotateMatrix(-5, Vector(0.0f, 1.0f, 0.0f));
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate * rotate2 * rotate3 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.871, 0.251, 0.016); // outer fire
		drawTriangle(-2 * sin((scalar - 1) / 4) + 5);

		scale = Matrix::makeScaleMatrix(0.65f, 0.25f, 0.55f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.83f, -0.0f, -1.15f);
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate * rotate2 * rotate3 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(1.0, 0.875, 0.196); // inner fire
		drawTriangle(-2 * sin((scalar - 1) / 4) + 5);
		//3
		scale = Matrix::makeScaleMatrix(0.55f, 0.35f, 0.55f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.58f, 0.75f, -0.35f);
		rotate = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(-5, Vector(0.0f, 0.0f, 1.0f));
		rotate3 = Matrix::makeRotateMatrix(25, Vector(1.0f, 0.0f, 0.0f));
		rotate4 = Matrix::makeRotateMatrix(15, Vector(0.0f, 1.0f, 0.0f));
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate * rotate2 * rotate3 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.871, 0.251, 0.016); // outer fire
		drawTriangle(-2 * sin((scalar - 5) / 6) + 5);

		scale = Matrix::makeScaleMatrix(0.4f, 0.15f, 0.4f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.64f, 0.75f, -0.35f);
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate * rotate2 * rotate3 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(1.0, 0.875, 0.196); // inner fire
		drawTriangle(-2 * sin((scalar - 5) / 6) + 5);
		// right
		scale = Matrix::makeScaleMatrix(0.8f, 0.5f, 0.8f);
		translate = Matrix::Matrix::makeTranslationMatrix(-0.8f, 0.6f, -0.65f);
		rotate = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(-35, Vector(0.0f, 0.0f, 1.0f));
		rotate3 = Matrix::makeRotateMatrix(-30, Vector(1.0f, 0.0f, 0.0f));
		rotate4 = Matrix::makeRotateMatrix(5, Vector(0.0f, 1.0f, 0.0f));
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate * rotate2 * rotate3 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.871, 0.251, 0.016); // outer fire
		drawTriangle(-2 * sin((scalar - 1) / 5) + 5);

		scale = Matrix::makeScaleMatrix(0.6f, 0.3f, 0.6f);
		translate = Matrix::Matrix::makeTranslationMatrix(-0.81f, 0.6f, -0.65f);
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate * rotate2 * rotate3 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(1.0, 0.875, 0.196); // inner fire
		drawTriangle(-2 * sin((scalar - 1) / 5) + 5);
		//2
		scale = Matrix::makeScaleMatrix(0.8f, 0.35f, 0.8f);
		translate = Matrix::Matrix::makeTranslationMatrix(-0.8f, -0.0f, -1.15f);
		rotate = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(-65, Vector(0.0f, 0.0f, 1.0f));
		rotate3 = Matrix::makeRotateMatrix(-25, Vector(1.0f, 0.0f, 0.0f));
		rotate4 = Matrix::makeRotateMatrix(5, Vector(0.0f, 1.0f, 0.0f));
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate * rotate2 * rotate3 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.871, 0.251, 0.016); // outer fire
		drawTriangle(-2 * sin((scalar - 1) / 4) + 5);

		scale = Matrix::makeScaleMatrix(0.65f, 0.25f, 0.55f);
		translate = Matrix::Matrix::makeTranslationMatrix(-0.83f, -0.0f, -1.15f);
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate * rotate2 * rotate3 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(1.0, 0.875, 0.196); // inner fire
		drawTriangle(-2 * sin((scalar - 1) / 4) + 5);
		//3
		scale = Matrix::makeScaleMatrix(0.55f, 0.35f, 0.55f);
		translate = Matrix::Matrix::makeTranslationMatrix(-0.58f, 0.75f, -0.35f);
		rotate = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(-5, Vector(0.0f, 0.0f, 1.0f));
		rotate3 = Matrix::makeRotateMatrix(-25, Vector(1.0f, 0.0f, 0.0f));
		rotate4 = Matrix::makeRotateMatrix(-15, Vector(0.0f, 1.0f, 0.0f));
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate * rotate2 * rotate3 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.871, 0.251, 0.016); // outer fire
		drawTriangle(-2 * sin((scalar - 5) / 6) + 5);

		scale = Matrix::makeScaleMatrix(0.4f, 0.15f, 0.4f);
		translate = Matrix::Matrix::makeTranslationMatrix(-0.64f, 0.75f, -0.35f);
		modelMatrix = scale3 * rotate5 * rotate4 * translate * rotate * rotate2 * rotate3 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(1.0, 0.875, 0.196); // inner fire
		drawTriangle(-2 * sin((scalar - 5) / 6) + 5);

		//middle
		scale = Matrix::makeScaleMatrix(0.55f, 0.35f, 0.55f);
		translate = Matrix::Matrix::makeTranslationMatrix(0, 0.40f, -1.40f);
		rotate = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(-60, Vector(0.0f, 0.0f, 1.0f));
		modelMatrix = scale3 * rotate5 * translate * rotate * rotate2 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.871, 0.251, 0.016); // outer fire
		drawTriangle(-2 * sin((scalar - 7) / 6) + 5);

		scale = Matrix::makeScaleMatrix(0.4f, 0.2f, 0.4f);
		translate = Matrix::Matrix::makeTranslationMatrix(-0.01, 0.40f, -1.40f);
		modelMatrix = scale3 * rotate5 * translate * rotate * rotate2 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(1.0, 0.875, 0.196); // inner fire
		drawTriangle(-2 * sin((scalar - 7) / 6) + 5);

		scale = Matrix::makeScaleMatrix(0.4f, 0.2f, 0.4f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.01, 0.40f, -1.40f);
		modelMatrix = scale3 * rotate5 * translate * rotate * rotate2 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawTriangle(-2 * sin((scalar - 7) / 6) + 5);
		//2
		scale = Matrix::makeScaleMatrix(0.65f, 0.45f, 0.65f);
		translate = Matrix::Matrix::makeTranslationMatrix(0, 0.0f, -1.75f);
		rotate = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(-80, Vector(0.0f, 0.0f, 1.0f));
		modelMatrix = scale3 * rotate5 * translate * rotate * rotate2 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.871, 0.251, 0.016); // outer fire
		drawTriangle(-2 * sin((scalar - 12) / 6) + 5);

		scale = Matrix::makeScaleMatrix(0.5f, 0.35f, 0.5f);
		translate = Matrix::Matrix::makeTranslationMatrix(-0.01, 0.0f, -1.75f);
		modelMatrix = scale3 * rotate5 * translate * rotate * rotate2 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(1.0, 0.875, 0.196); // inner fire
		drawTriangle(-2 * sin((scalar - 12) / 6) + 5);

		scale = Matrix::makeScaleMatrix(0.5f, 0.35f, 0.5f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.01, 0.0f, -1.75f);
		modelMatrix = scale3 * rotate5 * translate * rotate * rotate2 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawTriangle(-2 * sin((scalar - 12) / 6) + 5);
		//3
		scale = Matrix::makeScaleMatrix(0.35f, 0.15f, 0.35f);
		translate = Matrix::Matrix::makeTranslationMatrix(0, -0.4f, -1.85f);
		rotate = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		rotate2 = Matrix::makeRotateMatrix(-100, Vector(0.0f, 0.0f, 1.0f));
		modelMatrix = scale3 * rotate5 * translate * rotate * rotate2 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.871, 0.251, 0.016); // outer fire
		drawTriangle(-2 * sin((scalar - 8) / 6) + 5);

		scale = Matrix::makeScaleMatrix(0.15f, 0.08f, 0.15f);
		translate = Matrix::Matrix::makeTranslationMatrix(-0.01, -0.4f, -1.75f);
		modelMatrix = scale3 * rotate5 * translate * rotate * rotate2 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(1.0, 0.875, 0.196); // inner fire
		drawTriangle(-2 * sin((scalar - 8) / 6) + 5);

		scale = Matrix::makeScaleMatrix(0.15f, 0.08f, 0.15f);
		translate = Matrix::Matrix::makeTranslationMatrix(0.01, -0.4f, -1.75f);
		modelMatrix = scale3 * rotate5 * translate * rotate * rotate2 * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawTriangle(-2 * sin((scalar - 8) / 6) + 5);
	}
	//
	void drawLightning(Vertex v1, Vertex v2, float displacement, float amount)
	{
		//glLineWidth(20);
		glBegin(GL_LINES);

		if (displacement < amount)
		{
			glVertex3f(v1.x, v1.y, v1.z);
			glVertex3f(v2.x, v2.y, v2.z);
		}
		else
		{
			Vertex midV;

			midV.x = (v2.x + v1.x) / 2;
			midV.y = (v2.y + v1.y) / 2;
			midV.z = (v2.z + v2.z) / 2;

			//midV.x += (rand() % 2 - 0.5f) * displacement;
			midV.y += (rand() % 2 - 0.5f) * displacement;
			//midV.z += (rand() % 2 - 0.5f) * displacement;

			drawLightning(v1, midV, displacement / 2, amount);
			drawLightning(v2, midV, displacement / 2, amount);
		}
		glEnd();
	}
	void drawBar(const Matrix& viewMatrix)
	{
		Matrix translate, translate2, scale1, scale2, rotate1, modelMatrix, viewSpaceMatrix;

		GLfloat verticesRectangle[] =
		{
			-1.0f,-1.0f,1.0f,
			1.0f,-1.0f,1.0f,
			1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,
			-1.0f,1.0f,1.0f,
			-1.0f,-1.0f,1.0f,
		};

		glEnableClientState(GL_VERTEX_ARRAY);

		float averages[VISUALIZER_SIZE];
		//Rectangle bar values
		float barSize = 0.1f;
		float barScale = 2;
		//color
		float H = 2;
		float U = cos(H * M_PI / 180);
		float W = sin(H * M_PI / 180);
		//float colorR = 1.0, colorG = 0.5, colorB = 0.2;
		float colorR = 1.0, colorG = 0.6, colorB = 0.0;
		float tempR, tempG, tempB;
		//draw
		bool changeOrder = false;
		int f = 0;
		for (int i = 0; i < VISUALIZER_SIZE; i++)
		{
			if (f >= VISUALIZER_SIZE / 5)
				changeOrder = true;
			if (f <= 0)
				changeOrder = false;
			if (changeOrder)
				f--;
			else
				f++;
			averages[i] = m_spectrumLeft[f] + m_spectrumRight[f] / 2;
			//transform
			translate = Matrix::makeTranslationMatrix(((i) * 2.5), 0.0f, 0.0f); //spread em in a line
			translate2 = Matrix::makeTranslationMatrix(-12.0f, -6.28f, 0.0f);
			scale1 = Matrix::makeScaleMatrix(0.05f, averages[i] * barScale + barSize, 0.05f);
			scale2 = Matrix::makeScaleMatrix(5.0f,5.0f,5.0f);
			modelMatrix = scale2* translate2 * scale1 * translate;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			glColor3f(colorR, colorG, colorB);
			//draw rectangle bar
			glVertexPointer(3, GL_FLOAT, 0, verticesRectangle);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			//change hue
			tempR = rHue(colorR, colorG, colorB, U, W);
			tempG = gHue(colorR, colorG, colorB, U, W);
			tempB = bHue(colorR, colorG, colorB, U, W);
			colorR = tempR;
			colorG = tempG;
			colorB = tempB;
		}
		
		//reverse
		barScale = 1.5;
		for (int i = 0; i < VISUALIZER_SIZE; i++)
		{
			if (f >= VISUALIZER_SIZE / 5)
				changeOrder = true;
			if (f <= 0)
				changeOrder = false;
			if (changeOrder)
				f--;
			else
				f++;
			averages[i] = m_spectrumLeft[f] + m_spectrumRight[f] / 2;
			//transform
			translate = Matrix::makeTranslationMatrix(((i) * 2.5), 0.0f, 0.0f); //spread em in a line
			translate2 = Matrix::makeTranslationMatrix(-13.1f, -6.31f, 0.0f);
			scale1 = Matrix::makeScaleMatrix(0.05f, averages[i] * barScale + barSize, 0.05f);
			scale2 = Matrix::makeScaleMatrix(5.0f, 5.0f, 5.0f);
			rotate1 = Matrix::makeRotateMatrix(180, Vector(1.0f, 0.0f, 0.0f));
			modelMatrix = rotate1 * scale2 * translate2 * scale1 * translate ;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			glColor3f(colorR, colorG, colorB);
			//draw rectangle bar
			glVertexPointer(3, GL_FLOAT, 0, verticesRectangle);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			//change hue
			tempR = rHue(colorR, colorG, colorB, U, W);
			tempG = gHue(colorR, colorG, colorB, U, W);
			tempB = bHue(colorR, colorG, colorB, U, W);
			colorR = tempR;
			colorG = tempG;
			colorB = tempB;
		}
		for (int i = 0; i < VISUALIZER_SIZE; i++)
		{
			if (f >= VISUALIZER_SIZE / 5)
				changeOrder = true;
			if (f <= 0)
				changeOrder = false;
			if (changeOrder)
				f--;
			else
				f++;
			averages[i] = m_spectrumLeft[f] + m_spectrumRight[f] / 2;
			//transform
			translate = Matrix::makeTranslationMatrix(((i) * 2.5), 0.0f, 0.0f); //spread em in a line
			translate2 = Matrix::makeTranslationMatrix(-13.0f, -6.28f, 0.0f);
			scale1 = Matrix::makeScaleMatrix(0.05f, averages[i] * barScale + barSize, 0.05f);
			scale2 = Matrix::makeScaleMatrix(5.0f, 5.0f, 5.0f);
			modelMatrix = scale2 * translate2 * scale1 * translate;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			glColor3f(colorR, colorG, colorB);
			//draw rectangle bar
			glVertexPointer(3, GL_FLOAT, 0, verticesRectangle);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			//change hue
			tempR = rHue(colorR, colorG, colorB, U, W);
			tempG = gHue(colorR, colorG, colorB, U, W);
			tempB = bHue(colorR, colorG, colorB, U, W);
			colorR = tempR;
			colorG = tempG;
			colorB = tempB;
		}

		//reverse
		barScale = 1.5;
		for (int i = 0; i < VISUALIZER_SIZE; i++)
		{
			if (f >= VISUALIZER_SIZE / 5)
				changeOrder = true;
			if (f <= 0)
				changeOrder = false;
			if (changeOrder)
				f--;
			else
				f++;
			averages[i] = m_spectrumLeft[f] + m_spectrumRight[f] / 2;
			//transform
			translate = Matrix::makeTranslationMatrix(((i) * 2.5), 0.0f, 0.0f); //spread em in a line
			translate2 = Matrix::makeTranslationMatrix(-12.1f, -6.31f, 0.0f);
			scale1 = Matrix::makeScaleMatrix(0.05f, averages[i] * barScale + barSize, 0.05f);
			scale2 = Matrix::makeScaleMatrix(5.0f, 5.0f, 5.0f);
			rotate1 = Matrix::makeRotateMatrix(180, Vector(1.0f, 0.0f, 0.0f));
			modelMatrix = rotate1 * scale2 * translate2 * scale1 * translate;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			glColor3f(colorR, colorG, colorB);
			//draw rectangle bar
			glVertexPointer(3, GL_FLOAT, 0, verticesRectangle);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			//change hue
			tempR = rHue(colorR, colorG, colorB, U, W);
			tempG = gHue(colorR, colorG, colorB, U, W);
			tempB = bHue(colorR, colorG, colorB, U, W);
			colorR = tempR;
			colorG = tempG;
			colorB = tempB;
		}

		for (int i = 0; i < VISUALIZER_SIZE; i++)
		{
			if (f >= VISUALIZER_SIZE / 8)
				changeOrder = true;
			if (f <= 0)
				changeOrder = false;
			if (changeOrder)
				f--;
			else
				f++;
			averages[i] = m_spectrumLeft[f] + m_spectrumRight[f] / 2;
			//transform
			translate = Matrix::makeTranslationMatrix(((i) * 2.5), 0.0f, 0.0f); //spread em in a line
			translate2 = Matrix::makeTranslationMatrix(-13.0f, -6.28f, 0.0f);
			scale1 = Matrix::makeScaleMatrix(0.05f, averages[i] * barScale + barSize, 0.05f);
			scale2 = Matrix::makeScaleMatrix(5.0f, 5.0f, 5.0f);
			modelMatrix = scale2 * translate2 * scale1 * translate;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			glColor3f(colorR, colorG, colorB);
			//draw rectangle bar
			glVertexPointer(3, GL_FLOAT, 0, verticesRectangle);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			//change hue
			tempR = rHue(colorR, colorG, colorB, U, W);
			tempG = gHue(colorR, colorG, colorB, U, W);
			tempB = bHue(colorR, colorG, colorB, U, W);
			colorR = tempR;
			colorG = tempG;
			colorB = tempB;
		}

		//reverse
		barScale = 1.5;
		for (int i = 0; i < VISUALIZER_SIZE; i++)
		{
			if (f >= VISUALIZER_SIZE / 3)
				changeOrder = true;
			if (f <= 0)
				changeOrder = false;
			if (changeOrder)
				f--;
			else
				f++;
			averages[i] = m_spectrumLeft[f] + m_spectrumRight[f] / 2;
			//transform
			translate = Matrix::makeTranslationMatrix(((i) * 2.5), 0.0f, 0.0f); //spread em in a line
			translate2 = Matrix::makeTranslationMatrix(-12.1f, -6.31f, 0.0f);
			scale1 = Matrix::makeScaleMatrix(0.05f, averages[i] * barScale + barSize, 0.05f);
			scale2 = Matrix::makeScaleMatrix(5.0f, 5.0f, 5.0f);
			rotate1 = Matrix::makeRotateMatrix(180, Vector(1.0f, 0.0f, 0.0f));
			modelMatrix = rotate1 * scale2 * translate2 * scale1 * translate;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			glColor3f(colorR, colorG, colorB);
			//draw rectangle bar
			glVertexPointer(3, GL_FLOAT, 0, verticesRectangle);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			//change hue
			tempR = rHue(colorR, colorG, colorB, U, W);
			tempG = gHue(colorR, colorG, colorB, U, W);
			tempB = bHue(colorR, colorG, colorB, U, W);
			colorR = tempR;
			colorG = tempG;
			colorB = tempB;
		}
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	void drawRectangle(int texture, Vertex x1, Vertex y1, Vertex x2, Vertex y2)
	{
		GLfloat vertices[] =
		{
			x1.x ,x1.y, 0.0f,
			y1.x ,y1.y, 0.0f,
			y2.x ,y2.y, 0.0f,
			y2.x ,y2.y, 0.0f,
			x2.x ,x2.y, 0.0f,
			x1.x ,x1.y, 0.0f,
		};

		GLfloat texCoords[] =
		{
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
		};
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, mTextureID[texture]);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		

	}
	void drawCircle(float texture, float size, float radius, float bar_width, float height, float amplify)
	{
		float averages[VISUALIZER_SIZE];
		float bar_height;
		float rads = M_PI * 2 / size;
		//color values
		glColor3f(1.0f, 1.0f, 1.0f);
		//vertex
		Vertex pos1, pos2;
		Vertex midpoint;
		Vertex xPos, yPos, xPos2, yPos2;
		Vertex facingVector; facingVector.x = 1.0f; facingVector.y = 1.0f; facingVector.z = 1.0f;
		float m;
		//draw
		bool changeOrder = false;
		int f = 0;
		for (int i = 0; i < size; i++)
		{
			if (f >= size / 4)
				changeOrder = true;
			if (f <= 0)
				changeOrder = false;
			if (changeOrder)
				f--;
			else
				f++;
			averages[i] = m_spectrumLeft[f] + m_spectrumRight[f] / 2;

			bar_height = height + (averages[i] * amplify);

			pos1.x = cos(rads * i)* (radius);
			pos1.y = sin(rads * i)* (radius); //pos1.z = 0;
			pos2.x = cos(rads * i) * (radius + bar_height);
			pos2.y = sin(rads * i) * (radius + bar_height); //pos2.z = 0;

			//Perpendicular
			Vertex vec1;
			vec1.x = pos2.x - pos1.x;
			vec1.y = pos2.y - pos1.y;
			//Normalize
			m = sqrt((vec1.x * vec1.x) + (vec1.y * vec1.y) + (vec1.z * vec1.z)); // magnitude
			vec1.x /= m;
			vec1.y /= m;
			vec1.z /= m;

			Vertex vec2;
			//Cross Product
			vec2.x = vec1.y * facingVector.z - vec1.z * facingVector.y;
			vec2.y = -(vec1.x * facingVector.z - vec1.z * facingVector.x);
			vec2.z = vec1.x * facingVector.y - vec1.y * facingVector.x;
			//Normalize
			m = sqrt((vec2.x * vec2.x) + (vec2.y * vec2.y) + (vec2.z * vec2.z)); // magnitude
			vec2.x /= m;
			vec2.y /= m;
			vec2.z /= m;
			
			midpoint.x = pos1.x; 
			midpoint.y = pos1.y; //midpoint.z = pos1.z * 0.5f;
			xPos.x = midpoint.x + vec2.x * bar_width;
			xPos.y = midpoint.y + vec2.y * bar_width; //xPos.z = midpoint.z + vec2.z * 0.2f;
			yPos.x = midpoint.x - vec2.x * bar_width;
			yPos.y = midpoint.y - vec2.y * bar_width; //yPos.z = midpoint.z - vec2.z * 0.2f;

			midpoint.x = (pos1.x + pos2.x) * 0.5;
			midpoint.y = (pos1.y + pos2.y) * 0.5; //midpoint.z = (pos1.z + pos2.z) * 0.5;
			xPos2.x = midpoint.x + vec2.x * bar_width;
			xPos2.y = midpoint.y + vec2.y * bar_width; //xPos2.z = midpoint.z + vec2.z * 0.2f;
			yPos2.x = midpoint.x - vec2.x * bar_width;
			yPos2.y = midpoint.y - vec2.y * bar_width; //yPos2.z = midpoint.z - vec2.z * 0.2f;
			//draw
			drawRectangle(texture, xPos, yPos, pos2, pos2);
		}
	}
	void drawCircle2(float size, float radius, float height, float hueChangeValue, float amplify, float randomAmp)
	{
		float averages[VISUALIZER_SIZE];
		float bar_height, random;
		radius = radius;
		float rads = M_PI * 2 / size;
		//color values
		float H = hueChangeValue;
		float U = cos(H * M_PI / 180);
		float W = sin(H * M_PI / 180);
		//float colorR = 1.0, colorG = 0.4, colorB = 0.2; // 
		float colorR = 1.0, colorG = 0.6, colorB = 0.0; // slightly more pastel ish
		float tempR, tempG, tempB;
		//vertex
		Vertex pos1, pos2;
		Vertex lastPos1, lastPos2, firstPos1, firstPos2;
		Vertex midpoint;
		Vertex xPos, yPos, xPos2, yPos2;
		Vertex facingVector; facingVector.x = 1.0f; facingVector.y = 1.0f; facingVector.z = 1.0f;
		float m;
		//draw
		bool changeOrder = false;
		int f = 0;
		for (int i = 0; i < size; i++)
		{
			if (f >= size/3)
				changeOrder = true;
			if (f <= 0)
				changeOrder = false;
			if (changeOrder)
				f--;
			else
				f++;
			averages[i] = (m_spectrumLeft[f] + m_spectrumRight[f] / 2) * amplify;

			random = (float(rand()) / float((RAND_MAX)) * randomAmp);
			bar_height = height + (averages[i]) + random;

			pos1.x = cos(rads * i) * (radius);
			pos1.y = sin(rads * i) * (radius); //pos1.z = 0;
			pos2.x = cos(rads * i) * (radius + bar_height);
			pos2.y = sin(rads * i) * (radius + bar_height); //pos2.z = 0;

			//draw
			glColor3f(colorR, colorG, colorB);
			if (i == 0)
			{
				firstPos1 = pos1;
				firstPos2 = pos2;
			}
			else
			{
				drawRectangle(0, lastPos1, lastPos2, pos1, pos2);
			}

			lastPos1 = pos1;
			lastPos2 = pos2;
			//changeHue
			tempR = rHue(colorR, colorG, colorB, U, W);
			tempG = gHue(colorR, colorG, colorB, U, W);
			tempB = bHue(colorR, colorG, colorB, U, W);
			colorR = tempR;
			colorG = tempG;
			colorB = tempB;
		}
		drawRectangle(0, pos1, pos2, firstPos1, firstPos2);

		//colorR = 1.0, colorG = 0.4, colorB = 0.2; // 
		colorR = 1.0, colorG = 0.6, colorB = 0.0; // 
		for (int i = 0; i < size; i++)
		{
			if (f >= size/2.5)
				changeOrder = true;
			if (f <= 0)
				changeOrder = false;
			if (changeOrder)
				f--;
			else
				f++;
			averages[i] = (m_spectrumLeft[f] + m_spectrumRight[f] / 2) * amplify;

			random = (float(rand()) / float((RAND_MAX)) * randomAmp);
			bar_height = height + (averages[i]) + random;

			pos1.x = cos(rads * i) * (radius);
			pos1.y = sin(rads * i) * (radius); //pos1.z = 0;
			pos2.x = cos(rads * i) * (radius - bar_height);
			pos2.y = sin(rads * i) * (radius - bar_height); //pos2.z = 0;

			//draw
			glColor3f(colorR, colorG, colorB);
			if (i == 0)
			{
				firstPos1 = pos1;
				firstPos2 = pos2;
			}
			else
			{
				drawRectangle(0, lastPos1, lastPos2, pos1, pos2);
			}

			lastPos1 = pos1;
			lastPos2 = pos2;
			//changeHue
			tempR = rHue(colorR, colorG, colorB, U, W);
			tempG = gHue(colorR, colorG, colorB, U, W);
			tempB = bHue(colorR, colorG, colorB, U, W);
			colorR = tempR;
			colorG = tempG;
			colorB = tempB;
		}
		drawRectangle(0, pos1, pos2, firstPos1, firstPos2);
	}
	//
	float rotation1, rotation2, rotation3, rotation4, rotation5, rotation6, rotation7; 
	float displacement;
	void draw(const Matrix& viewMatrix)
	{
		updateFmod();
		//drawWorldOriginLines(viewMatrix); // draw Axis
		glColor3f(1.0f, 1.0f, 1.0f);
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		//drawBar(viewMatrix);
		spectrumAverage = ((m_spectrumLeft[1] + m_spectrumRight[1] / 2) * 1); // CHANGE SPECTRUM CHANNEL TO 0 IF USING POKEMON LANCE THEME

		Matrix translate, scale1, rotate1, modelMatrix, viewSpaceMatrix;

		//Bars
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBindTexture(GL_TEXTURE_2D, mTextureID[0]);
		drawBar(viewMatrix);
		glDisable(GL_BLEND);
		//Pokemon
		/*rotation += 0.5f + (1.0f * spectrumAverage);
		scalar += 0.5f + (10.0f * spectrumAverage);
		pokemonRotation += 0.2f + (10.0f * spectrumAverage);
		glBindTexture(GL_TEXTURE_2D, mTextureID[0]);
		drawPokemon(viewMatrix, 1.2, 2);*/

		//Lightning
		translate = Matrix::makeTranslationMatrix(0.0f, 0.0f, 0.0f);
		modelMatrix = translate;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(1.0f, 1.0f, 1.0f);

		Vertex lightningPos1;
		lightningPos1.x = -10.0f;
		Vertex lightningPos2;
		lightningPos2.x = 10.0f;
		displacement = spectrumAverage * 10;
		glBindTexture(GL_TEXTURE_2D, mTextureID[0]);
		drawLightning(lightningPos1, lightningPos2, displacement, 0.05f);

		//Circle
		rotation1 -= 1.0f * (spectrumAverage * 10);
		translate = Matrix::makeTranslationMatrix(0.0f, 0.0f, 0.0f);
		rotate1 = Matrix::makeRotateMatrix(rotation1, Vector(0.0f, 0.0f, 1.0f));
		modelMatrix = translate * rotate1;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCircle(1, 128,13.5, 0.2, 0 , 50);

		//Circle2
		rotation2 += 1.0f + (spectrumAverage * 10);
		translate = Matrix::makeTranslationMatrix(0.0f, 0.0f, 0.0f);
		rotate1 = Matrix::makeRotateMatrix(rotation2, Vector(0.0f, 0.0f, 1.0f));
		modelMatrix = translate * rotate1;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCircle2(68, 10.2, 0.4, 5.5, 4, 0.3); // fluid version but colors repeat only once
		//drawCircle2(68, 10.2, 0.4, 11, 4, 0.3); // new one fluid version 
		//drawCircle2(256, 10.2, 0.2, 2.8, 3, 0.2); // old one jaggy version
		

		//Circle3
		rotation3 += 0.2f + (spectrumAverage * 0.5);
		translate = Matrix::makeTranslationMatrix(0.0f, 0.0f, 0.0f);
		rotate1 = Matrix::makeRotateMatrix(rotation3, Vector(0.0f, 1.0f, 1.0f));
		modelMatrix = translate * rotate1;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCircle(1, 4, 20 + spectrumAverage * 5, 0.2, 0.5, 1);

		//Circle4
		rotation4 -= 0.3f + (spectrumAverage * 0.5);
		translate = Matrix::makeTranslationMatrix(0.0f, 0.0f, 0.0f);
		rotate1 = Matrix::makeRotateMatrix(rotation4, Vector(1.0f, 0.0f, 1.0f));
		modelMatrix = translate * rotate1;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCircle(1, 6, 24 + spectrumAverage * 5, 0.2, 0.5, 1);

		//Circle5
		rotation5 += 0.4f + (spectrumAverage * 0.5);
		translate = Matrix::makeTranslationMatrix(0.0f, 0.0f, 0.0f);
		rotate1 = Matrix::makeRotateMatrix(rotation5, Vector(0.0f, 1.0f, 1.0f));
		modelMatrix = translate * rotate1;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCircle(1, 8, 28 + spectrumAverage * 5, 0.2, 0.5, 1);

		//Circle6
		rotation6 -= 0.5f + (spectrumAverage * 0.5);
		translate = Matrix::makeTranslationMatrix(0.0f, 0.0f, 0.0f);
		rotate1 = Matrix::makeRotateMatrix(rotation6, Vector(1.0f, 0.0f, 1.0f));
		modelMatrix = translate * rotate1;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCircle(1, 10, 32 + spectrumAverage * 5, 0.2, 0.5, 1);

		//Circle7
		rotation7 += 0.6f + (spectrumAverage * 0.5);
		translate = Matrix::makeTranslationMatrix(0.0f, 0.0f, 0.0f);
		rotate1 = Matrix::makeRotateMatrix(rotation7, Vector(0.0f, 1.0f, 1.0f));
		modelMatrix = translate * rotate1;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCircle(1, 12, 36 + spectrumAverage * 5, 0.2, 0.5, 1);

		
		
	}
};

#endif

