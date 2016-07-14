#version 120

attribute vec3 Position;
attribute vec2 TexCoord;
attribute vec3 Normal;
uniform  mat4 projMat;
varying vec4 txcoord;


void main() {

	vec4 pos=vec4(Position.xyz,1.0);
	gl_Position=projMat*pos;
	txcoord=vec4(TexCoord.xy,0.0,1.0);

}  