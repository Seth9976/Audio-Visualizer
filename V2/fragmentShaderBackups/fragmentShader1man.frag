precision mediump float;
varying vec4 fColor;
varying vec2 fTexCoord;

uniform sampler2D sampler2d;
uniform float Factor1; // spectrumAverage
uniform float Factor2; // +0.5f
uniform float Factor3;
uniform float Factor4;
uniform float Factor5;
uniform float Factor6;
uniform float Factor7;
uniform float Factor8;
uniform float Factor9;
uniform float Factor10;
uniform float Factor11;
uniform float Factor12;
uniform float Factor13;
uniform float Factor14;

bool Hex = false;
bool Orbit = false;
bool Ripples = false;
bool Circle = false;

float value(float x, float y)
{
	complex<float> point(x/width-1.5, y/height-0.5);
	
	complex<float> z(0.0, 0.0);
	
	unsigned int nb_iter = 0;
	while(abs(z) < 2 && nb_iter <= 34) {
		z= z * z+ point;
		nb_iter++;
	}
	if(nb_iter < 34)
		return 255;
	else return 0;
}

void main()
{	
	
	float val = value(gl_FragCoord.x, gl_FragCoord.y) 
	gl_FragColor = (val, 0.0, 0.0, 1.0);
}