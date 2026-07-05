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
	
	//circle
	
	vec2 center = vec2(400.0,300.0);
		
	float xDis= gl_FragCoord.x-center.x;
	float yDis = gl_FragCoord.y-center.y;
	float radius = 200.0;
	float distanceSqr = xDis*xDis + yDis*yDis;
	if(distanceSqr <= Factor4*radius)
	{
		resultColor.r = mod(combinedColor.r+Factor1, 1.0);
		resultColor.g = mod(combinedColor.g+Factor1, 1.0);
		resultColor.b = mod(combinedColor.b+Factor1, 1.0);
	}
	else
	{
		
		resultColor.r = greyscale;
		resultColor.g = greyscale;
		resultColor.b = greyscale;
	}
	
	//gl_FragColor = resultColor;

	//gl_FragColor = vec4 ( r, g, b, 1.0 ); 
}