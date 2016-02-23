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
#include "2d-graph.h"
#include "geo-2d.h"
#include "..\utilities\type.h"
#include "..\shader\shader.h"

static PFNGLACTIVETEXTUREPROC					glActiveTexture;


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

	case glzBlendingMode::MULTIPLY:
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		break;
		


	}
	
	glEnable(GL_BLEND);
}



//*** obj2d_Sprite ***



void obj2d_Sprite::draw(glzCamera2D *camera)
{

	glzMatrix m;
	glzMatrix mt;
	setblendingmode(blend);
	glzShaderUseBasic();

	m.LoadIdentity();
	mt.LoadIdentity();
	
	m *= camera->m;
	if (n_parent != nullptr)
		m *= n_parent->m;
	m *= n_local.m;

	unsigned int basic_program = glzShaderReurnBasic();

	glzUniformMatrix4fv(basic_program, "projMat", mt);
	glzUniform1i(basic_program, "texunit0", 0);	
	glzUniform4f(basic_program, "color", blendcolor.r, blendcolor.g, blendcolor.b, blendcolor.a);

	glzDirectSpriteRender(m, texture->handle, sprite.get_sprite(current_animation, current_frame), width*scale, height*scale, origin);
	glDisable(GL_BLEND);
	return;
}


void obj2d_Sprite::update(float seconds)
{

	if ((framespeed == 0.0) || (animationstate == glzOBject2DAnimationstate::STOPPED)) return;


	int maxframes = sprite.map.at(current_animation).map.size();

	
	frametime += seconds*framespeed;


	while (frametime >= 1.0f) 
	{ 
		current_frame++; frametime -= 1.0f;


		if (current_frame >= maxframes)
		{
						
			if (animationstate == glzOBject2DAnimationstate::PLAYINGONCE)
			{
				current_frame = maxframes - 1;	
				animationstate == glzOBject2DAnimationstate::STOPPED;
			}


			else if(animationstate == glzOBject2DAnimationstate::PLAYING)
			{
				current_frame -= maxframes;
			}
			
		}	
	
	}

	//while (current_frame >= maxframes) current_frame -= maxframes;

	return;
}


void obj2d_Sprite::set_i(glzOBject2DSetvar type, int v)
{
	switch (type)
	{
	//case glzOBject2DSetvar::TEXTURE:
//		texture = v;
	//	break;
	
	}
	return;
}

void obj2d_Sprite::set_f(glzOBject2DSetvar type, float v)
{
	switch (type)
	{

//	case glzOBject2DSetvar::FRAMESPEED:
//		framespeed = v;
//		break;
	}
	return;
}



//*** obj2d_Fullscreen ***

void obj2d_Fullscreen::draw(glzCamera2D *camera)
{

	glzMatrix m;
	glzMatrix mt;

	setblendingmode(blend);
	glzShaderUseBasic();


	m.LoadIdentity();
	mt.LoadIdentity();
//	m *= camera->m;

//	if (n_parent != nullptr)
//		m *= n_parent->m;
//	m *= n_local.m;



	mt.translate(-0.0, -0.0, 0.0);

	mt.scale(2.0, 2.0, 1.0);
	unsigned int basic_program = glzShaderReurnBasic();

	glzUniformMatrix4fv(basic_program, "projMat", mt);
	glzUniform1i(basic_program, "texunit0", 0);
	glzUniform4f(basic_program, "color", blendcolor.r, blendcolor.g, blendcolor.b, blendcolor.a);
	glzDirectSpriteRender(m, texture->handle, sprite, glzOrigin::CENTERED);

	glDisable(GL_BLEND);
	return;
}

void obj2d_Fullscreen::update(float seconds)
{
	return;
}

void obj2d_Fullscreen::set_i(glzOBject2DSetvar type, int v)
{
	switch (type)
	{
	//case glzOBject2DSetvar::TEXTURE:
		//texture->handle = v;
		//break;
	}
	return;
}

void obj2d_Fullscreen::set_f(glzOBject2DSetvar type, float v)
{
	switch (type)
	{

		//	case glzOBject2DSetvar::SCALE:
		//		scale = v;
		//		break;
	}
	return;
}


//*** obj2d_Background ***

void obj2d_Background::draw(glzCamera2D *camera)
{

	glzMatrix m;
	glzMatrix mt;

	setblendingmode(blend);
	glzShaderUseTiledSprite();


	m.LoadIdentity();
	mt.LoadIdentity();

	vert3 offset;

	offset += n_local.pos;
	offset.project(camera->m);

	mt.translate(0.0, 0.0, 0.0);

	mt.scale(2.0, 2.0, 1.0);
	unsigned int tile_program = glzShaderReurnTiledSprite();


	glzSprite tsprite = sprite.get_sprite(current_animation, current_frame);



	glzUniformMatrix4fv(tile_program, "projMat", mt);
	glzUniform1i(tile_program, "texunit0", 0);
	glzUniform4f(tile_program, "color", blendcolor.r, blendcolor.g, blendcolor.b, blendcolor.a);

	glzUniform2f(tile_program, "spritepos", tsprite.a.u, tsprite.a.v);
	glzUniform2f(tile_program, "spritesize", tsprite.d.u - tsprite.a.u, tsprite.d.v - tsprite.a.v);
	glzUniform2f(tile_program, "spriteoffset", -offset.x*0.5*paralax, -offset.y*0.5*paralax);
	glzUniform2f(tile_program, "spritescale", (camera->width / scale) / camera->zoom, (camera->height / scale) / camera->zoom);


	glzDirectSpriteRender(m, texture->handle, glzSprite(), glzOrigin::CENTERED);

	glDisable(GL_BLEND);
	return;
}

void obj2d_Background::update(float seconds)
{

	if ((framespeed == 0.0) || (animationstate == glzOBject2DAnimationstate::STOPPED)) return;

	int maxframes = sprite.map.at(current_animation).map.size();

	frametime += seconds*framespeed;

	while (frametime >= 1.0f)
	{
		current_frame++; frametime -= 1.0f;


		if (current_frame >= maxframes)
		{

			if (animationstate == glzOBject2DAnimationstate::PLAYINGONCE)
			{
				current_frame = maxframes - 1;
				animationstate == glzOBject2DAnimationstate::STOPPED;
			}


			else if (animationstate == glzOBject2DAnimationstate::PLAYING)
			{
				current_frame -= maxframes;
			}

		}

	}

	return;

}

void obj2d_Background::set_i(glzOBject2DSetvar type, int v)
{
	switch (type)
	{
//	case glzOBject2DSetvar::TEXTURE:
//		texture = v;
//		break;
	
	}
	return;
}

void obj2d_Background::set_f(glzOBject2DSetvar type, float v)
{
	switch (type)
	{

//	case glzOBject2DSetvar::FRAMESPEED:
//		framespeed = v;
//		break;
	}
	return;
}


//*** obj2d_Clear ***

void obj2d_Clear::draw(glzCamera2D *camera)
{
	glClearColor(blendcolor.r, blendcolor.g, blendcolor.b, blendcolor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	return;
}

void obj2d_Clear::update(float seconds)
{
	return;
}

//*** obj2d_Object2DGraph ***

 void obj2d_Object2DGraph::draw(glzCamera2D *camera)
{	
		 if (rendergraph!=nullptr)
				rendergraph->draw();
	return;
}

 void obj2d_Object2DGraph::update(float seconds)
{
	return;
}



 void obj2d_Object2DGraph::set_r(glzOBject2DSetvar type, Object2DGraph *v)
 {
		 switch (type)
		 {

		 case glzOBject2DSetvar::RENDEREGRAPH:
			 rendergraph = v;
			 		break;
		 }
		 return;
 }




//*** obj2d_Tiles ***

void obj2d_Tiles::draw(glzCamera2D *camera)
{

	glzMatrix m;
	glzMatrix mt;
	setblendingmode(blend);

	glzShaderUseTilemap();

	m.LoadIdentity();
	mt.LoadIdentity();

	m *= camera->m;

	if (n_parent != nullptr)
		m *= n_parent->m;
	m *= n_local.m;

	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

	unsigned int atlas_program = glzShaderReurnTilemap();

	glzUniformMatrix4fv(atlas_program, "projMat", mt);
	glzUniform1i(atlas_program, "texunit0", 0);
	glzUniform1i(atlas_program, "texunit1", 1);
	glzUniform4f(atlas_program, "color", blendcolor.r, blendcolor.g, blendcolor.b, blendcolor.a);

	glzUniform1i(atlas_program, "layer", layer);
	glzUniform1i(atlas_program, "anim", current_frame);
	glzUniform1i(atlas_program, "width", map->width);
	glzUniform1i(atlas_program, "height", map->height);
	glzUniform1i(atlas_program, "a_width", tileWidth);
	glzUniform1i(atlas_program, "a_height", tileHeight);
	glzUniform1i(atlas_program, "extr", 0);



	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture->handle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, map->tex);
	

	glzDirectSpriteRender(m, map->tex, sprite, width*scale, height*scale, origin);


	glDisable(GL_BLEND);
	return;
}

void obj2d_Tiles::update(float seconds)
{

	if ((framespeed == 0.0) || (animationstate == glzOBject2DAnimationstate::STOPPED)) return;


	const int maxframes = 4;


	frametime += seconds*framespeed;


	while (frametime >= 1.0f)
	{
		current_frame++; frametime -= 1.0f;


		if (current_frame >= maxframes)
		{

			if (animationstate == glzOBject2DAnimationstate::PLAYINGONCE)
			{
				current_frame = maxframes - 1;
				animationstate == glzOBject2DAnimationstate::STOPPED;
			}


			else if (animationstate == glzOBject2DAnimationstate::PLAYING)
			{
				current_frame -= maxframes;
			}

		}

	}
	
	return;
}

void obj2d_Tiles::set_i(glzOBject2DSetvar type, int v)
{
	switch (type)
	{
	//case glzOBject2DSetvar::TEXTURE:
		//texture = v;
		//break;
	}
	return;
}

void obj2d_Tiles::set_f(glzOBject2DSetvar type, float v)
{
	switch (type)
	{

//	case glzOBject2DSetvar::SCALE:
//		scale = v;
//		break;
	}
	return;
}

void obj2d_Tiles::set_sp(glzOBject2DSetvar type, glzSprite v)
{
	switch (type)
	{

			case glzOBject2DSetvar::SPRITE:
				sprite = v;
				break;
	}
	return;
}





//*** obj2d_Text ***

void obj2d_Text::draw(glzCamera2D *camera)
{

	glzMatrix m;
	glzMatrix mt;
	setblendingmode(blend);

	glzShaderUseBasic();

	m.LoadIdentity();
	mt.LoadIdentity();

	m *= camera->m;

	if (n_parent != nullptr)
		m *= n_parent->m;
	m *= n_local.m;
	//m.scale(scale, scale, scale);

	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

	unsigned int basic_program = glzShaderReurnBasic();

	glzUniformMatrix4fv(basic_program, "projMat", m);
	glzUniform1i(basic_program, "texunit0", 0);
	glzUniform4f(basic_program, "color", blendcolor.r, blendcolor.g, blendcolor.b, blendcolor.a);
	glBindTexture(GL_TEXTURE_2D, texture->handle);


	glzDirectDrawText(text, scale, aspect, kern, origin);

	glDisable(GL_BLEND);
	return;
}

void obj2d_Text::update(float seconds)
{

	

	return;
}

void obj2d_Text::set_s(glzOBject2DSetvar type, string v)
{
	switch (type)
	{
	case glzOBject2DSetvar::TEXT:
		text = v;
		break;
	}
	return;
}


//*** obj2d_ColorTint ***

void obj2d_ColorTint::draw(glzCamera2D *camera)
{

	glzMatrix m;
	glzMatrix mt;

	setblendingmode(blend);
	glzShaderUseColorTint();


	m.LoadIdentity();
	mt.LoadIdentity();

	mt.translate(-1.0, -1.0, 0.0);

	mt.scale(2.0, 2.0, 1.0);
	unsigned int colortint_program = glzShaderReurnColorTint();

	glzUniformMatrix4fv(colortint_program, "projMat", mt);
	glzUniform4f(colortint_program, "color", blendcolor.r, blendcolor.g, blendcolor.b, blendcolor.a);
	glzDirectSpriteRender(m, texture->handle, glzSprite(), glzOrigin::CENTERED);

	glDisable(GL_BLEND);
	return;
}

void obj2d_ColorTint::update(float seconds)
{
	return;
}


