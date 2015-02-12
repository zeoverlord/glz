// Copyright 2014 Peter Wallström
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

// glz 2d scenegraph base class - i think it's usefull
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git


#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>
#include "ztool-2d-graph.h"
#include "ztool-geo-2d.h"
#include "ztool-type.h"


void setblendingmode(glzBlendingMode bmode)
{

	switch (bmode)
	{
	case glzBlendingMode::NONE:
		return;
		break;

	case glzBlendingMode::ADDITIVE:
		glBlendFunc(GL_ONE, GL_ONE);
		break;

	case glzBlendingMode::ALPHA:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	}
	
	glEnable(GL_BLEND);
}



//*** obj2d_Sprite ***

void obj2d_Sprite::draw(glzCamera2D *camera)
{

	glzMatrix m;
	setblendingmode(blend);

	m.LoadIdentity();
	m *= camera->m;
	if (n_parent != nullptr)
		m *= n_parent->m;
	m *= n_local.m;
	m.scale(scale, scale, scale);

	glzDirectSpriteRender(m, texture, sprite, glzOrigin::CENTERED);

	glDisable(GL_BLEND);
	return;
}

void obj2d_Sprite::update(float seconds)
{
	return;
}

void obj2d_Sprite::set_i(glzOBject2DSetvar type, int v)
{
	switch (type)
	{
	case glzOBject2DSetvar::TEXTURE:
		texture = v;
		break;
	}
	return;
}

void obj2d_Sprite::set_f(glzOBject2DSetvar type, float v)
{
	switch (type)
	{

	case glzOBject2DSetvar::SCALE:
		scale = v;
		break;
	}
	return;
}




//*** obj2d_Sprite_Animated ***



void obj2d_Sprite_Animated::draw(glzCamera2D *camera)
{

	glzMatrix m;
	setblendingmode(blend);

	m.LoadIdentity();
	
	m *= camera->m;
	if (n_parent != nullptr)
		m *= n_parent->m;
	m *= n_local.m;
	m.scale(scale, scale, scale);

	glzDirectSpriteRender(m, texture, sprite.get_sprite(current_animation, current_frame), glzOrigin::CENTERED);
	glDisable(GL_BLEND);
	return;
}

void obj2d_Sprite_Animated::update(float seconds)
{

	int maxframes = sprite.map.at(current_animation).map.size();
	
	frametime += seconds;


	while (frametime >= framespeed) { current_frame++; frametime -= framespeed; }

	while (current_frame >= maxframes) current_frame -= maxframes;

	return;
}


void obj2d_Sprite_Animated::set_i(glzOBject2DSetvar type, int v)
{
	switch (type)
	{
	case glzOBject2DSetvar::TEXTURE:
		texture = v;
		break;
	case glzOBject2DSetvar::CURRENT_ANIMATION:
		current_animation = v;
		break;
	case glzOBject2DSetvar::CURRENT_FRAME:
		current_frame = v;
		break;
	}
	return;
}

void obj2d_Sprite_Animated::set_f(glzOBject2DSetvar type, float v)
{
	switch (type)
	{
	case glzOBject2DSetvar::SCALE:
		scale = v;
		break;
	case glzOBject2DSetvar::FRAMESPEED:
		framespeed = v;
		break;
	}
	return;
}

