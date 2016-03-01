// Copyright 2016 Peter Wallström
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.

// while it is not required i like to request a few things
// 1. please do share any meaningfull/usefull changes/additions/fixes you make with me so that i could include it in any future version
// 2. likewise do share any ideas for improvements
// 3. If you make something comersiol or at least something you release publicly that relies on this code then i would like to know and maybe use in my CV
// 4. Please do include me in your credits

// glz 2d utilities - i think it's usefull
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>
#include "2d-graph.h"
#include "geo-2d.h"
#include "2d-utilities.h"


// backdrops are usefull, if you don't have the time to write a 2D renderer then just cheat with a texture backdrop instead



void glzBackdrop(texturecontainer *texture, glzBlendingMode blend)
{
	glzColor blendcolor(1.0, 1.0, 1.0, 1.0);

	glzBackdrop(texture, blend, blendcolor);
	return;
}

void glzBackdrop(texturecontainer *texture, glzBlendingMode blend, glzColor color)
{

	glzMatrix mt;
	mt.LoadIdentity();
	mt.translate(0.0f, 0.0f, 0.0f);
	mt.scale(2.0f, 2.0f, 1.0f);

	glzBackdrop(texture, blend, mt, color);
	return;
}



void glzBackdrop(texturecontainer *texture, glzBlendingMode blend, glzMatrix mat, glzColor color)
{
	glzSprite sprite;
	glzColor blendcolor(1.0f, 1.0f, 1.0f, 1.0f);
	glzMatrix m;

	setblendingmode(blend);
	glzShaderUseBasic();

	m.LoadIdentity();
	unsigned int basic_program = glzShaderReurnBasic();
	glDisable(GL_DEPTH_TEST);

	glzUniformMatrix4fv(basic_program, "projMat", mat);
	glzUniform1i(basic_program, "texunit0", 0);
	glzUniform4f(basic_program, "color", blendcolor.r, blendcolor.g, blendcolor.b, blendcolor.a);
	glzDirectSpriteRender(m, texture->handle, sprite, glzOrigin::CENTERED);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	return;
}

void glzDrawSprite(texturecontainer *texture, glzBlendingMode blend, float x, float y, float scale, float aspect)
{
	glzSprite sprite;
	glzMatrix m;
	glzMatrix mt;
	glzColor blendcolor(1.0f, 1.0f, 1.0f, 1.0f);

	setblendingmode(blend);
	glzShaderUseBasic();

	m.LoadIdentity();

	mt.LoadIdentity();
	mt.translate(x,y,1.0f);
	//mt.scale(scale/aspect, scale, 1.0f);

	unsigned int basic_program = glzShaderReurnBasic();
	glDisable(GL_DEPTH_TEST);
	glzUniformMatrix4fv(basic_program, "projMat", mt);
	glzUniform1i(basic_program, "texunit0", 0);
	glzUniform4f(basic_program, "color", blendcolor);
	glzDirectSpriteRender(m, texture->handle, sprite, scale/aspect, scale, glzOrigin::CENTERED);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	return;
}


void glzDrawText(string text, float x, float y, float scale, float kern, float aspect, texturecontainer *font, glzColor color)
{
	glzShaderUseBasic();
	glzMatrix m;
	m.LoadIdentity();
	m.ortho(-4, 4, -2, 2, -100, 100);
	m.translate(x, y, 0);

	unsigned int basic_program = glzShaderReurnBasic();

	glzUniformMatrix4fv(basic_program, "projMat", m);
	glzUniform1i(basic_program, "texunit0", 0);
	glzUniform4f(basic_program, "color", color);

	glBindTexture(GL_TEXTURE_2D, font->handle);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glzDirectDrawText(text, scale, aspect, kern, glzOrigin::BOTTOM_LEFT);
	glDisable(GL_BLEND);

}