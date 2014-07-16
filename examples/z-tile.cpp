// Copyright 2013 Peter Wallström
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



#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>
#include "zeobase2.h"
#include <fstream>
#include <math.h>
#include "..\glz\z-tool_appbase.h"
#include "..\glz\ztool-geo.h"
#include "..\glz\ztool-shader.h"
#include "..\glz\ztool-glz.h"
#include "..\glz\ztool-vectormath.h"
#include "..\glz\ztool-tex.h"
#include "..\glz\ztool-geo-2d.h"
#include "..\glz\ztool-geo-generate.h"

using namespace std;										

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors


GL_Window*	g_window;
Keys*		g_keys;


enum ztUIP { NONE, BACKGROUND, PIXELMAP, SPRITESHEET, SPRITE, UIFRAME };

enum ztLeveltex { L1A, L1B, L2A, L2B, DYNAMIC_A, DYNAMIC_B, DYNAMIC_C, DYNAMIC_D, NO_CHANGE };


// User Defined Variables
float		angle=0,width,height;												// Used To Rotate The Triangles
unsigned int vao[16],vao_num[16],textvao[16],textvao_num[16];
glzMatrix m;
unsigned int spritetexture[5], fonttexture[15], leveltexture[5];


char tbuffer[160];
char tbuffer2[160];
float texttimer=0;
float spriteframetimer=0;
int spriteframe=0;

int gamestate=1,testanim=0;
float testanimTimer = 0;
float keyTimer = 0;

int arm_width = 16, arm_height = 16;
int tiles_width = 16, tiles_height = 16;



//float paintarea_x = WINDOW_WIDTH / 2, paintarea_y = WINDOW_HEIGHT / 2, paintarea_Zoom = 512;

float paintarea_x = 0.0f, paintarea_y = 0.0f, paintarea_Zoom = 512;
float paintarea_pixel_x = 0.0f, paintarea_pixel_y = 0.0f;

float paintarea_x_s = 0.0f, paintarea_y_s = 0.0f, paintarea_Zoom_s = 512;
float paintarea_pixel_x_s = 0.0f, paintarea_pixel_y_s = 0.0f;

GLhandleARB  ProgramObject, ProgramObjectFT, ProgramObjectFSQ, ProgramObjectAtlas;
texture_transform text_tt;


int Mpos_x_old;
int Mpos_y_old;
int Mpos_x_rel;
int Mpos_y_rel;
int Mweel_old;
int Mweel_rel;

vert3 mwp; // mouse work position, z is ignored

vert3 muip;


int z_tileUI_point = ztUIP::BACKGROUND;


int cursprite_x = 0, cursprite_y = 0;
bool cursprite_anim = false, cursprite_extra = false;

ztLeveltex image_has_changed = ztLeveltex::NO_CHANGE;
bool dual_view = false;
bool toggle_extra = false;




ztLeveltex curlayer = ztLeveltex::L1A;
bool has_l1 = true;
bool has_l2 = true;
bool has_d = true;

img_head img_1;
img_head img_2;
img_head img_d;
unsigned char *img_1_data;
unsigned char *img_2_data;
unsigned char *img_d_data;
char leveltex_1_filename[255] = "data\\supertiles1.tga";
char leveltex_2_filename[255] = "data\\supertiles2.tga";
char leveltex_d_filename[255] = "data\\supertilesd.tga";
//char leveltex_filename[255] = "data\\supertilesaspect.tga";
//char leveltex_filename[255] = "data\\a-map.tga";



static PFNGLUSEPROGRAMPROC						glUseProgram;
static PFNGLUNIFORM1IPROC                       glUniform1i;
static PFNGLUNIFORMMATRIX4FVPROC                glUniformMatrix4fv;
static PFNGLUNIFORM4FARBPROC                    glUniform4f;
static PFNGLGETUNIFORMLOCATIONPROC              glGetUniformLocation;
static PFNGLBLENDCOLORPROC						glBlendColor;
static PFNGLACTIVETEXTUREPROC					glActiveTexture;


#define COL_BLACK	0
#define COL_WHITE	1
#define COL_RED		2
#define COL_GREEN	3
#define COL_BLUE	4


int WINDOW_HEIGHT;
int WINDOW_WIDTH;


void preInitialize(void)
{
	glzAppinitialization app;
	app.set_title(L"z-tile level editor");
	WINDOW_HEIGHT = app.data.WINDOW_HEIGHT;
	WINDOW_WIDTH = app.data.WINDOW_WIDTH;
	app.data.WINDOW_WIDTH = 1280;
	app.data.WINDOW_HEIGHT = 720;
	app.data.ALLOW_RESIZE = true;
}


BOOL Initialize (GL_Window* window, Keys* keys)					// Any GL Init Code & User Initialiazation Goes Here
{
	g_window	= window;
	g_keys		= keys;

	GetFocus();
	GetAsyncKeyState(WM_KEYUP);

	// Start Of User Initialization
	angle		= 0.0f;											// Set Starting Angle To Zero

	glClearColor (0.06f, 0.06f, 0.06f, 1.0f);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glFrontFace(GL_CCW);
	//ShowCursor(false);



	glUseProgram				= (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
	glGetUniformLocation		= (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress("glGetUniformLocation");                                                                     
	glUniform1i= (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
	glUniform4f= (PFNGLUNIFORM4FARBPROC) wglGetProcAddress("glUniform4fARB");
	glUniformMatrix4fv= (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");

	glBlendColor = (PFNGLBLENDCOLORPROC)wglGetProcAddress("glBlendColor");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	
	




	glzMatrix mt;
	mt.LoadIdentity();
	mt.scale(0.17f,0.17f,0.17f);

	ProgramObject = glzShaderLoad("data\\glsl.vert", "data\\glsl.frag", glzVAOType::AUTO);
	ProgramObjectAtlas = glzShaderLoad("data\\atlastexture.vert", "data\\atlastexture.frag", glzVAOType::AUTO);
//	ProgramObjectFT = glzShaderLoad("data\\fancytext.vert", "data\\fancytext.frag", glzVAOType::AUTO);
//	ProgramObjectFSQ = glzShaderLoad("data\\fsq.vert", "data\\fsq.frag", glzVAOType::AUTO);
	glzShaderLink(ProgramObject);
	glzShaderLink(ProgramObjectAtlas);
//	glzShaderLink(ProgramObjectFT);
//	glzShaderLink(ProgramObjectFSQ);
	// load the textures
	fonttexture[0] = glzLoadTexture("data\\fonts\\arial.tga", glzTexFilter::LINEAR);
//	fonttexture[1] = glzLoadTexture("data\\fonts\\minya_m.tga", glzTexFilter::LINEAR);
//	fonttexture[2] = glzLoadTexture("data\\fonts\\ms_gothic.tga", glzTexFilter::LINEAR);
	//fonttexture[3] = glzLoadTexture("data\\fonts\\digitalstrip_l.tga", glzTexFilter::LINEAR);
	//fonttexture[4] = glzLoadTexture("data\\fonts\\morpheus_l.tga", glzTexFilter::LINEAR);

//	texture[0] = glzLoadTexture("data\\back.tga", glzTexFilter::LINEAR);
//	texture[1] = glzLoadTexture("data\\derpy_phirana.tga", glzTexFilter::NEAREST);  // the derpy phirana is not an actual logo but just an example on how you can put it there
//	texture[2] = glzLoadTexture("data\\explotion128a.tga", glzTexFilter::NEAREST);


	// load data

	spritetexture[0] = glzLoadTexture("data\\tileset.tga", glzTexFilter::NEAREST);


	if (has_l1)
	{
		glzReadTgaHead(&img_1, leveltex_1_filename);
		img_1_data = new unsigned char[img_1.imageSize];
		glzLoadTga(&img_1, leveltex_1_filename, img_1_data);
		glzMaketex(&img_1, img_1_data, glzTexFilter::NEAREST);
		leveltexture[1] = img_1.m_id;

		arm_width = img_1.m_width;
		arm_height = img_1.m_height;

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img_1.m_width, img_1.m_height, img_1.m_type, GL_UNSIGNED_BYTE, img_1_data);

	}

	if (has_l2)
	{
		glzReadTgaHead(&img_2, leveltex_2_filename);
		img_2_data = new unsigned char[img_2.imageSize];
		glzLoadTga(&img_2, leveltex_2_filename, img_2_data);
		glzMaketex(&img_2, img_2_data, glzTexFilter::NEAREST);
		leveltexture[2] = img_2.m_id;
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img_2.m_width, img_2.m_height, img_2.m_type, GL_UNSIGNED_BYTE, img_2_data);

	}

	if (has_d)
	{
		glzReadTgaHead(&img_d, leveltex_d_filename);
		img_d_data = new unsigned char[img_d.imageSize];
		glzLoadTga(&img_d, leveltex_d_filename, img_d_data);
		glzMaketex(&img_d, img_d_data, glzTexFilter::NEAREST);
		leveltexture[3] = img_d.m_id;
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img_d.m_width, img_d.m_height, img_d.m_type, GL_UNSIGNED_BYTE, img_d_data);

	}





	//texture[1] = glzLoadTexture("data\\a-map.tga", glzTexFilter::NEAREST);


	spritetexture[2] = glzLoadTexture("data\\red.tga", glzTexFilter::NEAREST);
	spritetexture[3] = glzLoadTexture("data\\cursor.tga", glzTexFilter::NEAREST);

	


	return TRUE;												// Return TRUE (Initialization Successful)
}


void Deinitialize (void)										// Any User DeInitialization Goes Here
{

	// this shouldn't normally be nessecary, but it's better to make it a habit to delete data for when you start to load and unload resources mid game.

	delete(img_1_data);

	glzKillAllVAO();		// deletes all vaos in existance


}

void paint_pixel(int x, int y, int px, int py, bool animate, bool extra, ztLeveltex layer)
{

	if ((layer == ztLeveltex::DYNAMIC_A) || (layer == ztLeveltex::DYNAMIC_B) || (layer == ztLeveltex::DYNAMIC_C) || (layer == ztLeveltex::DYNAMIC_D)) return;

	// get data
	int d_o = 0;
	char dx, dy;

	if ((layer == ztLeveltex::L1B) || (layer == ztLeveltex::L2B)) d_o = 2;

	if ((layer == ztLeveltex::L1A) || (layer == ztLeveltex::L1B))
	{ 
		dx = img_1_data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, img_1.m_width, img_1.m_height, true)];
		dy = img_1_data[glz2dTo1dImageRemap(x, y, 1 + d_o, 4, img_1.m_width, img_1.m_height, true)];
	}

	else
	{
		dx = img_2_data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, img_2.m_width, img_2.m_height, true)];
		dy = img_2_data[glz2dTo1dImageRemap(x, y, 1 + d_o, 4, img_2.m_width, img_2.m_height, true)];
	}



	bool ani=false, ext = false;
	if (dx > 127) { dx -= 128; ani = true; }
	if (dy > 127) { dy -= 128; ext = true; }

	if ((px == dx) && (py == dy) && (animate == ani) && (extra == ext)) return; //no change so do nothing

	if (px > 127) px = 127;
	if (py > 127) py = 127;

	if (animate) px += 128;
	if (extra) py += 128;

	if ((layer == ztLeveltex::L1A) || (layer == ztLeveltex::L1B))
	{
		img_1_data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, img_1.m_width, img_1.m_height, true)] = px;
		img_1_data[glz2dTo1dImageRemap(x, y, 1 + d_o, 4, img_1.m_width, img_1.m_height, true)] = py;
	}

	else
	{
		img_2_data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, img_2.m_width, img_2.m_height, true)] = px;
		img_2_data[glz2dTo1dImageRemap(x, y, 1 + d_o, 4, img_2.m_width, img_2.m_height, true)] = py;
	}	
	
	image_has_changed = layer;

	return;
}


void paint_dynamic_pixel(int x, int y, int data, ztLeveltex layer)
{

	if ((layer == ztLeveltex::L1A) || (layer == ztLeveltex::L1B) || (layer == ztLeveltex::L2A) || (layer == ztLeveltex::L2B)) return;


	// get data
	int d_o = 0;
	char dx;

	if (layer == ztLeveltex::DYNAMIC_A) d_o = 0;
	if (layer == ztLeveltex::DYNAMIC_B) d_o = 1;
	if (layer == ztLeveltex::DYNAMIC_C) d_o = 2;
	if (layer == ztLeveltex::DYNAMIC_D) d_o = 3;	

	dx = img_d_data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, img_d.m_width, img_d.m_height, true)];	
	img_d_data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, img_d.m_width, img_d.m_height, true)] = data;

	image_has_changed = layer;

	return;
}



void paint_animate(int x, int y, bool data, ztLeveltex layer)
{

	// get data
	int d_o = 0;
	char dx;
	if ((layer == ztLeveltex::DYNAMIC_A) || (layer == ztLeveltex::DYNAMIC_B) || (layer == ztLeveltex::DYNAMIC_C) || (layer == ztLeveltex::DYNAMIC_D)) return;

	if ((layer == ztLeveltex::L1B) || (layer == ztLeveltex::L2B)) d_o = 2;

	if ((layer == ztLeveltex::L1A) || (layer == ztLeveltex::L1B))
	{
		dx = img_1_data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, img_1.m_width, img_1.m_height, true)];
	}
	else
	{
		dx = img_2_data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, img_2.m_width, img_2.m_height, true)];
	}

	if ((layer == ztLeveltex::L1A) || (layer == ztLeveltex::L1B))
	{
		if (data) img_1_data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, img_1.m_width, img_1.m_height, true)] = dx | 128;
		else  img_1_data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, img_1.m_width, img_1.m_height, true)] = dx & 127;
	}
	else
	{
		if (data) img_2_data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, img_2.m_width, img_2.m_height, true)] = dx | 128;
		else  img_2_data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, img_2.m_width, img_2.m_height, true)] = dx & 127;
	}

	
	image_has_changed = layer;

	return;

}

void paint_extra(int x, int y, bool data, ztLeveltex layer)
{

	// get data
	int d_o = 0;
	char dx;
	if ((layer == ztLeveltex::DYNAMIC_A) || (layer == ztLeveltex::DYNAMIC_A) || (layer == ztLeveltex::DYNAMIC_A) || (layer == ztLeveltex::DYNAMIC_A)) return;

	if ((layer == ztLeveltex::L1B) || (layer == ztLeveltex::L2B)) d_o = 2;

	if ((layer == ztLeveltex::L1A) || (layer == ztLeveltex::L1B))
	{
		dx = img_1_data[glz2dTo1dImageRemap(x, y, 1 + d_o, 4, img_1.m_width, img_1.m_height, true)];
	}
	else
	{
		dx = img_2_data[glz2dTo1dImageRemap(x, y, 1 + d_o, 4, img_2.m_width, img_2.m_height, true)];
	}

	if ((layer == ztLeveltex::L1A) || (layer == ztLeveltex::L1B))
	{
		if (data) img_1_data[glz2dTo1dImageRemap(x, y, 1 + d_o, 4, img_1.m_width, img_1.m_height, true)] = dx | 128;
		else  img_1_data[glz2dTo1dImageRemap(x, y, 1 + d_o, 4, img_1.m_width, img_1.m_height, true)] = dx & 127;
	}
	else
	{
		if (data) img_2_data[glz2dTo1dImageRemap(x, y, 1 + d_o, 4, img_2.m_width, img_2.m_height, true)] = dx | 128;
		else  img_2_data[glz2dTo1dImageRemap(x, y, 1 + d_o, 4, img_2.m_width, img_2.m_height, true)] = dx & 127;
	}


	image_has_changed = layer;

	return;

}


void Update (float seconds)								// Perform Motion Updates Here
{

	//Ui stuff
	GLint viewport[4];

	image_has_changed = ztLeveltex::NO_CHANGE;
	glGetIntegerv(GL_VIEWPORT, viewport);


	Mweel_rel = g_keys->Mweel -Mweel_old;
	Mpos_x_rel = g_keys->Mpos_x - Mpos_x_old;
	Mpos_y_rel = g_keys->Mpos_y - Mpos_y_old;


		glzMatrix mt;
		mt.LoadIdentity();
		mt.scale(0.17f,0.17f,0.17f);


	if (g_keys->keyDown [VK_ESCAPE] == TRUE)					// Is ESC Being Pressed?
	{
		TerminateApplication (g_window);						// Terminate The Program
	}

	if (g_keys->keyDown [VK_F1] == TRUE)						// Is F1 Being Pressed?
	{
		ToggleFullscreen (g_window);							// Toggle Fullscreen Mode
	}

	gamestate = 1;

	if (g_keys->keyDown[VK_SPACE] == TRUE)					
	{
		gamestate = 2;
	}


	if (g_keys->keyDown[VK_TAB] == TRUE) dual_view = true;
	else dual_view = false;





	testanimTimer += seconds;
	if (testanimTimer > 0.45) { testanim++; testanimTimer -= 0.45; }

	if (testanim >= 4) testanim = 0;

	keyTimer += seconds;	
	
	if (gamestate==1)
	{
	



	
	if (keyTimer > 0.15) 
	{
		if (g_keys->keyDown[VK_UP] == TRUE) { cursprite_y--; keyTimer = 0.0; }
		if (g_keys->keyDown[VK_DOWN] == TRUE) { cursprite_y++; keyTimer = 0.0; }

		if (g_keys->keyDown[VK_LEFT] == TRUE) { cursprite_x--; keyTimer = 0.0; }
		if (g_keys->keyDown[VK_RIGHT] == TRUE) { cursprite_x++; keyTimer = 0.0; }

		//if (g_keys->keyDown[VK_UP] == TRUE) { paintarea_pixel_y--; keyTimer = 0.0; }
		//	if (g_keys->keyDown[VK_DOWN] == TRUE) { paintarea_pixel_y++; keyTimer = 0.0; }

		//	if (g_keys->keyDown[VK_LEFT] == TRUE) { paintarea_pixel_x--; keyTimer = 0.0; }
		//	if (g_keys->keyDown[VK_RIGHT] == TRUE) { paintarea_pixel_x++; keyTimer = 0.0; }

		if (g_keys->keyDown['E'] == TRUE) { if (toggle_extra) toggle_extra = false; else toggle_extra = true; keyTimer = 0.0; }

	}
	


	if (cursprite_x < 0) cursprite_x = 0;
	if (cursprite_y < 0) cursprite_y = 0;

	if (cursprite_x > tiles_width-1) cursprite_x = tiles_width-1;
	if (cursprite_y > tiles_height-1) cursprite_y = tiles_height-1;



		if (g_keys->keyDown[VK_SUBTRACT] == TRUE) paintarea_Zoom -= seconds * 150;
		if (g_keys->keyDown[VK_ADD] == TRUE) paintarea_Zoom += seconds * 150;


		// Todo: add if cursor points at tile surface
	
		if ((g_keys->RMdown == TRUE) && (z_tileUI_point == ztUIP::PIXELMAP))
			{
				paintarea_x += Mpos_x_rel;
				paintarea_y -= Mpos_y_rel;
			}
	

		paintarea_Zoom += Mweel_rel;
		if (paintarea_Zoom < 120) paintarea_Zoom = 120;


		float aspect = arm_width / arm_height;

		if (Mweel_rel>0)
		{
			paintarea_x -= ((g_keys->Mpos_x - viewport[2] / 2.0) / viewport[2]) * paintarea_Zoom *0.3333*aspect;
			paintarea_y += ((g_keys->Mpos_y - viewport[3] / 2.0) / viewport[3]) * paintarea_Zoom *0.3333;
			
		}

		if (Mweel_rel<0)
		{
			paintarea_x *=  0.8;
			paintarea_y *= 0.8;
			
		}
	




	Mweel_old = g_keys->Mweel;
	Mpos_x_old = g_keys->Mpos_x;
	Mpos_y_old = g_keys->Mpos_y;




	z_tileUI_point = ztUIP::BACKGROUND;
	

	muip.x = ((g_keys->Mpos_x - viewport[2] / 2.0) / viewport[2]);
	muip.y = ((g_keys->Mpos_y - viewport[3] / 2.0) / viewport[3]);

		// this took some dooing
	mwp.x = (((g_keys->Mpos_x - viewport[2] / 2.0) / viewport[2]) / (paintarea_Zoom / (viewport[2])) - (paintarea_x / paintarea_Zoom));
	mwp.y = ((g_keys->Mpos_y  - viewport[3] / 2.0) / viewport[3]) / (paintarea_Zoom / (viewport[3])) + (paintarea_y / paintarea_Zoom);

	


	//paintarea_pixel_x = (quantize((mwp.x*arm_width) / aspect, aspect)*aspect)+aspect;   //   (((mwp.x)*arm_width / aspect)*aspect) + (0.5*aspect);
	//paintarea_pixel_x = glzIntegral(((mwp.x)*arm_width / aspect)*aspect) + (0.5*aspect);
	paintarea_pixel_x = glzIntegral((mwp.x + 0.5*aspect)*arm_width/aspect);
	paintarea_pixel_y = glzIntegral((mwp.y + 0.5)*arm_height);


	//paintarea_pixel_x = glzIntegral(((mwp.x + 0.5*(arm_width / arm_height))*arm_width) / (arm_width / arm_height))*(arm_width / arm_height);
	//paintarea_pixel_y = glzIntegral((mwp.y + 0.5)*arm_height);


	
	if (mwp.x + 0.5+(arm_width) <= 0.0) { paintarea_pixel_x = 0.0; }
	if (mwp.y + 0.5 <= 0.0) { paintarea_pixel_y = 0.0; }

	if (paintarea_pixel_x > arm_width - 1) { paintarea_pixel_x = arm_width - 1; }
	if (paintarea_pixel_y > arm_height - 1) { paintarea_pixel_y = arm_height - 1; }

	if ((((mwp.x + 0.5*aspect)*arm_width / aspect) >= 0.0) &&
		(((mwp.x + 0.5*aspect)*arm_width / aspect) < arm_width) &&
		(mwp.y + 0.5 >= 0.0) && 
		((mwp.y + 0.5)*arm_height <= arm_height)) z_tileUI_point = ztUIP::PIXELMAP;

	

	if (z_tileUI_point == ztUIP::PIXELMAP)
	{ 
		if (g_keys->LMdown == TRUE)
		{
			paint_dynamic_pixel(paintarea_pixel_x, paintarea_pixel_y, cursprite_x + (cursprite_y * 16), curlayer);
			paint_pixel(paintarea_pixel_x, paintarea_pixel_y, cursprite_x, cursprite_y, cursprite_anim, cursprite_extra, curlayer);
		}
		

		if (g_keys->keyDown['A'] == TRUE) paint_animate(paintarea_pixel_x, paintarea_pixel_y, true, curlayer);
		if (g_keys->keyDown['Z'] == TRUE) paint_animate(paintarea_pixel_x, paintarea_pixel_y, false, curlayer);
		if (g_keys->keyDown['S'] == TRUE) paint_extra(paintarea_pixel_x, paintarea_pixel_y, true, curlayer);
		if (g_keys->keyDown['X'] == TRUE) paint_extra(paintarea_pixel_x, paintarea_pixel_y, false, curlayer);
	}
	else
	{
		
	}


	switch (image_has_changed)
	{
		case ztLeveltex::L1A:
		case ztLeveltex::L1B:
			glBindTexture(GL_TEXTURE_2D, leveltexture[1]);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img_1.m_width, img_1.m_height, img_1.m_type, GL_UNSIGNED_BYTE, img_1_data);			
			break;

		case ztLeveltex::L2A:
		case ztLeveltex::L2B:
			glBindTexture(GL_TEXTURE_2D, leveltexture[2]);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img_2.m_width, img_2.m_height, img_2.m_type, GL_UNSIGNED_BYTE, img_2_data);
			break;

		case ztLeveltex::DYNAMIC_A:
		case ztLeveltex::DYNAMIC_B:
		case ztLeveltex::DYNAMIC_C:
		case ztLeveltex::DYNAMIC_D:
			glBindTexture(GL_TEXTURE_2D, leveltexture[3]);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img_d.m_width, img_d.m_height, img_d.m_type, GL_UNSIGNED_BYTE, img_d_data);
			break;

		default:
			break;
	}
	
	image_has_changed = ztLeveltex::NO_CHANGE;

	if ((g_keys->keyDown['1'] == TRUE) && (has_l1)) curlayer = ztLeveltex::L1A;
	if ((g_keys->keyDown['2'] == TRUE) && (has_l1)) curlayer = ztLeveltex::L1B;
	if ((g_keys->keyDown['3'] == TRUE) && (has_l2)) curlayer = ztLeveltex::L2A;
	if ((g_keys->keyDown['4'] == TRUE) && (has_l2)) curlayer = ztLeveltex::L2B;

	if ((g_keys->keyDown['5'] == TRUE) && (has_d)) curlayer = ztLeveltex::DYNAMIC_A;
	if ((g_keys->keyDown['6'] == TRUE) && (has_d)) curlayer = ztLeveltex::DYNAMIC_B;
	if ((g_keys->keyDown['7'] == TRUE) && (has_d)) curlayer = ztLeveltex::DYNAMIC_C;
	if ((g_keys->keyDown['8'] == TRUE) && (has_d)) curlayer = ztLeveltex::DYNAMIC_D;
	

		}


	if (gamestate == 2)
	{
		if (z_tileUI_point == ztUIP::PIXELMAP)
		{
			if ((g_keys->RMdown == TRUE) && (z_tileUI_point == ztUIP::PIXELMAP))
			{
				paintarea_x_s += Mpos_x_rel;
				paintarea_y_s -= Mpos_y_rel;
			}


			paintarea_Zoom_s += Mweel_rel;
			if (paintarea_Zoom_s < 120) paintarea_Zoom_s = 120;


		}


			Mweel_old = g_keys->Mweel;
			Mpos_x_old = g_keys->Mpos_x;
			Mpos_y_old = g_keys->Mpos_y;

			//Ui stuff
		
			z_tileUI_point = ztUIP::BACKGROUND;


			muip.x = ((g_keys->Mpos_x - viewport[2] / 2.0) / viewport[2]);
			muip.y = ((g_keys->Mpos_y - viewport[3] / 2.0) / viewport[3]);

			// this took some dooing
			mwp.x = ((g_keys->Mpos_x - viewport[2] / 2.0) / viewport[2]) / (paintarea_Zoom_s / (viewport[2])) - (paintarea_x_s / paintarea_Zoom_s);
			mwp.y = ((g_keys->Mpos_y - viewport[3] / 2.0) / viewport[3]) / (paintarea_Zoom_s / (viewport[3])) + (paintarea_y_s / paintarea_Zoom_s);




			paintarea_pixel_x_s = glzIntegral((mwp.x + 0.5)*tiles_width);
			paintarea_pixel_y_s = glzIntegral((mwp.y + 0.5)*tiles_height);


			if (mwp.x + 0.5 <= 0.0) { paintarea_pixel_x_s = 0.0; }
			if (mwp.y + 0.5 <= 0.0) { paintarea_pixel_y_s = 0.0; }

			if (paintarea_pixel_x_s > tiles_width - 1) { paintarea_pixel_x_s = tiles_width - 1; }
			if (paintarea_pixel_y_s > tiles_height - 1) { paintarea_pixel_y_s = tiles_height - 1; }

			if ((mwp.x + 0.5 >= 0.0) && ((mwp.x + 0.5)*tiles_width < tiles_width) && (mwp.y + 0.5 >= 0.0) && ((mwp.y + 0.5)*tiles_height <= tiles_height))  z_tileUI_point = ztUIP::PIXELMAP;
			
			if ((z_tileUI_point == ztUIP::PIXELMAP) && (g_keys->LMdown == TRUE)) { cursprite_x = paintarea_pixel_x_s; cursprite_y = paintarea_pixel_y_s; }

		
	}


	if (keyTimer > 0.15)
	{
		if (g_keys->keyDown[VK_RETURN]) {
			if (has_l1)
				glzSaveTGA(leveltex_1_filename, img_1.m_width, img_1.m_height, 0, glzTexCompression::UNCOMPRESSED, img_1.m_type, img_1_data); keyTimer = 0; 

			if (has_l2)
				glzSaveTGA(leveltex_2_filename, img_2.m_width, img_2.m_height, 0, glzTexCompression::UNCOMPRESSED, img_2.m_type, img_2_data); keyTimer = 0;

			if (has_d)
				glzSaveTGA(leveltex_d_filename, img_d.m_width, img_d.m_height, 0, glzTexCompression::UNCOMPRESSED, img_d.m_type, img_d_data); keyTimer = 0;
		}
	}
}


void draw_text(float x, float y, int text, int font, unsigned int po, unsigned int col)
{
	glUseProgram(po);

	unsigned int loc1 = glGetUniformLocation(po,"projMat");
	unsigned int loc2 = glGetUniformLocation(po,"texunit0");
	unsigned int loc3 = glGetUniformLocation(po,"tint");
	m.LoadIdentity();
	m.ortho( -4, 4, -2, 2, -100, 100);
	m.translate(x,y,0);

	float mtemp[16];
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

	if (col==COL_BLACK)	glUniform4f(loc3, 0.0f,0.0f,0.0f,1.0f);
	if (col==COL_WHITE)	glUniform4f(loc3, 1.0f,1.0f,1.0f,1.0f);
	if (col==COL_RED)	glUniform4f(loc3, 1.0f,0.0f,0.0f,1.0f);
	if (col==COL_GREEN)	glUniform4f(loc3, 0.0f,1.0f,0.0f,1.0f);
	if (col==COL_BLUE)	glUniform4f(loc3, 0.0f,0.0f,1.0f,1.0f);


	

	glBindTexture(GL_TEXTURE_2D,fonttexture[font]);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glzDrawVAO(textvao_num[text],textvao[text],GL_TRIANGLES);
	glDisable(GL_BLEND);

}

void draw_text2(char text[255], float x, float y, float scale, float kern, int font, unsigned int po, unsigned int col)
{
	glUseProgram(po);

	unsigned int loc1 = glGetUniformLocation(po, "projMat");
	unsigned int loc2 = glGetUniformLocation(po, "texunit0");
	unsigned int loc3 = glGetUniformLocation(po, "tint");
	m.LoadIdentity();
	m.ortho2DPixelspace( WINDOW_HEIGHT, WINDOW_WIDTH, glzOrigin::BOTTOM_LEFT);	
	m.translate(x, y, 0);

	float mtemp[16];
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

	if (col == COL_BLACK)	glUniform4f(loc3, 0.0f, 0.0f, 0.0f, 1.0f);
	if (col == COL_WHITE)	glUniform4f(loc3, 1.0f, 1.0f, 1.0f, 1.0f);
	if (col == COL_RED)	glUniform4f(loc3, 1.0f, 0.0f, 0.0f, 1.0f);
	if (col == COL_GREEN)	glUniform4f(loc3, 0.0f, 1.0f, 0.0f, 1.0f);
	if (col == COL_BLUE)	glUniform4f(loc3, 0.0f, 0.0f, 1.0f, 1.0f);

	float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, fonttexture[font]);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glzDirectDrawText(text, scale, aspect, kern, glzOrigin::BOTTOM_LEFT);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

}



// custom sprite rendering function, if you have a lot of sprites then one of these is probably usefull
void draw_sprite(float x, float y, float s, int sprite, int tx, int offset, unsigned int po, float col[4])
{
	glUseProgram(po);

	unsigned int loc1 = glGetUniformLocation(po,"projMat");
	unsigned int loc2 = glGetUniformLocation(po,"texunit0");
	unsigned int loc3 = glGetUniformLocation(po,"tint");
	m.LoadIdentity();
	m.ortho(-4, 4, -2, 2, -100, 100);
	m.translate(x,y,0);
	m.scale(s,s,s);

	float mtemp[16];
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

	glUniform4f(loc3, col[0],col[1],col[2],col[3]);

	glBindTexture(GL_TEXTURE_2D, spritetexture[tx]);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glzDrawVAO(offset*6,6,vao[sprite],GL_TRIANGLES);
	glDisable(GL_BLEND);

}

// backdrops are usefull, if you don't have the time to write a 2D renderer then just cheat with a texture backdrop instead
void draw_backdrop(unsigned int bgtexture)
{
	glUseProgram(ProgramObjectFSQ);
	unsigned int loc1 = glGetUniformLocation(ProgramObjectFSQ,"projMat");
	unsigned int loc2 = glGetUniformLocation(ProgramObjectFSQ,"texunit0");
	unsigned int loc3 = glGetUniformLocation(ProgramObjectFSQ,"tint");
	
	m.LoadIdentity();

	float mtemp[16];
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);
	glUniform1i(loc2, 0);
	glUniform4f(loc3, 1.0f,1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D,bgtexture);
	glzDrawVAO(vao_num[0],vao[0],GL_TRIANGLES);

}

void draw_backdrop2(unsigned int bgtexture, glzMatrix mat, float col[4])
{
	glUseProgram(ProgramObjectFSQ);
	unsigned int loc1 = glGetUniformLocation(ProgramObjectFSQ,"projMat");
	unsigned int loc2 = glGetUniformLocation(ProgramObjectFSQ,"texunit0");
	unsigned int loc3 = glGetUniformLocation(ProgramObjectFSQ,"tint");
	


	float mtemp[16];
	mat.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

	glUniform1i(loc2, 0);
	glUniform4f(loc3, col[0],col[1],col[2],col[3]);
	glBindTexture(GL_TEXTURE_2D,bgtexture);
	glzDrawVAO(vao_num[0],vao[0],GL_TRIANGLES);

}


void Draw (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	
	float mtemp[16];
	glEnable(GL_TEXTURE_2D);
	unsigned int loc1 = glGetUniformLocation(ProgramObject,"projMat");
	unsigned int loc2 = glGetUniformLocation(ProgramObject,"texunit0");
	unsigned int loc3 = glGetUniformLocation(ProgramObject,"tint");


	unsigned int loc4 = glGetUniformLocation(ProgramObjectAtlas, "projMat");
	unsigned int loc5 = glGetUniformLocation(ProgramObjectAtlas, "texunit0");
	unsigned int loc6 = glGetUniformLocation(ProgramObjectAtlas, "texunit1");
	unsigned int loc7 = glGetUniformLocation(ProgramObjectAtlas, "layer");
	unsigned int loc8 = glGetUniformLocation(ProgramObjectAtlas, "anim");

	unsigned int loc9 = glGetUniformLocation(ProgramObjectAtlas, "width");
	unsigned int loc10 = glGetUniformLocation(ProgramObjectAtlas, "height");
	unsigned int loc11 = glGetUniformLocation(ProgramObjectAtlas, "a_width");
	unsigned int loc12 = glGetUniformLocation(ProgramObjectAtlas, "a_height");
	unsigned int loc13 = glGetUniformLocation(ProgramObjectAtlas, "extr");




//	unsigned int loc4 = glGetUniformLocation(ProgramObjectFT,"projMat");
//	unsigned int loc5 = glGetUniformLocation(ProgramObjectFT,"texunit0");

	glUseProgram(ProgramObject);
	glUniform1i(loc2, 0);	
	glUniform4f(loc3, 1.0f,1.0f,1.0f,1.0f);

	glUseProgram(ProgramObjectAtlas);
	glUniform1i(loc5, 0);
	glUniform1i(loc6, 1);
	glUniform1i(loc7, curlayer);
	glUniform1i(loc8, testanim);

	glUniform1i(loc9, arm_width);
	glUniform1i(loc10, arm_height);
	glUniform1i(loc11, tiles_width);
	glUniform1i(loc12, tiles_height);
	glUniform1i(loc13, 0);






	// i have used these gamestates in a few games for LD48 now and they are really quick and dirty, but effective.
	// they allow you to quickly make a title screen and end screen at the end of the project without changing that much code, instead you just encapsulate it in a gamestate


	if (gamestate == 1)
	{
		glDisable(GL_DEPTH_TEST);
		m.LoadIdentity();
	
		float aspect = arm_width / arm_height;
		

		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);

		m.ortho(-viewport[2] / 2, viewport[2] / 2, -viewport[3]/2, viewport[3]/2, -100, 100);
		m.translate(paintarea_x, paintarea_y, 0.0);
		m.scale(paintarea_Zoom, paintarea_Zoom, 0.0);
		//glzMatrix m;
		//m.LoadIdentity();
		//m.ortho2DPixelspace(viewport[2], viewport[3], glzOrigin::BOTTOM_LEFT);
		glUseProgram(ProgramObjectAtlas);


		m.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc4, 1, GL_FALSE, mtemp);
		

	



		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, spritetexture[0]);
		glActiveTexture(GL_TEXTURE0);


		if (!dual_view)
		{


			if ((curlayer == ztLeveltex::L1A) || (curlayer == ztLeveltex::L1B))	glBindTexture(GL_TEXTURE_2D, leveltexture[1]);
			if (has_l2) { if ((curlayer == ztLeveltex::L2A) || (curlayer == ztLeveltex::L2B))	glBindTexture(GL_TEXTURE_2D, leveltexture[2]); }
			if (has_d) { if ((curlayer == ztLeveltex::DYNAMIC_A) || (curlayer == ztLeveltex::DYNAMIC_B) || (curlayer == ztLeveltex::DYNAMIC_C) || (curlayer == ztLeveltex::DYNAMIC_D))	glBindTexture(GL_TEXTURE_2D, leveltexture[3]); }



			if ((curlayer == ztLeveltex::L1A) || (curlayer == ztLeveltex::L2A))glUniform1i(loc7, 0);
			if ((curlayer == ztLeveltex::L1B) || (curlayer == ztLeveltex::L2B))	glUniform1i(loc7, 1);
			
			if (curlayer == ztLeveltex::DYNAMIC_A)	glUniform1i(loc7, 2);
			if (curlayer == ztLeveltex::DYNAMIC_B)	glUniform1i(loc7, 3);
			if (curlayer == ztLeveltex::DYNAMIC_C)	glUniform1i(loc7, 4);
			if (curlayer == ztLeveltex::DYNAMIC_D)	glUniform1i(loc7, 5);


			glzDirectSpriteRender(0.0, 0.0, 2, arm_width / arm_height, 1.0, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);

		

			if (toggle_extra){
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);

				glUniform1i(loc13, 1);
				glzDirectSpriteRender(0.0, 0.0, 2, arm_width / arm_height, 1.0, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);
				glDisable(GL_BLEND);
			}

			else glUniform1i(loc13, 0);

		}
		else
		{    

			
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			if (has_l1)
			{
				glBindTexture(GL_TEXTURE_2D, leveltexture[1]);
				glUniform1i(loc7, 0);
				glzDirectSpriteRender(0.0, 0.0, 2, arm_width / arm_height, 1.0, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);

				glUniform1i(loc7, 1);
				glzDirectSpriteRender(0.0, 0.0, 2, arm_width / arm_height, 1.0, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);

			}
			if (has_l2)
			{
				glBindTexture(GL_TEXTURE_2D, leveltexture[2]);
				glUniform1i(loc7, 0);
				glzDirectSpriteRender(0.0, 0.0, 2, arm_width / arm_height, 1.0, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);

				glUniform1i(loc7, 1);
				glzDirectSpriteRender(0.0, 0.0, 2, arm_width / arm_height, 1.0, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);

			}

			if ((curlayer == ztLeveltex::DYNAMIC_A) || (curlayer == ztLeveltex::DYNAMIC_B) || (curlayer == ztLeveltex::DYNAMIC_C) || (curlayer == ztLeveltex::DYNAMIC_D))
			{ 

				if (curlayer == ztLeveltex::DYNAMIC_A)	glUniform1i(loc7, 2);
				if (curlayer == ztLeveltex::DYNAMIC_B)	glUniform1i(loc7, 3);
				if (curlayer == ztLeveltex::DYNAMIC_C)	glUniform1i(loc7, 4);
				if (curlayer == ztLeveltex::DYNAMIC_D)	glUniform1i(loc7, 5);

				glBindTexture(GL_TEXTURE_2D, leveltexture[3]);
				glzDirectSpriteRender(0.0, 0.0, 2, arm_width / arm_height, 1.0, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);

				
			}



			
			glDisable(GL_BLEND);

		}



		glBindTexture(GL_TEXTURE_2D, spritetexture[3]);
		glUseProgram(ProgramObject);

		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);
	
	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		
		if (z_tileUI_point == ztUIP::PIXELMAP) glzDirectSpriteRender(-0.5*aspect + (0.5f*aspect / arm_width) + (paintarea_pixel_x / arm_width)*aspect, -0.5 + (0.5f / arm_height) + ((arm_height - 1 - paintarea_pixel_y) / arm_height), 2, (1.0 / arm_width)*(arm_width / arm_height), 1.0 / arm_height, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);

		glDisable(GL_BLEND);
		
	




		m.LoadIdentity();
		m.ortho(-viewport[2] / 2, viewport[2] / 2, -viewport[3] / 2, viewport[3] / 2, -100, 100);

	
		m.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

		glBindTexture(GL_TEXTURE_2D, spritetexture[0]);
		//glzDirectSpriteRender(viewport[2] / 2, -viewport[3] / 2, 2, 64.0, 64.0, 0, 0, 1.0, 1.0, glzOrigin::BOTTOM_RIGHT);
		glzDirectSpriteRenderAtlas(viewport[2] / 2, -viewport[3] / 2, 1, 64, 64, tiles_width, tiles_height, (cursprite_y* tiles_width) + (tiles_width - cursprite_x - 1), glzOrigin::BOTTOM_RIGHT);

		glEnable(GL_DEPTH_TEST);


	}

	if (gamestate == 2) // sprite selection screen
	{
		glDisable(GL_DEPTH_TEST);
		m.LoadIdentity();



		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);

		m.ortho(-viewport[2] / 2, viewport[2] / 2, -viewport[3] / 2, viewport[3] / 2, -100, 100);
		m.translate(paintarea_x_s, paintarea_y_s, 0.0);
		m.scale(paintarea_Zoom_s, paintarea_Zoom_s, 0.0);
		//glzMatrix m;
		//m.LoadIdentity();
		//m.ortho2DPixelspace(viewport[2], viewport[3], glzOrigin::BOTTOM_LEFT);
		glUseProgram(ProgramObject);


		m.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);
		glUniform1i(loc7, 0);

				
		glBindTexture(GL_TEXTURE_2D, spritetexture[0]);
		
				glzDirectSpriteRender(0.0 , 0.0, 2, 1.0 , 1.0 , 0, 0, 1.0, 1.0, glzOrigin::CENTERED);



				glBindTexture(GL_TEXTURE_2D, spritetexture[3]);
		

		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		if (z_tileUI_point == ztUIP::PIXELMAP) glzDirectSpriteRender(-0.5 + (0.5f / tiles_width) + (paintarea_pixel_x_s / tiles_width), -0.5 + (0.5f / tiles_height) + ((tiles_height - 1 - paintarea_pixel_y_s) / tiles_height), 2, 1.0 / tiles_width, 1.0 / tiles_height, 0, 0, 1.0, 1.0, glzOrigin::CENTERED);
		glDisable(GL_BLEND);

		




		m.LoadIdentity();
		m.ortho(-viewport[2] / 2, viewport[2] / 2, -viewport[3] / 2, viewport[3] / 2, -100, 100);


		m.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);
		glUniform1i(loc7, 0);

		glBindTexture(GL_TEXTURE_2D, spritetexture[0]);
		//glzDirectSpriteRender(viewport[2] / 2, -viewport[3] / 2, 2, 64.0, 64.0, 0, 0, 1.0, 1.0, glzOrigin::BOTTOM_RIGHT);
		glzDirectSpriteRenderAtlas(viewport[2] / 2, -viewport[3] / 2, 1, 64, 64, tiles_width, tiles_height, (cursprite_y* tiles_width) + (tiles_width-cursprite_x-1), glzOrigin::BOTTOM_RIGHT);

		glEnable(GL_DEPTH_TEST);


	}



	glUseProgram(0);
	glFlush ();													// Flush The GL Rendering Pipeline
}
