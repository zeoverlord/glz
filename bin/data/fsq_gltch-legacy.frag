#version 120

varying vec4 texco;
uniform sampler2D texunit0;
uniform sampler2D texunit1;
uniform vec4 tint;
uniform int width;
uniform int height;

void main() {
   
vec4 glitchbase = texture2D(texunit1, texco.xy); 

float glitchlevel=1.0;

glitchlevel=floor(glitchbase.x*18.0+1.0);

float glitchscalex=width/glitchlevel;
float glitchscaley=height/glitchlevel;

vec2 tc;

tc.x=floor(texco.x*glitchscalex);
tc.y=floor(texco.y*glitchscaley);

tc.x=(tc.x+0.5)/glitchscalex; 
tc.y=(tc.y+0.5)/glitchscaley; 

vec4 base = texture2D(texunit0, tc.xy);    

gl_FragColor=base*tint;     


}             