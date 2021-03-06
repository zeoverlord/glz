// Copyright 2013 Peter Wallstr�m
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

// glz 2D example app - i wouldn't say it produces any meningfull results, but it's a start.
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git


#include "z-tile.h"
#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>
#include <fstream>
#include <math.h>
#include "..\..\glz-core\app\appbase.h"
#include "..\..\glz-core\3d\geo.h"
#include "..\..\glz-core\shader\shader.h"
#include "..\..\glz-core\utilities\glz.h"
#include "..\..\glz-core\utilities\vectormath.h"
#include "..\..\glz-core\image\tex.h"
#include "..\..\glz-core\2d\geo-2d.h"
#include "..\..\glz-core\3d\geo-generate.h"
#include "..\..\glz-core\utilities\tiletools.h"
#include "..\..\glz-core\utilities\resourcemanager.h"
#include "..\..\glz-core\utilities\config.h"
#include "..\..\glz-core\input\input.h"

using namespace std;
using namespace GLZ;

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors







static PFNGLUSEPROGRAMPROC						glUseProgram;
static PFNGLUNIFORM1IPROC                       glUniform1i;
static PFNGLUNIFORMMATRIX4FVPROC                glUniformMatrix4fv;
static PFNGLUNIFORM4FARBPROC                    glUniform4f;
static PFNGLGETUNIFORMLOCATIONPROC              glGetUniformLocation;
static PFNGLBLENDCOLORPROC						glBlendColor;
static PFNGLACTIVETEXTUREPROC					glActiveTexture;



ZtileState::ZtileState()
{
	angle = 0;
	texttimer = 0;
	spriteframetimer = 0;
	spriteframe = 0;

	gamestate = 1;
	testanim = 0;
	testanimTimer = 0;
	keyTimer = 0;

	tiles_width = 16;
	tiles_height = 16;
	paintarea_x = 0.0f;
	paintarea_y = 0.0f;
	paintarea_Zoom = 512;
	paintarea_pixel_x = 0.0f;
	paintarea_pixel_y = 0.0f;

	paintarea_x_s = 0.0f;
	paintarea_y_s = 0.0f;
	paintarea_Zoom_s = 512;
	paintarea_pixel_x_s = 0.0f;
	paintarea_pixel_y_s = 0.0f;
	
	z_tileUI_point = ztUIP::BACKGROUND;


	cursprite_x = 0;
	cursprite_y = 0;
	cursprite_anim = false;
	cursprite_extra = false;

	dual_view = false;
	toggle_extra = false;




	curlayer = ztLeveltex::L1A;
	has_l1 = true;
	has_l2 = true;
	has_d = true;

	leveltex_1_filename = "data\\supertiles1.tga";
	leveltex_2_filename = "data\\supertiles2.tga";
	leveltex_d_filename = "data\\supertilesd.tga";
	//char leveltex_filename[255] = "data\\supertilesaspect.tga";
	//char leveltex_filename[255] = "data\\a-map.tga";

}



bool ZtileState::Initialize(int width, int height)					// Any GL Init Code & User Initialiazation Goes Here
{
	glzAppinitialization app;
	glzResourcemanager rm;

	GetFocus();
	GetAsyncKeyState(WM_KEYUP);

	// Start Of User Initialization
	angle = 0.0f;											// Set Starting Angle To Zero

	glClearColor(0.06f, 0.06f, 0.06f, 1.0f);						// Black Background
	glClearDepth(1.0f);										// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel(GL_SMOOTH);									// Select Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glFrontFace(GL_CCW);
	//ShowCursor(false);



	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform4f = (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");

	glBlendColor = (PFNGLBLENDCOLORPROC)wglGetProcAddress("glBlendColor");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");






	glzMatrix mt;
	mt.LoadIdentity();
	mt.scale(0.17f, 0.17f, 0.17f);
	if (app.data.legacyMode)
	{
		ProgramObject = glzShaderLoad("data\\glsl-legacy.vert", "data\\glsl-legacy.frag", glzVAOType::AUTO);
		}
	else
	{

		ProgramObject = glzShaderLoad("data\\glsl.vert", "data\\glsl.frag", glzVAOType::AUTO);
	}
	glzShaderLink(ProgramObject);
	rm.createTexture("font.arial", "data\\fonts\\arial.tga", glzTexFilter::LINEAR, 2);

	// load data

	rm.createTexture("spritemap.sm0", "data\\tileset.tga", glzTexFilter::NEAREST);
	rm.createTexture("spritemap.sm1", "data\\colisiontile.tga", glzTexFilter::NEAREST);
	rm.createTexture("spritemap.sm2", "data\\tileset.tga", glzTexFilter::NEAREST);
	rm.createTexture("spritemap.sm3", "data\\tileset.tga", glzTexFilter::NEAREST);
	rm.createTexture("spritemap.sm4", "data\\tileset.tga", glzTexFilter::NEAREST);

	rm.createTexture("sprite.red", "data\\red.tga", glzTexFilter::NEAREST);
	rm.createTexture("sprite.cursor", "data\\cursor.tga", glzTexFilter::NEAREST);

	glzConfig config; 
	config.load("data\\ztiledata.txt");
	
	if(has_l1) map_l1.load(config.readString("levelTex_1",leveltex_1_filename), glzTileType::DOUBLE_LAYER);
	if(has_l2) map_l2.load(config.readString("levelTex_2", leveltex_2_filename), glzTileType::DOUBLE_LAYER);
	if(has_d) map_dynamic.load(config.readString("levelTex_d", leveltex_d_filename), glzTileType::QUAD_LAYER);


/*	if(has_l1) map_l1.load("data\\supertiles1.tga", glzTileType::DOUBLE_LAYER);
	if(has_l2) map_l2.load("data\\supertiles2.tga", glzTileType::DOUBLE_LAYER);
	if(has_d) map_dynamic.load("data\\supertilesd.tga", glzTileType::QUAD_LAYER);*/


	return TRUE;												// Return TRUE (Initialization Successful)
}


void ZtileState::Deinitialize(void)										// Any User DeInitialization Goes Here
{

	// this shouldn't normally be nessecary, but it's better to make it a habit to delete data for when you start to load and unload resources mid game.

	glzKillAllVAO();		// deletes all vaos in existance


}


void ZtileState::Update(float seconds)								// Perform Motion Updates Here
{

	glzInput input;
	glzResourcemanager rm;
	rm.load_one();

	//Ui stuff
	GLint viewport[4];




	glGetIntegerv(GL_VIEWPORT, viewport);


	Mweel_rel = input.getMouseWeel() - Mweel_old;
	Mpos_x_rel = input.getMouseX() - Mpos_x_old;
	Mpos_y_rel = input.getMouseY() - Mpos_y_old;


	glzMatrix mt;
	mt.LoadIdentity();
	mt.scale(0.17f, 0.17f, 0.17f);


	if(input.getKeyState(VK_ESCAPE) == TRUE)					// Is ESC Being Pressed?
	{
		mMessageQuit = true;						// Terminate The Program
	}

	if(input.getKeyState(VK_F1) == TRUE)						// Is F1 Being Pressed?
	{
		mMessageFullscreen = true;							// Toggle Fullscreen Mode
	}

	gamestate = 1;

	if (input.getKeyState(VK_SPACE))
	{
		gamestate = 2;
	}


	if (input.getKeyState(VK_TAB)) dual_view = true;
	else dual_view = false;





	testanimTimer += seconds;
	if (testanimTimer > 0.45f) { testanim++; testanimTimer -= 0.45f; }

	if (testanim >= 4) testanim = 0;

	keyTimer += seconds;

	if (gamestate == 1)
	{





		if (keyTimer > 0.15)
		{
			if (input.getKeyState(VK_UP)) { cursprite_y--; keyTimer = 0.0; }
			if (input.getKeyState(VK_DOWN)) { cursprite_y++; keyTimer = 0.0; }

			if (input.getKeyState(VK_LEFT)) { cursprite_x--; keyTimer = 0.0; }
			if (input.getKeyState(VK_RIGHT)) { cursprite_x++; keyTimer = 0.0; }

			//if (g_keys->keyDown[VK_UP] == TRUE) { paintarea_pixel_y--; keyTimer = 0.0; }
			//	if (g_keys->keyDown[VK_DOWN] == TRUE) { paintarea_pixel_y++; keyTimer = 0.0; }

			//	if (g_keys->keyDown[VK_LEFT] == TRUE) { paintarea_pixel_x--; keyTimer = 0.0; }
			//	if (g_keys->keyDown[VK_RIGHT] == TRUE) { paintarea_pixel_x++; keyTimer = 0.0; }

			if (input.getKeyState('E')) { if (toggle_extra) toggle_extra = false; else toggle_extra = true; keyTimer = 0.0; }

		}



		if (cursprite_x < 0) cursprite_x = 0;
		if (cursprite_y < 0) cursprite_y = 0;

		if (cursprite_x > tiles_width - 1) cursprite_x = tiles_width - 1;
		if (cursprite_y > tiles_height - 1) cursprite_y = tiles_height - 1;



		if (input.getKeyState(VK_SUBTRACT)) paintarea_Zoom -= seconds * 150.0f;
		if (input.getKeyState(VK_ADD)) paintarea_Zoom += seconds * 150.0f;


		// Todo: add if cursor points at tile surface

		if ((input.getMouseR()) && (z_tileUI_point == ztUIP::PIXELMAP))
		{
			paintarea_x += Mpos_x_rel;
			paintarea_y -= Mpos_y_rel;
		}


		paintarea_Zoom += Mweel_rel;
		if (paintarea_Zoom < 120.0f) paintarea_Zoom = 120.0f;


		float aspect = (float)map_l1.width / (float)map_l1.height;

		if (Mweel_rel>0)
		{
			paintarea_x -= ((input.getMouseX() - viewport[2] / 2.0f) / viewport[2]) * paintarea_Zoom *0.3333f*aspect;
			paintarea_y += ((input.getMouseY() - viewport[3] / 2.0f) / viewport[3]) * paintarea_Zoom *0.3333f;

		}

		if (Mweel_rel<0)
		{
			paintarea_x *= 0.8f;
			paintarea_y *= 0.8f;

		}





		Mweel_old = input.getMouseWeel();
		Mpos_x_old = input.getMouseX();
		Mpos_y_old = input.getMouseY();




		z_tileUI_point = ztUIP::BACKGROUND;


		muip.x = ((input.getMouseX() - viewport[2] / 2.0f) / viewport[2]);
		muip.y = ((input.getMouseY() - viewport[3] / 2.0f) / viewport[3]);

		// this took some dooing
		mwp.x = (((input.getMouseX() - viewport[2] / 2.0f) / viewport[2]) / (paintarea_Zoom / (viewport[2])) - (paintarea_x / paintarea_Zoom));
		mwp.y = ((input.getMouseY() - viewport[3] / 2.0f) / viewport[3]) / (paintarea_Zoom / (viewport[3])) + (paintarea_y / paintarea_Zoom);




		//paintarea_pixel_x = (quantize((mwp.x*arm_width) / aspect, aspect)*aspect)+aspect;   //   (((mwp.x)*arm_width / aspect)*aspect) + (0.5*aspect);
		//paintarea_pixel_x = glzIntegral(((mwp.x)*arm_width / aspect)*aspect) + (0.5*aspect);
		paintarea_pixel_x = glzIntegral((mwp.x + 0.5f*aspect)*map_l1.width / aspect);
		paintarea_pixel_y = glzIntegral((mwp.y + 0.5f)*map_l1.height);


		//paintarea_pixel_x = glzIntegral(((mwp.x + 0.5*(arm_width / arm_height))*arm_width) / (arm_width / arm_height))*(arm_width / arm_height);
		//paintarea_pixel_y = glzIntegral((mwp.y + 0.5)*arm_height);



		if (mwp.x + 0.5f + (map_l1.width) <= 0.0f) { paintarea_pixel_x = 0.0f; }
		if (mwp.y + 0.5f <= 0.0f) { paintarea_pixel_y = 0.0f; }

		if (paintarea_pixel_x > map_l1.width - 1) { paintarea_pixel_x = map_l1.width - 1; }
		if (paintarea_pixel_y > map_l1.height - 1) { paintarea_pixel_y = map_l1.height - 1; }

		if ((((mwp.x + 0.5f*aspect)*map_l1.width / aspect) >= 0.0f) &&
			(((mwp.x + 0.5f*aspect)*map_l1.width / aspect) < map_l1.width) &&
			(mwp.y + 0.5f >= 0.0f) &&
			((mwp.y + 0.5f)*map_l1.height <= map_l1.height)) z_tileUI_point = ztUIP::PIXELMAP;



		if (z_tileUI_point == ztUIP::PIXELMAP)
		{
			if (input.getMouseL())
			{
				if (curlayer == ztLeveltex::DYNAMIC_A) map_dynamic.put_pixel(paintarea_pixel_x, paintarea_pixel_y, 0, cursprite_x + (cursprite_y * 16));
				if (curlayer == ztLeveltex::DYNAMIC_B) map_dynamic.put_pixel(paintarea_pixel_x, paintarea_pixel_y, 1, cursprite_x + (cursprite_y * 16));
				if (curlayer == ztLeveltex::DYNAMIC_C) map_dynamic.put_pixel(paintarea_pixel_x, paintarea_pixel_y, 2, cursprite_x + (cursprite_y * 16));
				if (curlayer == ztLeveltex::DYNAMIC_D) map_dynamic.put_pixel(paintarea_pixel_x, paintarea_pixel_y, 3, cursprite_x + (cursprite_y * 16));


				if (curlayer == ztLeveltex::L1A) map_l1.paint_pixel(paintarea_pixel_x, paintarea_pixel_y, cursprite_x, cursprite_y, cursprite_anim, cursprite_extra, 1);
				if (curlayer == ztLeveltex::L1B) map_l1.paint_pixel(paintarea_pixel_x, paintarea_pixel_y, cursprite_x, cursprite_y, cursprite_anim, cursprite_extra, 2);
				if (curlayer == ztLeveltex::L2A) map_l2.paint_pixel(paintarea_pixel_x, paintarea_pixel_y, cursprite_x, cursprite_y, cursprite_anim, cursprite_extra, 1);
				if (curlayer == ztLeveltex::L2B) map_l2.paint_pixel(paintarea_pixel_x, paintarea_pixel_y, cursprite_x, cursprite_y, cursprite_anim, cursprite_extra, 2);

			}

			if (input.getKeyState('A'))
			{
				if (curlayer == ztLeveltex::L1A) map_l1.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, true, 0);
				if (curlayer == ztLeveltex::L1B) map_l1.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, true, 2);
				if (curlayer == ztLeveltex::L2A) map_l2.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, true, 0);
				if (curlayer == ztLeveltex::L2B) map_l2.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, true, 2);
			}

			if (input.getKeyState('Z'))
			{
				if (curlayer == ztLeveltex::L1A) map_l1.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, false, 0);
				if (curlayer == ztLeveltex::L1B) map_l1.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, false, 2);
				if (curlayer == ztLeveltex::L2A) map_l2.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, false, 0);
				if (curlayer == ztLeveltex::L2B) map_l2.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, false, 2);
			}

			if (input.getKeyState('S'))
			{
				if (curlayer == ztLeveltex::L1A) map_l1.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, true, 1);
				if (curlayer == ztLeveltex::L1B) map_l1.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, true, 3);
				if (curlayer == ztLeveltex::L2A) map_l2.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, true, 1);
				if (curlayer == ztLeveltex::L2B) map_l2.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, true, 3);
			}

			if (input.getKeyState('X'))
			{
				if (curlayer == ztLeveltex::L1A) map_l1.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, false, 1);
				if (curlayer == ztLeveltex::L1B) map_l1.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, false, 3);
				if (curlayer == ztLeveltex::L2A) map_l2.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, false, 1);
				if (curlayer == ztLeveltex::L2B) map_l2.put_extra_bit(paintarea_pixel_x, paintarea_pixel_y, false, 3);
			}


		}
		else
		{

		}


		map_l1.update_texture();
		map_l2.update_texture();
		map_dynamic.update_texture();

		if ((input.getKeyState('1')) && (has_l1)) curlayer = ztLeveltex::L1A;
		if ((input.getKeyState('2')) && (has_l1)) curlayer = ztLeveltex::L1B;
		if ((input.getKeyState('3')) && (has_l2)) curlayer = ztLeveltex::L2A;
		if ((input.getKeyState('4')) && (has_l2)) curlayer = ztLeveltex::L2B;

		if ((input.getKeyState('5')) && (has_d)) curlayer = ztLeveltex::DYNAMIC_A;
		if ((input.getKeyState('6')) && (has_d)) curlayer = ztLeveltex::DYNAMIC_B;
		if ((input.getKeyState('7')) && (has_d)) curlayer = ztLeveltex::DYNAMIC_C;
		if ((input.getKeyState('8')) && (has_d)) curlayer = ztLeveltex::DYNAMIC_D;


	}


	if (gamestate == 2)
	{
		if (z_tileUI_point == ztUIP::PIXELMAP)
		{
			if ((input.getMouseR()) && (z_tileUI_point == ztUIP::PIXELMAP))
			{
				paintarea_x_s += Mpos_x_rel;
				paintarea_y_s -= Mpos_y_rel;
			}


			paintarea_Zoom_s += Mweel_rel;
			if (paintarea_Zoom_s < 120.0f) paintarea_Zoom_s = 120.0f;


		}


		Mweel_old = input.getMouseWeel();
		Mpos_x_old = input.getMouseX();
		Mpos_y_old = input.getMouseY();

		//Ui stuff

		z_tileUI_point = ztUIP::BACKGROUND;


		muip.x = ((input.getMouseX() - viewport[2] / 2.0f) / viewport[2]);
		muip.y = ((input.getMouseY() - viewport[3] / 2.0f) / viewport[3]);

		// this took some dooing
		mwp.x = ((input.getMouseX() - viewport[2] / 2.0f) / viewport[2]) / (paintarea_Zoom_s / (viewport[2])) - (paintarea_x_s / paintarea_Zoom_s);
		mwp.y = ((input.getMouseY() - viewport[3] / 2.0f) / viewport[3]) / (paintarea_Zoom_s / (viewport[3])) + (paintarea_y_s / paintarea_Zoom_s);




		paintarea_pixel_x_s = glzIntegral((mwp.x + 0.5f)*tiles_width);
		paintarea_pixel_y_s = glzIntegral((mwp.y + 0.5f)*tiles_height);


		if (mwp.x + 0.5f <= 0.0f) { paintarea_pixel_x_s = 0.0f; }
		if (mwp.y + 0.5f <= 0.0f) { paintarea_pixel_y_s = 0.0f; }

		if (paintarea_pixel_x_s > tiles_width - 1) { paintarea_pixel_x_s = tiles_width - 1; }
		if (paintarea_pixel_y_s > tiles_height - 1) { paintarea_pixel_y_s = tiles_height - 1; }

		if ((mwp.x + 0.5f >= 0.0f) && ((mwp.x + 0.5f)*tiles_width < tiles_width) && (mwp.y + 0.5f >= 0.0f) && ((mwp.y + 0.5f)*tiles_height <= tiles_height))  z_tileUI_point = ztUIP::PIXELMAP;

		if ((z_tileUI_point == ztUIP::PIXELMAP) && (input.getMouseL() == true)) { cursprite_x = paintarea_pixel_x_s; cursprite_y = paintarea_pixel_y_s; }


	}


	if (keyTimer > 0.15f)
	{
		if (input.getKeyState(VK_RETURN)) {
			if (has_l1)	map_l1.save();
			if (has_l2) map_l2.save();
			if (has_d) map_dynamic.save();
		}
	}
}

void ZtileState::DisplayUpdate(int width, int height)
{
	mView.setDisplay(0, 0, width, height);
}


void ZtileState::Draw(void)
{

	glzResourcemanager rm;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer

	float mtemp[16];
	glEnable(GL_TEXTURE_2D);


	unsigned int atlas_program = glzShaderReurnTilemap();


	unsigned int loc1 = glGetUniformLocation(ProgramObject, "projMat");
	unsigned int loc2 = glGetUniformLocation(ProgramObject, "texunit0");
	unsigned int loc3 = glGetUniformLocation(ProgramObject, "tint");
	
	glUseProgram(ProgramObject);
	glUniform1i(loc2, 0);
	glUniform4f(loc3, 1.0f, 1.0f, 1.0f, 1.0f);


	glUseProgram(atlas_program);
	glzUniform1i(atlas_program, "texunit0", 0);
	glzUniform1i(atlas_program, "texunit1", 1);
	glzUniform4f(atlas_program, "color", 1.0f, 1.0f, 1.0f, 1.0f);

	glzUniform1i(atlas_program, "layer", curlayer);
	glzUniform1i(atlas_program, "anim", testanim);
	glzUniform1i(atlas_program, "width", map_l1.width);
	glzUniform1i(atlas_program, "height", map_l1.height);
	glzUniform1i(atlas_program, "a_width", tiles_width);
	glzUniform1i(atlas_program, "a_height", tiles_height);
	glzUniform1i(atlas_program, "extr", 0);
	

	// i have used these gamestates in a few games for LD48 now and they are really quick and dirty, but effective.
	// they allow you to quickly make a title screen and end screen at the end of the project without changing that much code, instead you just encapsulate it in a gamestate


	if (gamestate == 1)
	{
		glDisable(GL_DEPTH_TEST);
		m.LoadIdentity();

		float aspect = map_l1.width / map_l1.height;


		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);

		m.ortho(-viewport[2] * 0.5f, viewport[2] * 0.5f, -viewport[3] * 0.5f, viewport[3] * 0.5f, -100.0f, 100.0f);
		m.translate(paintarea_x, paintarea_y, 0.0f);
		m.scale(paintarea_Zoom, paintarea_Zoom, 0.0f);
		glUseProgram(atlas_program);
		
		m.transferMatrix(&mtemp[0]);
		glzUniformMatrix4fv(atlas_program, "projMat", m);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm0"));
		glActiveTexture(GL_TEXTURE0);


		if (!dual_view)
		{


			if((curlayer == ztLeveltex::L1A) || (curlayer == ztLeveltex::L1B))	glBindTexture(GL_TEXTURE_2D, map_l1.tex);
			if(has_l2) { if ((curlayer == ztLeveltex::L2A) || (curlayer == ztLeveltex::L2B))	glBindTexture(GL_TEXTURE_2D, map_l2.tex); }
			if(has_d) { if ((curlayer == ztLeveltex::DYNAMIC_A) || (curlayer == ztLeveltex::DYNAMIC_B) || (curlayer == ztLeveltex::DYNAMIC_C) || (curlayer == ztLeveltex::DYNAMIC_D))	glBindTexture(GL_TEXTURE_2D, map_dynamic.tex); }


			if ((curlayer == ztLeveltex::L1A) || (curlayer == ztLeveltex::L2A))	glzUniform1i(atlas_program, "layer", 0);
			if((curlayer == ztLeveltex::L1B) || (curlayer == ztLeveltex::L2B))	glzUniform1i(atlas_program, "layer", 1);


			glActiveTexture(GL_TEXTURE1);
			if(curlayer == ztLeveltex::DYNAMIC_A)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm1")); glzUniform1i(atlas_program, "layer", 2); }
			if(curlayer == ztLeveltex::DYNAMIC_B)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm2")); glzUniform1i(atlas_program, "layer", 3); }
			if(curlayer == ztLeveltex::DYNAMIC_C)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm3")); glzUniform1i(atlas_program, "layer", 4); }
			if(curlayer == ztLeveltex::DYNAMIC_D)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm4")); glzUniform1i(atlas_program, "layer", 5); }
			glActiveTexture(GL_TEXTURE0);

			glzDirectSpriteRender(0.0f, 0.0f, 2.0f, map_l1.width / map_l1.height, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, glzOrigin::CENTERED);

			if (toggle_extra){
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);

				glzUniform1i(atlas_program, "a_height", 1);
				glzDirectSpriteRender(0.0f, 0.0f, 2.0f, map_l1.width / map_l1.height, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, glzOrigin::CENTERED);
				glDisable(GL_BLEND);
			}

			else glzUniform1i(atlas_program, "a_height", 0);

		}
		else
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			if (has_l1)
			{
				glBindTexture(GL_TEXTURE_2D, map_l1.tex);

				glzUniform1i(atlas_program, "layer", 0);
				glzDirectSpriteRender(0.0f, 0.0f, 2.0f, map_l1.width / map_l1.height, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, glzOrigin::CENTERED);

				glzUniform1i(atlas_program, "layer", 1);
				glzDirectSpriteRender(0.0f, 0.0f, 2.0f, map_l1.width / map_l1.height, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, glzOrigin::CENTERED);

			}
			if (has_l2)
			{
				glBindTexture(GL_TEXTURE_2D, map_l2.tex);
				glzUniform1i(atlas_program, "layer", 0);
				glzDirectSpriteRender(0.0f, 0.0f, 2.0f, map_l1.width / map_l1.height, 1.0, 0.0f, 0.0f, 1.0f, 1.0f, glzOrigin::CENTERED);

				glzUniform1i(atlas_program, "layer", 1);
				glzDirectSpriteRender(0.0f, 0.0f, 2.0f, map_l1.width / map_l1.height, 1.0, 0.0f, 0.0f, 1.0f, 1.0f, glzOrigin::CENTERED);

			}

			if ((curlayer == ztLeveltex::DYNAMIC_A) || (curlayer == ztLeveltex::DYNAMIC_B) || (curlayer == ztLeveltex::DYNAMIC_C) || (curlayer == ztLeveltex::DYNAMIC_D))
			{

				glActiveTexture(GL_TEXTURE1);
				if(curlayer == ztLeveltex::DYNAMIC_A)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm1")); glzUniform1i(atlas_program, "layer", 2); }
				if(curlayer == ztLeveltex::DYNAMIC_B)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm2")); glzUniform1i(atlas_program, "layer", 3); }
				if(curlayer == ztLeveltex::DYNAMIC_C)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm3")); glzUniform1i(atlas_program, "layer", 4); }
				if(curlayer == ztLeveltex::DYNAMIC_D)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm4")); glzUniform1i(atlas_program, "layer", 5); }
				glActiveTexture(GL_TEXTURE0);

				glBindTexture(GL_TEXTURE_2D, map_dynamic.tex);
				glzDirectSpriteRender(0.0f, 0.0f, 2.0f, map_l1.width / map_l1.height, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, glzOrigin::CENTERED);
			}
			glDisable(GL_BLEND);

		}


		glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("sprite.cursor"));
		glUseProgram(ProgramObject);

		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);


		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		// render cursor
		if (z_tileUI_point == ztUIP::PIXELMAP) glzDirectSpriteRender(-0.5f*aspect + (0.5f*aspect / map_l1.width) + (paintarea_pixel_x / map_l1.width)*aspect, -0.5f + (0.5f / map_l1.height) + ((map_l1.height - 1 - paintarea_pixel_y) / map_l1.height), 2, (1.0 / map_l1.width)*(map_l1.width / map_l1.height), 1.0 / map_l1.height, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);

		//if ((z_tileUI_point == ztUIP::PIXELMAP) && (fakelevel.getTilecolision((mwp.x + 0.5*aspect)*arm_width / aspect, (mwp.y + 0.5)*arm_height, 1))) glzDirectSpriteRender(-0.5*aspect + (0.5f*aspect / arm_width) + (paintarea_pixel_x / arm_width)*aspect, -0.5 + (0.5f / arm_height) + ((arm_height - 1 - paintarea_pixel_y) / arm_height), 2, (1.0 / arm_width)*(arm_width / arm_height), 1.0 / arm_height, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);



		glDisable(GL_BLEND);






		m.LoadIdentity();
		m.ortho(-viewport[2] * 0.5f, viewport[2] * 0.5f, -viewport[3] * 0.5f, viewport[3] * 0.5f, -100.0f, 100.0f);


		m.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

		glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm0"));
		if(curlayer == ztLeveltex::DYNAMIC_A)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm1")); }
		if(curlayer == ztLeveltex::DYNAMIC_B)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm2")); }
		if(curlayer == ztLeveltex::DYNAMIC_C)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm3")); }
		if(curlayer == ztLeveltex::DYNAMIC_D)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm4")); }
		//glzDirectSpriteRender(viewport[2] / 2, -viewport[3] / 2, 2, 64.0, 64.0, 0, 0, 1.0, 1.0, glzOrigin::BOTTOM_RIGHT);	

		// render the little sprite tile
		glzDirectSpriteRenderAtlas((viewport[2] * 0.5f) - 64, (-viewport[3] * 0.5f) + 64, 1, -64, -64, tiles_width, tiles_height, (cursprite_y* tiles_width) + (tiles_width - cursprite_x - 1), glzOrigin::BOTTOM_RIGHT);

		glEnable(GL_DEPTH_TEST);


	}

	if (gamestate == 2) // sprite selection screen
	{
		glDisable(GL_DEPTH_TEST);
		m.LoadIdentity();



		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);

		m.ortho(-viewport[2] * 0.5f, viewport[2] * 0.5f, -viewport[3] * 0.5f, viewport[3] * 0.5f, -100.0f, 100.0f);
		m.translate(paintarea_x_s, paintarea_y_s, 0.0);
		m.scale(paintarea_Zoom_s, paintarea_Zoom_s, 0.0);
		//glzMatrix m;
		//m.LoadIdentity();
		//m.ortho2DPixelspace(viewport[2], viewport[3], glzOrigin::BOTTOM_LEFT);
		glUseProgram(ProgramObject);


		m.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);
		glzUniform1i(atlas_program, "layer", 0);


		glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm0"));
		if(curlayer == ztLeveltex::DYNAMIC_A)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm1")); }
		if(curlayer == ztLeveltex::DYNAMIC_B)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm2")); }
		if(curlayer == ztLeveltex::DYNAMIC_C)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm3")); }
		if(curlayer == ztLeveltex::DYNAMIC_D)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm4")); }


		glzDirectSpriteRender(0.0, 0.0, 2, 1.0, 1.0, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);



		glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("sprite.cursor"));


		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		if (z_tileUI_point == ztUIP::PIXELMAP) glzDirectSpriteRender(-0.5 + (0.5f / tiles_width) + (paintarea_pixel_x_s / tiles_width), -0.5 + (0.5f / tiles_height) + ((tiles_height - 1 - paintarea_pixel_y_s) / tiles_height), 2, 1.0 / tiles_width, 1.0 / tiles_height, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);
		glDisable(GL_BLEND);






		m.LoadIdentity();
		m.ortho(-viewport[2] / 2, viewport[2] / 2, -viewport[3] / 2, viewport[3] / 2, -100, 100);


		m.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);
		glzUniform1i(atlas_program, "layer", 0);

		glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm0"));

		if(curlayer == ztLeveltex::DYNAMIC_A)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm1")); }
		if(curlayer == ztLeveltex::DYNAMIC_B)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm2")); }
		if(curlayer == ztLeveltex::DYNAMIC_C)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm3")); }
		if(curlayer == ztLeveltex::DYNAMIC_D)	{ glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("spritemap.sm4")); }

		//glzDirectSpriteRender(viewport[2] / 2, -viewport[3] / 2, 2, 64.0, 64.0, 0, 0, 1.0, 1.0, glzOrigin::BOTTOM_RIGHT);
		// render the little sprite tile
		glzDirectSpriteRenderAtlas((viewport[2] / 2) - 64, (-viewport[3] / 2) + 64, 1, -64, -64, tiles_width, tiles_height, (cursprite_y* tiles_width) + (tiles_width - cursprite_x - 1), glzOrigin::BOTTOM_RIGHT);


		glEnable(GL_DEPTH_TEST);


	}



	glUseProgram(0);
	glFlush();													// Flush The GL Rendering Pipeline
}
