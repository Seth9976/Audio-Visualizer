precision mediump float;
varying vec4 fColor;
varying vec2 fTexCoord;

uniform sampler2D sampler2d;
uniform float Factor1;
uniform float Factor2;
uniform float Factor3;
uniform float Factor4;
uniform float Factor5;


void main()
{	
//SEIN WAEVS
	//VALUES
	float deThiccV = 0.1; // how THICC it is vertically
	float deThiccH = 10.0; // how THICC it is horizontally
	float deSpeedMultiplier = 2.0; // how FAST the sine wave travels
	float deHeight = 0.5; // how HIGH the sine wave is
	float deFattMan = 1.0; // how FATT the sine wave is
	float deIDKWhat = 1.0; // Idk what this effect is called
	
	vec3 color1 = vec3(1.0, 1.0, 0.0);
	vec3 color2 = vec3(0.0, 0.0, 1.0);
	
	//SAYA PUNYE CODE
	vec2 resolution = vec2(800.0, 600.0); // the resoltuion !remember to change resolution in solution also
	
	float xCenter = gl_FragCoord.x / resolution.x; // sin wave X pixel
	float yCenter = gl_FragCoord.y / resolution.y; // sin wave Y pixel
	            
	float curve = deThiccV * sin((deThiccH * xCenter) + (deSpeedMultiplier * Factor1)); // curvy boi
	                     
	float line = smoothstep(1.0 - clamp(distance(curve+yCenter, 0.5) * deFattMan, 0.0, 1.0), deIDKWhat, 0.99);
	     //DO NOT CHANGE THIS^                                             AND THIS^    ^idk what this does
	vec3 sineWave = (1.0 - line) * vec3(mix(color1, color2, line));
	
	//CIRCLE RADIUS
	vec2 center = vec2(400.0,300.0);
	float xDis = gl_FragCoord.x-center.x;; // center position of sine wave
	float yDis = gl_FragCoord.y-center.y;; // center position of sine wave
	float radius = 10000.0; // circle radius
	float distanceSqr = xDis*xDis + yDis*yDis; // circle mask
	                  //vec3    //transparency
	if(distanceSqr <= (Factor4*0.2)*radius)
		gl_FragColor = vec4(sineWave, 1.0);
	else
		gl_FragColor = vec4(sineWave, 0.2);
	//---------------------------------------------------------------------------------------------------------
	//SIR CHIN FOO PUNYE CODE
	// float amp = 60.0;
	// float windowHalfHeight = 300.0;
	// float lightenedPointY = cos(gl_FragCoord.x*0.02 + Factor1) * amp + windowHalfHeight;
	
	// vec2 lightenedPoint = vec2(gl_FragCoord.x, lightenedPointY);
	// vec2 currentPixelPos = vec2(gl_FragCoord.x, gl_FragCoord.y);
	
	// float dist = abs(currentPixelPos.y - lightenedPointY);
	// float thickness = 20.0;
	
	// if(dist <= thickness)
		// gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	// else
		// gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0); 
	
	
	
	//circle
	
	// vec2 center = vec2(400.0,300.0);
		
	// float xDis= gl_FragCoord.x-center.x;
	// float yDis = gl_FragCoord.y-center.y;
	// float radius = 200.0;
	// float distanceSqr = xDis*xDis + yDis*yDis;
	// if(distanceSqr <= Factor4*radius)
	// {
		// resultColor.r = mod(combinedColor.r+Factor1, 1.0);
		// resultColor.g = mod(combinedColor.g+Factor1, 1.0);
		// resultColor.b = mod(combinedColor.b+Factor1, 1.0);
	// }
	// else
	// {
		// resultColor.r = greyscale;
		// resultColor.g = greyscale;
		// resultColor.b = greyscale;
	// }
	
	
	/*
	
	// use texture color only
	
	vec4 texColor = texture2D(sampler2d, fTexCoord);
	vec4 combinedColor;
	combinedColor = fColor * texColor;
	//combinedColor = fColor * texColor / (fColor * fColor);
	//combinedColor = fColor * texColor / (texColor * texColor) / (fColor * fColor);
	
	vec4 resultColor;
	//resultColor.r = mod(combinedColor.r+Factor1, 1.0);
	//resultColor.g = mod(combinedColor.g+Factor1, 1.0);
	//resultColor.b = mod(combinedColor.b+Factor1, 1.0);
	resultColor.r = combinedColor.r;
	resultColor.g = combinedColor.g;
	resultColor.b = combinedColor.b;
	
	float greyscale = ((resultColor.r*0.29 + resultColor.g*0.59 + resultColor.b*0.11));
	float greyscale2 = (resultColor.r + resultColor.g + resultColor.b) / 3.0; 
	float newR, newG, newB;
	newR = (resultColor.r*0.39 + resultColor.g*0.77 + resultColor.b*0.19);
	newG = (resultColor.r*0.35 + resultColor.g*0.69 + resultColor.b*0.17);
	newB = (resultColor.r*0.27 + resultColor.g*0.53 + resultColor.b*0.13);
	resultColor.r = newR;
	resultColor.g = newG;
	resultColor.b = newB;
	
	resultColor.a = combinedColor.a;
	
	//gl_FragColor = resultColor;

	//gl_FragColor = vec4 ( r, g, b, 1.0 ); */
}