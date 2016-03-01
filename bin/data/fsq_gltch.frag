#version 140


in vec4 texco;
out vec4 fragment_color;
uniform sampler2D texunit0;
uniform sampler2D texunit1;
uniform vec4 tint;
uniform int width;
uniform int height;




void main() {
   


vec4 glitchbase = texture2D(texunit1, texco.xy); 



float glitchlevel=1.0;

modf(glitchbase.x*18.0+1.0,glitchlevel);


float glitchscalex=width/glitchlevel;
float glitchscaley=height/glitchlevel;

vec2 tc;

modf(texco.x*glitchscalex,tc.x);
modf(texco.y*glitchscaley,tc.y);

tc.x=(tc.x+0.5)/glitchscalex; 
tc.y=(tc.y+0.5)/glitchscaley; 

vec4 base = texture2D(texunit0, tc.xy);    

fragment_color=base*tint;     


}             