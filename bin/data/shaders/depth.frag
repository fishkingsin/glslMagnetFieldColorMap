//Graciously provided by
// http://mouaif.wordpress.com/2009/01/05/photoshop-math-with-glsl-shaders/
const float _NoiseFreq = 4.0;
const float _NoiseAmp = -0.5;
const vec3 _NoiseAnim = vec3(0, -1, 0);
// For all settings: 1.0 = 100% 0.5=50% 1.5 = 150%
vec3 ContrastSaturationBrightness(vec3 color, float brt, float sat, float con)
{
	// Increase or decrease theese values to adjust r, g and b color channels seperately
	const float AvgLumR = 0.5;
	const float AvgLumG = 0.5;
	const float AvgLumB = 0.5;
	
	const vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);
	
	vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
	vec3 brtColor = color * brt;
	vec3 intensity = vec3(dot(brtColor, LumCoeff));
	vec3 satColor = mix(intensity, brtColor, sat);
	vec3 conColor = mix(AvgLumin, satColor, con);
	return conColor;
}
// color gradient
// this should be in a 1D texture really
vec4 gradient(float x)
{
	const vec4 c0 = vec4(4, 4, 4, 1);	// hot white
	const vec4 c1 = vec4(1, 1, 0, 1);	// yellow
	const vec4 c2 = vec4(1, 0, 0, 1);	// red
	const vec4 c3 = vec4(0.4, 0.4, 0.4, 4);	// grey
	
	float t = fract(x*3.0);
	vec4 c;
	if (x < 0.3333) {
		c =  mix(c0, c1, t);
	} else if (x < 0.6666) {
		c = mix(c1, c2, t);
	} else {
		c = mix(c2, c3, t);
	}
	//return vec4(x);
	//return vec4(t);
	return c;
}
// matrix to rotate the noise octaves
mat3 m = mat3( 0.00,  0.80,  0.60,
              -0.80,  0.36, -0.48,
              -0.60, -0.48,  0.64 );
float hash( float n )
{
    return fract(sin(n)*43758.5453);
}
float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    
    f = f*f*(3.0-2.0*f);
    
    float n = p.x + p.y*57.0 + 113.0*p.z;
    
    float res = mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                        mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
                    mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                        mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
    return res;
}
float fbm( vec3 p )
{
    float f;
    f = 0.5000*noise( p ); p = m*p*2.02;
    f += 0.2500*noise( p ); p = m*p*2.03;
    f += 0.1250*noise( p ); p = m*p*2.01;
    f += 0.0625*noise( p );
    p = m*p*2.02; f += 0.03125*abs(noise( p ));
    return f/0.9375;
}
uniform sampler2DRect tex;
uniform float brightness;
uniform float contrast;
uniform float saturation;
uniform float alpha;
uniform int invert;
uniform vec3 c1;
uniform vec3 c2;
uniform vec3 c3;
uniform vec3 c4;
uniform vec3 c5;
uniform vec3 c6;
uniform vec3 c7;
uniform vec3 c8;
uniform vec3 c9;
float oriBrightness;

varying vec2  TexCoord;
varying vec4 newVertexPos;

uniform int maxHeight;
uniform int mode;
uniform float iGlobalTime;
float displace ;
void main (void)
{
	//use the color from the image, but use the r channel of the mask as the alpha channel of our output
    vec4 mTex = texture2DRect(tex,gl_TexCoord[0].st);
	vec3 color = ContrastSaturationBrightness(texture2DRect(tex,gl_TexCoord[0].st).rgb, brightness, saturation, contrast);
	if(invert == 1) color = vec3(1.0) - color;
	vec3 oriColor = texture2DRect(tex,gl_TexCoord[0].st).rgb;
	oriBrightness  = (oriColor.r+oriColor.g+oriColor.b)/3.0;
	//gl_FragData[0] = vec4(color, alpha)*newVertexPos;  
	float grayscale = (color.r+color.g+color.b)/3.0;//(newVertexPos.r+newVertexPos.g+newVertexPos.b)/3.0;
	float halfTone = .0;//grayscale ;
	float eachHeight = float(maxHeight/8);
	float y = 0.0;
	int b = 1;
	if(mode==0)
	{
        grayscale =grayscale+sin(iGlobalTime)+0.5;
        if(grayscale < 0.0) grayscale = 1.0-grayscale;
        else if(grayscale > 1.0)
            grayscale = grayscale-sin(iGlobalTime);
		if(grayscale<0.125) halfTone = 0.0;
		else if(grayscale>0.125 && grayscale<0.25) halfTone = 1.0;
		else if(grayscale>0.25 && grayscale<0.375) halfTone = 0.0;
		else if(grayscale>0.375 && grayscale<0.5) halfTone = 1.0;
		else if(grayscale>0.5 && grayscale<0.625) halfTone = 0.0;
		else if(grayscale>0.625 && grayscale<0.75) halfTone = 1.0;
		else if(grayscale>0.75 && grayscale<0.875) halfTone = 0.0;
		else if(grayscale>0.875 && grayscale < 1.0) halfTone = 1.0;
		else halfTone = 0.0;
//		for(int i = 0 ; i < 8 ; i+=1)
//		{
//			y+=1.0;
//			
//			if((eachHeight*y)<newVertexPos.z && (eachHeight*(y+1.0))>newVertexPos.z) 
//			{
//				if(mod(y,2.0) ==0.0)
//				{
//					halfTone = 1.0;
//				}
//				else
//				{
//					halfTone = 0.0;
//					
//				}
//			}
//
//		}
		gl_FragData[0] = vec4(halfTone,halfTone,halfTone, alpha);
		
	}
	else if (mode==1)
	{
		if(grayscale<0.125) halfTone = 0.0;
		else if(grayscale>0.125 && grayscale<0.25) halfTone = 0.125;
		else if(grayscale>0.25 && grayscale<0.375) halfTone = 0.25;
		else if(grayscale>0.375 && grayscale<0.5) halfTone = 0.375;
		else if(grayscale>0.5 && grayscale<0.625) halfTone = 0.5;
		else if(grayscale>0.625 && grayscale<0.75) halfTone = 0.625;
		else if(grayscale>0.75 && grayscale<0.875) halfTone = 0.75;
		else if(grayscale>0.875 && grayscale < 1.0) halfTone = 0.875;
		else halfTone = 1.0;
		gl_FragData[0] = vec4(halfTone,halfTone,halfTone, alpha);
	}
	else if (mode==2)
	{
		vec3 color = c1;
		if(grayscale<0.125) color = c1;
		else if(grayscale>0.125 && grayscale<0.25) color = c2;
		else if(grayscale>0.25 && grayscale<0.375) color = c3;
		else if(grayscale>0.375 && grayscale<0.5) color = c4;
		else if(grayscale>0.5 && grayscale<0.625) color = c5;
		else if(grayscale>0.625 && grayscale<0.75) color = c6;
		else if(grayscale>0.75 && grayscale<0.875) color = c7;
		else if(grayscale>0.875 && grayscale < 1.0) color = c8;
		else color = c9;
		gl_FragData[0] = vec4(color, alpha);
	}
    else if (mode==3)
	{
        displace = fbm(mTex.rgb*_NoiseFreq + _NoiseAnim*iGlobalTime);
		vec4 c = gradient(grayscale+displace);
		gl_FragData[0] = vec4(c.rgb, alpha);
	}
	
}
