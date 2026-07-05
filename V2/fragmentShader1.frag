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

void main()
{	
	float PI = 3.14159;
	float spectrumAverage = Factor1;
	//Colors
	float r,g,b;
	float newR, newG, newB;
	
	vec4 texColor = texture2D(sampler2d, fTexCoord);
	vec4 combinedColor, combinedColor2;
	combinedColor = fColor * texColor / (fColor * fColor);
	combinedColor2 = fColor * texColor / (texColor * texColor) / (fColor * fColor);
	
	newR = (r*0.39 + g*0.77 + b*0.19);
	newG = (r*0.35 + g*0.69 + b*0.17);
	newB = (r*0.27 + g*0.53 + b*0.13);
	r = newR;
	g = newG;
	b = newB;
	
	vec3 color1 = vec3(1.0, 1.0, 1.0);
	vec3 color2 = vec3(r, g, b);
	vec3 color3 = vec3(0.0, 0.0, 1.0);
	vec3 black = vec3(0.0, 0.0, 0.0);
	
	vec2 center = vec2(800.0,450.0);
	//SEIN WAEVS
	//VALUES
	float deThiccV = 0.1; // how THICC it is vertically
	float deThiccH = 5.0; // how THICC it is horizontally
	float deSpeedMultiplier = 2.0; // how FAST the sine wave travels
	float deHeight = 0.3; // how HIGH the sine wave is
	float deFattMan = 10.0; // how FATT the sine wave is
	float deIDKWhat = 1.0; // Idk what this effect is called
	color1 = vec3(1.0, 1.0, 1.0);
	color2 = vec3(0.5, 0.5, 0.5);
	//SINE 1
	vec2 resolution = vec2(800.0, 900.0); // the resoltuion !remember to change resolution in solution also
	float xCenter = gl_FragCoord.x / resolution.x; // sin wave X pixel
	float yCenter = gl_FragCoord.y / resolution.y; // sin wave Y pixel
	float curve = (deThiccV * Factor1 * 1.0) * sin((deThiccH * xCenter) + (deSpeedMultiplier * Factor14)); // curvy boi 
	float line = smoothstep(1.0 - clamp(distance(curve+yCenter, 0.5) * deFattMan, 0.0, 1.0), deIDKWhat, 0.99);
	     //DO NOT CHANGE THIS^                                             AND THIS^    ^idk what this does
	vec3 sineWave = (1.0 - line) * vec3(mix(color1, color2, line));
	//gl_FragColor = vec4(sineWave, 1.0);
	
	// PHASE 1
	color1 = vec3(1.0, 1.0, 1.0);
	color2 = vec3(0.0, 0.0, 1.0);
	float radius = Factor2;	
	float radius2 = 0.0;
	float radius3 = 0.0;
	float xDis = gl_FragCoord.x-center.x + (radius * cos(180.0 + (Factor14 * 0.1)));
	float yDis = gl_FragCoord.y-center.y + (radius * sin(180.0 + (Factor14 * 0.1)));
	float distanceSqr = xDis*xDis + yDis*yDis;
	if(distanceSqr <= radius)
	{
		gl_FragColor = vec4 ( color1, 1.0 );
	}
	xDis = gl_FragCoord.x-center.x - (radius * cos(180.0 + Factor14* 0.1));
	yDis = gl_FragCoord.y-center.y - (radius * sin(180.0 + Factor14* 0.1));
	distanceSqr = xDis*xDis + yDis*yDis;
	if(distanceSqr <= radius)
	{
		gl_FragColor = vec4 ( color1, 1.0 );
	}
	// PHASE 2
	if(Factor2 < 0.0)
	{
		radius = (Factor4 * Factor4 * 10.0) - 500.0;
		radius2 = (Factor4 * Factor4 * 10.0);
		xDis = gl_FragCoord.x-center.x;
		yDis = gl_FragCoord.y-center.y;
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr <= radius2)
			gl_FragColor = vec4(sineWave, 1.0);
		if(distanceSqr >= radius && distanceSqr <= radius2)
			gl_FragColor = vec4 ( color1, 1.0 );
	}
	
	//PHASE 3
	if(Factor3 < 0.0)
	{
		radius = (Factor5 * Factor5 * 10.0) - 500.0;
		radius2 = (Factor5 * Factor5 * 10.0);
		if(distanceSqr >= radius && distanceSqr <= radius2)
		{
			gl_FragColor = vec4 ( color1, 1.0 );
		}
		Hex = true;
		Orbit = true;
		Ripples = true;
		Circle = true;
	}
	float speed = Factor14;
	float speed2 = Factor14;
	float size = 20.0 - (spectrumAverage * 10.0);
	// Hex
	if(Hex)
	{
		float angle1 = 5.1; //WHY ARE THESE ANGLES SO WEIRD MR CHIN FOO?
		float angle2 = 180.0;
		float angle3 = 354.9; //???
		//Outside
		r = mod(0.2 + (Factor14 * 0.1) , 1.0);
		g = mod(0.95 + (Factor14 * 0.1) , 1.0);
		b = mod(0.7 + (Factor14 * 0.1) , 1.0);
		color2 = vec3(r, g, b);
		radius = Factor5;
		radius2 = (Factor5 );
		radius3 = (Factor5 ) + (Factor12 * 500.0);
		xDis = gl_FragCoord.x-center.x + (radius * cos(angle1 + speed* 0.1));
		yDis = gl_FragCoord.y-center.y + (radius * sin(angle1 + speed* 0.1));
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( color2, 1.0 );
		xDis = gl_FragCoord.x-center.x - (radius * cos(angle1 + speed* 0.1));
		yDis = gl_FragCoord.y-center.y - (radius * sin(angle1+ speed* 0.1));
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( color2, 1.0 );
		xDis = gl_FragCoord.x-center.x + (radius * cos(angle2 + speed* 0.1));
		yDis = gl_FragCoord.y-center.y + (radius * sin(angle2 + speed* 0.1));
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( color2, 1.0 );
		xDis = gl_FragCoord.x-center.x - (radius * cos(angle2 + speed* 0.1));
		yDis = gl_FragCoord.y-center.y - (radius * sin(angle2 + speed* 0.1));
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( color2, 1.0 );
		xDis = gl_FragCoord.x-center.x + (radius * cos(angle3 + speed* 0.1));
		yDis = gl_FragCoord.y-center.y + (radius * sin(angle3+ speed* 0.1));
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( color2, 1.0 );
		xDis = gl_FragCoord.x-center.x - (radius * cos(angle3 + speed* 0.1));
		yDis = gl_FragCoord.y-center.y - (radius * sin(angle3 + speed* 0.1));
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( color2, 1.0 );
		//Inside
		r = mod(0.9 + (Factor14 * 0.1) , 1.0);
		g = mod(0.3 + (Factor14 * 0.1) , 1.0);
		b = mod(0.7 + (Factor14 * 0.1) , 1.0);
		color2 = vec3(r, g, b);
		radius = Factor5;
		radius2 = (Factor5 ) - (Factor12 * 500.0);
		radius3 = (Factor5 );
		xDis = gl_FragCoord.x-center.x + (radius * cos(angle1 + speed* 0.1));
		yDis = gl_FragCoord.y-center.y + (radius * sin(angle1 + speed* 0.1));
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( color2, 1.0 );
		xDis = gl_FragCoord.x-center.x - (radius * cos(angle1 + speed* 0.1));
		yDis = gl_FragCoord.y-center.y - (radius * sin(angle1 + speed* 0.1));
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( color2, 1.0 );
		xDis = gl_FragCoord.x-center.x + (radius * cos(angle2 + speed* 0.1));
		yDis = gl_FragCoord.y-center.y + (radius * sin(angle2 + speed* 0.1));
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( color2, 1.0 );
		xDis = gl_FragCoord.x-center.x - (radius * cos(angle2 + speed* 0.1));
		yDis = gl_FragCoord.y-center.y - (radius * sin(angle2 + speed* 0.1));
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( color2, 1.0 );
		xDis = gl_FragCoord.x-center.x + (radius * cos(angle3 + speed* 0.1));
		yDis = gl_FragCoord.y-center.y + (radius * sin(angle3 + speed* 0.1));
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( color2, 1.0 );
		xDis = gl_FragCoord.x-center.x - (radius * cos(angle3 + speed* 0.1));
		yDis = gl_FragCoord.y-center.y - (radius * sin(angle3 + speed* 0.1));
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( color2, 1.0 );
	}
	// Ripples
	if(Ripples)
	{
		radius = (Factor6 * Factor6) - 600.0;
		radius2 = (Factor6 * Factor6);
		xDis = gl_FragCoord.x-center.x;
		yDis = gl_FragCoord.y-center.y;
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr >= radius && distanceSqr <= radius2)
		{
			
			gl_FragColor = vec4 ( color1, 1.0 );
		}
		radius = (Factor7 * Factor7) - 600.0;
		radius2 = (Factor7 * Factor7);
		if(distanceSqr >= radius && distanceSqr <= radius2)
		{
			gl_FragColor = vec4 ( color1, 1.0 );
		}
		radius = (Factor8 * Factor8) - (15.0 * Factor6 * spectrumAverage);
		radius2 = (Factor8 * Factor8) + (15.0 * Factor6 * spectrumAverage);
		if(distanceSqr >= radius && distanceSqr <= radius2)
		{
			gl_FragColor = vec4 ( color1, 1.0 );
		}
		radius = (Factor9 * Factor9);
		radius2 = (Factor9 * Factor9) + (50.0 * Factor9);
		if(distanceSqr >= radius && distanceSqr <= radius2)
		{
			r = mod(combinedColor2.r + (Factor14 * 0.1) , 1.0);
			g = mod(combinedColor2.g + (Factor14 * 0.1) , 1.0);
			b = mod(combinedColor2.b + (Factor14 * 0.1) , 1.0);
			color2 = vec3(r, g, b);
			gl_FragColor = vec4 ( color2, 1.0 );
		}
		radius = (Factor10 * Factor10) - (50.0 * Factor6);
		radius2 = (Factor10 * Factor10) + (50.0 * Factor6);
		if(distanceSqr >= radius && distanceSqr <= radius2)
		{
			r = mod(combinedColor.r + (Factor14 * 0.1) , 1.0);
			g = mod(combinedColor.g + (Factor14 * 0.1) , 1.0);
			b = mod(combinedColor.b + (Factor14 * 0.1) , 1.0);
			color2 = vec3(r, g, b);
			gl_FragColor = vec4 ( color2, 1.0 );
		}
	}
	
	//Orbit
	if(Orbit)
	{
		radius = (Factor5 * 1.25);
		radius2 = 0.0;
		radius3 = (Factor5 * 1.25) + (Factor1 * 300.0);
		xDis = gl_FragCoord.x-center.x + (radius * cos(60.0 - (speed2* 0.1 + (spectrumAverage * 0.1))));
		yDis = gl_FragCoord.y-center.y + (radius * sin(60.0 + (speed2* 0.3+ (spectrumAverage * 0.1))));
		distanceSqr = (xDis*xDis + yDis*yDis) * size ;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( 0.9,0.5,0.2, 1.0 );
		xDis = gl_FragCoord.x-center.x - (radius * cos(60.0 + (speed2* 0.05+ (spectrumAverage* 0.1))));
		yDis = gl_FragCoord.y-center.y - (radius * sin(60.0 - (speed2* 0.15+ (spectrumAverage* 0.1))));
		distanceSqr = (xDis*xDis + yDis*yDis) * size ;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( 0.1,0.8,0.9, 1.0 );
		xDis = gl_FragCoord.x-center.x + (radius * cos(120.0 - (speed2* 0.15+ (spectrumAverage* 0.1))));
		yDis = gl_FragCoord.y-center.y + (radius * sin(120.0 + (speed2* 0.1+ (spectrumAverage* 0.1))));
		distanceSqr = (xDis*xDis + yDis*yDis) * size ;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( 0.55,0.95,0.1, 1.0 );
		xDis = gl_FragCoord.x-center.x - (radius * cos(120.0 + (speed2* 0.1+ (spectrumAverage* 0.01))));
		yDis = gl_FragCoord.y-center.y - (radius * sin(120.0 - (speed2* 0.1+ (spectrumAverage* 0.01))));
		distanceSqr = (xDis*xDis + yDis*yDis) * size ;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( 0.2,0.65,0.95, 1.0 );
		xDis = gl_FragCoord.x-center.x + (radius * cos(180.0 - (speed2* 0.1+ (spectrumAverage* 0.01))));
		yDis = gl_FragCoord.y-center.y + (radius * sin(180.0 + (speed2* 0.2+ (spectrumAverage* 0.01))));
		distanceSqr = (xDis*xDis + yDis*yDis) * size ;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( 0.2,0.87,0.52, 1.0 );
		xDis = gl_FragCoord.x-center.x - (radius * cos(180.0 + (speed2* 0.25+ (spectrumAverage* 0.01))));
		yDis = gl_FragCoord.y-center.y - (radius * sin(180.0 - (speed2* 0.2+ (spectrumAverage* 0.01))));
		distanceSqr = (xDis*xDis + yDis*yDis) * size ;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( 0.9,0.35,0.95, 1.0 );
		xDis = gl_FragCoord.x-center.x - (radius * cos(240.0 - (speed2* 0.3+ (spectrumAverage* 0.01))));
		yDis = gl_FragCoord.y-center.y - (radius * sin(240.0 + (speed2* 0.1+ (spectrumAverage* 0.01))));
		distanceSqr = (xDis*xDis + yDis*yDis) * size ;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( 0.4,0.75,0.9, 1.0 );
		xDis = gl_FragCoord.x-center.x + (radius * cos(240.0 + (speed2* 0.1+ (spectrumAverage* 0.01))));
		yDis = gl_FragCoord.y-center.y + (radius * sin(240.0 - (speed2* 0.05+ (spectrumAverage* 0.01))));
		distanceSqr = (xDis*xDis + yDis*yDis) * size ;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( 0.92,0.55,0.22, 1.0 );
		xDis = gl_FragCoord.x-center.x + (radius * cos(300.0 - (speed2* 0.1+ (spectrumAverage* 0.01))));
		yDis = gl_FragCoord.y-center.y + (radius * sin(300.0 + (speed2* 0.15+ (spectrumAverage* 0.01))));
		distanceSqr = (xDis*xDis + yDis*yDis) * size ;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( 0.9,0.35,0.85, 1.0 );
		xDis = gl_FragCoord.x-center.x - (radius * cos(300.0 + (speed2* 0.4+ (spectrumAverage* 0.01))));
		yDis = gl_FragCoord.y-center.y - (radius * sin(300.0 - (speed2* 0.1+ (spectrumAverage* 0.01))));
		distanceSqr = (xDis*xDis + yDis*yDis) * size ;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( 0.7,0.9,0.2, 1.0 );
		xDis = gl_FragCoord.x-center.x + (radius * cos(360.0 - (speed2* 0.1+ (spectrumAverage* 0.01))));
		yDis = gl_FragCoord.y-center.y + (radius * sin(360.0 + (speed2* 0.5+ (spectrumAverage* 0.01))));
		distanceSqr = (xDis*xDis + yDis*yDis) * size ;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( 0.55,0.15,0.9, 1.0 );
		xDis = gl_FragCoord.x-center.x - (radius * cos(360.0 + (speed2* 0.2+ (spectrumAverage* 0.01))));
		yDis = gl_FragCoord.y-center.y - (radius * sin(360.0 - (speed2* 0.3+ (spectrumAverage* 0.01))));
		distanceSqr = (xDis*xDis + yDis*yDis) * size ;
		if(distanceSqr >= radius2 && distanceSqr <= radius3)
			gl_FragColor = vec4 ( 0.9,0.3,0.47, 1.0 );
	}
	//Circle
	if(Circle)
	{
		radius = (Factor5 * 2.0) + spectrumAverage * 200.0;
		xDis = gl_FragCoord.x-center.x;
		yDis = gl_FragCoord.y-center.y;
		distanceSqr = xDis*xDis + yDis*yDis;
		if(distanceSqr <= radius)
		{
			gl_FragColor = vec4 ( color1, 1.0 );
		}
	}
	
	
}