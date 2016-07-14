#version 120


varying vec4 color;
uniform sampler2D texunit0;


void main() {
        
vec2 separation = vec2(-0.001,0.001);
vec4 base = texture2D(texunit0, color.xy);
vec4 base_highlight = texture2D(texunit0, color.xy-separation);
vec4 base_shade = texture2D(texunit0, color.xy+separation);

vec4 txtcolor_shade = vec4(0.0,0.2,0.4,1.0)*base_shade.a;
vec4 txtcolor_highlight=(txtcolor_shade*(1.0-base_highlight.a)) +(vec4(1.0,1.0,1.0,1.0)*base_highlight.a);
vec4 txtcolor=txtcolor_highlight*(1.0-base.a)+vec4(0.4,0.6,1.0,1.0)*base.a;


//fragment_color=base;
gl_FragColor=txtcolor;
//fragment_color=vec4(1.0,0.0,0.0,1.0);

}             