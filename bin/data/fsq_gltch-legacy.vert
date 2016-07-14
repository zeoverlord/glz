#version 120


attribute vec3 Position;
attribute vec2 TexCoord;
attribute vec3 Normal;
varying vec4 texco;
uniform  mat4 projMat;

void main() {

	vec4 pos=vec4(Position.xyz,1.0);
	gl_Position=projMat*pos;
	texco=vec4(TexCoord.xy,0.0,1.0);

}   