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
#include "..\glz\ztool_appbase.h"
#include <fstream>
#include <math.h>
#include "..\glz\ztool-geo.h"
#include "..\glz\ztool-shader.h"
#include "..\glz\ztool-glz.h"
#include "..\glz\ztool-vectormath.h"
#include "..\glz\ztool-tex.h"
#include "..\glz\ztool-geo-2d.h"
#include "..\glz\ztool-geo-generate.h"
#include "..\glz\ztool-particle.h"
#include "..\glz\ztool-2d-graph.h"

using namespace std;										

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors


GL_Window*	g_window;
Keys*		g_keys;

// User Defined Variables
float		angle=0,width,height;												// Used To Rotate The Triangles
unsigned int vao[16],vao_num[16],textvao[16],textvao_num[16];
glzMatrix m;
unsigned int texture[15],fonttexture[15];


char tbuffer[160];
char tbuffer2[160];
float texttimer=0;
float spriteframetimer=0;
int spriteframe=0;

int gamestate=8;


glzCamera2D cam;
Object2DGraph tempgraph(&cam);
//Object2DGraph tempgraph2(&cam);
node3 n;

glztiles tilemap;
glztiles tilemap2;


GLhandleARB  ProgramObject, ProgramObjectFT, ProgramObjectFSQ, ProgramObjectFSQ_glitch;
texture_transform text_tt;

glzSimpleParticleSystem ps;

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
	app.set_title(L"ZeoBase GL Framework");	
	//app.data.START_WINDOWED = false;
	//app.data.SHOW_FRAME = false;
	app.data.ALLOW_RESIZE = true;
	WINDOW_HEIGHT = app.data.WINDOW_HEIGHT;
	WINDOW_WIDTH = app.data.WINDOW_WIDTH;
}

BOOL Initialize (GL_Window* window, Keys* keys)					// Any GL Init Code & User Initialiazation Goes Here
{
	g_window	= window;
	g_keys		= keys;

	GetFocus();
	GetAsyncKeyState(WM_KEYUP);

	// Start Of User Initialization
	angle		= 0.0f;											// Set Starting Angle To Zero

	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glFrontFace(GL_CCW);



	glUseProgram				= (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
	glGetUniformLocation		= (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress("glGetUniformLocation");                                                                     
	glUniform1i= (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
	glUniform4f= (PFNGLUNIFORM4FARBPROC) wglGetProcAddress("glUniform4fARB");
	glUniformMatrix4fv= (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");

	glBlendColor = (PFNGLBLENDCOLORPROC)wglGetProcAddress("glBlendColor");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

	




	glzMatrix mt;
	glzMatrix mt2;
	glzMatrix mt3;
	glzMatrix mo;
	glzMatrix mg;
	glzMatrix mh;

	unsigned int ad[64]={2,2,2,1,2,2,2,2,
						 2,2,4,1,4,2,2,2,
						 3,3,0,1,0,3,3,3,
						 3,3,0,1,0,3,3,3,
						 2,2,4,1,4,2,2,2,
						 2,2,2,1,2,2,2,2,
						 2,2,2,1,1,1,1,1,
						 2,2,2,2,2,2,2,2};





	mt.LoadIdentity();
	mt2.LoadIdentity();
	mt3.LoadIdentity();

	mo.LoadIdentity();
	mg.LoadIdentity();
	mh.LoadIdentity();


	
	
	//glzTranslatef(mt,-3.9f,1.9f,0);
	mg.translate(0,0,0);	

	mt.scale(0.17f,0.17f,0.17f);
	mt2.scale(0.3f, 0.3f, 0.3f);
	mt3.scale(0.13f, 0.13f, 0.13f);
	mg.scale(32.0f, 32.0f, 32.0f);
	mh.scale(0.4f, 0.4f, 0.4f);


	mh.rotate(90,1.0f,0.0f,0.0f);

	mh.translate(-8.0,8.0,0.0);

	// if orientation is messed up on the tga then do this, but generally tga files have their origin set to glzOrigin::BOTTOM_LEFT
	img_head img;
	glzReadTgaHead(&img, "data\\fonts\\arial.tga");
	text_tt = glzMakeTTAtlas(16, 16, 0, img.origin);



// text screen
	textvao_num[0] = glzVAOMakeText("Normal text", mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[0]);
	textvao_num[1] = glzVAOMakeText("Tabs and endlines work just as well\nTab\t\t\t\\t\nEndline \t\t\\n", mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[1]);
	textvao_num[2] = glzVAOMakeText("fast changing text:0000", mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[2]);
	textvao_num[3] = glzVAOMakeText("slow changing text:0000", mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[3]);
	textvao_num[4] = glzVAOMakeText("You can also play with the kerning if you want", mt, 0.7f, text_tt, glzOrigin::TOP_LEFT, &textvao[4]);
	textvao_num[5] = glzVAOMakeText("Using", mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[5]);
	textvao_num[6] = glzVAOMakeText("all kinds", mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[6]);
	textvao_num[7] = glzVAOMakeText("of fonts", mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[7]);
	textvao_num[8] = glzVAOMakeText("Fancy text is fancy", mt2, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[8]);

// fsq screen

	textvao_num[9] = glzVAOMakeText("Full screen quads", mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[9]);
	
// Sprite screen

	textvao_num[10] = glzVAOMakeText("Sprites", mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[10]);

// grid screen

	textvao_num[11] = glzVAOMakeText("Atlas grid", mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[11]);

// sprite screen

	textvao_num[12] = glzVAOMakeText("Direct draw sprites in various modes", mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[12]);

	// sprite screen

	textvao_num[13] = glzVAOMakeText("Simple 2D particle system, easy to use, not hard to master", mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[13]);



// all screens
	textvao_num[15] = glzVAOMakeText("Switch screens with 1, 2, 3, 4, 5, 6", mt3, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[15]);






	primitive_gen prim_fsq_pg[1];	
	prim_fsq_pg[0] = glzMakePGDefault(glzPrimitive::FSQ);
	prim_fsq_pg[0].tt=glzMakeTTDefault();

	vao_num[0]=glzVAOMakePrimitive(prim_fsq_pg[0], &vao[0]); // change the first argument to 2 for an extra object, this is subject to some major redecorating

	
	primitive_gen prim_sprite1_pg[1];	
	prim_sprite1_pg[0] = glzMakePGDefault(glzPrimitive::SPRITE_ATLAS_ARRAY);
	prim_sprite1_pg[0].tt = glzMakeTTAtlas(8, 4, 0, glzOrigin::BOTTOM_LEFT);
	
	vao_num[1]=glzVAOMakePrimitive(prim_sprite1_pg[0], &vao[1]); // change the first argument to 2 for an extra object, this is subject to some major redecorating


	texture_transform grid_tt = glzMakeTTAtlasArray(4, 4, ad, glzOrigin::BOTTOM_LEFT);
	vao_num[2]=glzVAOMakeAtlasGrid(8, 8, mg, grid_tt, &vao[2]);




	ProgramObject = glzShaderLoad("data\\glsl.vert", "data\\glsl.frag", glzVAOType::AUTO);
	ProgramObjectFT = glzShaderLoad("data\\fancytext.vert", "data\\fancytext.frag", glzVAOType::AUTO);
	ProgramObjectFSQ = glzShaderLoad("data\\fsq.vert", "data\\fsq.frag", glzVAOType::AUTO);
	ProgramObjectFSQ_glitch = glzShaderLoad("data\\fsq_gltch.vert", "data\\fsq_gltch.frag", glzVAOType::AUTO);

	
	glzShaderLink(ProgramObject);
	glzShaderLink(ProgramObjectFT);
	glzShaderLink(ProgramObjectFSQ);
	glzShaderLink(ProgramObjectFSQ_glitch);
	// load the textures
	fonttexture[0] = glzLoadTexture("data\\fonts\\arial.tga", glzTexFilter::LINEAR);
	fonttexture[1] = glzLoadTexture("data\\fonts\\minya_m.tga", glzTexFilter::LINEAR);
	fonttexture[2] = glzLoadTexture("data\\fonts\\ms_gothic.tga", glzTexFilter::LINEAR);
	fonttexture[3] = glzLoadTexture("data\\fonts\\digitalstrip_l.tga", glzTexFilter::LINEAR);
	fonttexture[4] = glzLoadTexture("data\\fonts\\morpheus_l.tga", glzTexFilter::LINEAR);

	texture[0] = glzLoadTexture("data\\back.tga", glzTexFilter::LINEAR);
	texture[1] = glzLoadTexture("data\\derpy_phirana.tga", glzTexFilter::LINEAR);  // the derpy phirana is not an actual logo but just an example on how you can put it there
	texture[2] = glzLoadTexture("data\\explotion128a.tga", glzTexFilter::NEAREST);
	texture[3] = glzLoadTexture("data\\tinytiles.tga", glzTexFilter::NEAREST);
	texture[4] = glzLoadTexture("data\\blob.tga", glzTexFilter::NEAREST);
	texture[5] = glzLoadTexture("data\\cv90-1080p-04.tga", glzTexFilter::LINEAR);

	texture[6] = glzLoadTexture("data\\tileset.tga", glzTexFilter::NEAREST); // sprite layers


	
	cam.setsize(800, 500);
//	cam.moveTo(vert3(0.0,100.0, 0.0));
	cam.moveSpeed(100);


	n.rs.rotate(20, 0.0, 0.0, 1.0);

	n.pos = vert3(200.0,-130.0,0.0);
	
	

	tempgraph.add(obj2d_Clear());

	tempgraph.add(obj2d_Fullscreen(-1, texture[5]));

	// load the tilemap
	tilemap.load("data\\supertiles1.tga", glzTileType::DOUBLE_LAYER);
	tilemap2.load("data\\supertiles2.tga", glzTileType::DOUBLE_LAYER);

	tempgraph.add(obj2d_Tiles(42, &tilemap, 0, 16, 16, 1.0, nullptr, node3(vert3(-512, -256.0, 0.0)), texture[6], 128, 1.0f));
	tempgraph.add(obj2d_Tiles(42, &tilemap, 1, 16, 16, 1.0, nullptr, node3(vert3(-512, -256.0, 0.0)), texture[6], 128, 1.0f));
	tempgraph.add(obj2d_Tiles(42, &tilemap2, 0, 16, 16, 1.0, nullptr, node3(vert3(-512, -256.0, 0.0)), texture[6], 128, 1.0f));
	tempgraph.add(obj2d_Tiles(42, &tilemap2, 1, 16, 16, 1.0, nullptr, node3(vert3(-512, -256.0, 0.0)), texture[6], 128, 1.0f));




	tempgraph.add(obj2d_Sprite(343, glzSprite(8, 4, 16, 0.0), nullptr, node3(), texture[2], 1.0));
	tempgraph.add(obj2d_Sprite(-1, glzSprite(), &n, node3(), texture[1], 1.0));


	glzSpriteList expl_spritelist;
	glzSpriteanimationList expl_spritelist_vector;


	expl_spritelist_vector.map.push_back(glzSpriteList(8,4));
	

	int v[] = { 31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 };
	expl_spritelist_vector.map.push_back(glzSpriteList(8, 4, v, 32));

	tempgraph.add(obj2d_Sprite(111, expl_spritelist_vector, nullptr, node3(vert3(100.0, 0.0, 0.0)), texture[2], 1.0, 30.0f));

	tempgraph.set(343, glzOBject2DSetvar::SCALE, 2.0f);
	tempgraph.set(111, glzOBject2DSetvar::BLEND, glzBlendingMode::ADDITIVE);
	tempgraph.set(111, glzOBject2DSetvar::CURRENT_ANIMATION, 0);
	tempgraph.set(111, glzOBject2DSetvar::ANIMATIONSTOP);
	tempgraph.set(343, glzOBject2DSetvar::Z_LEVEL, 2.0f);
//	tempgraph.sort_z = true;
	

//	tempgraph2.add(obj2d_Clear(66, glzColor(0.5, 0.0, 0.0, 0.5)));
	//tempgraph.add(obj2d_Clear(66,glzColor(0.0,0.0,0.0,0.5)));
//	tempgraph.add(obj2d_Object2DGraph(67, &tempgraph2));
	
	tempgraph.add(obj2d_Background(-1, expl_spritelist_vector, glzBlendingMode::ADDITIVE, 2.0, 30.0f, 1.0, texture[2]));
	
		



	return TRUE;												// Return TRUE (Initialization Successful)
}


void Deinitialize (void)										// Any User DeInitialization Goes Here
{

	// this shouldn't normally be nessecary, but it's better to make it a habit to delete data for when you start to load and unload resources mid game.

	glzKillAllVAO();		// deletes all vaos in existance


}

void Update (float seconds)								// Perform Motion Updates Here
{

	glzAppinitialization app;
	WINDOW_HEIGHT = app.data.WINDOW_HEIGHT;
	WINDOW_WIDTH = app.data.WINDOW_WIDTH;
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
	
	angle += seconds*10;						// Update angle Based On The Clock
		
	if (angle>360) angle=0;



	if (gamestate==1)
	{



		vert3 testa(2.0, 2.0, 2.0);
		vert3 testb(sin(angle*PI_OVER_180), 2.0, 2.0);

		float testc = 0.0f;
		testc = testb.distance(testa);

		
		sprintf_s(tbuffer, 160, "fast changing text:%f", testc);
		textvao_num[2] = glzVAOMakeText(tbuffer, mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[2]);  // this updates text once every frame

		if (texttimer>1.0)
		{
			sprintf_s(tbuffer2, 160, "slow changing text:%f", testc);
			textvao_num[3] = glzVAOMakeText(tbuffer2, mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[3]);  // this updates text once every second
			texttimer=0.0f;
		}
		texttimer+=seconds;

	}


if (gamestate==3)
	{
	
		spriteframetimer+=seconds;
		spriteframe+=glzTimeCounter(&spriteframetimer, 0.05f);  // this runs the explotion animation
		spriteframe=spriteframe%32;
	}

if (gamestate == 6)
{
	ps.set_clamp(false, false, false);



	if (g_keys->keyDown[VK_SPACE] == TRUE)
	{
	
		ps.spawn_burst(1000*seconds, vert3(0.0f, 100.0f, 0.0f), 0.1f, 2.0f, 1.0f, 0.5f, 3.0f, 0.3f, 1.0, 1.0f);
		//ps.spawn_line_burst(1000*seconds, vert3(0.0f, 100.0f, 0.0f),600.0, 0.1f, 2.0f, 1.0f, 0.5f, 2.2f, 0.2f, 1.0, 1.0f);
		
}
	vec3 ctemp(0.0,1.0,0.0);
	ctemp.normalize(1.0);
	vec3 ctemp2(-0.2, 1.0, 0.0);
	ctemp2.normalize(1.0);

	vec3 gv(0.0f, -9.82f, 0.0f);

	ps.set_time(seconds);

	ps.singularity(vert3(-100.0, -50.0, 0.0), 100.0, 5.0);
	ps.singularity(vert3(-100.0, 50.0, 0.0), 100.0, 10.0);
	ps.noise(10.0);
	ps.drag(1.0);
	ps.terminal_velocity(1000.0);
	ps.gravity(gv);
	ps.collide_plane(ctemp, vert3(0.0, -100.0, 0.0));

	//ps.collide_plane(ctemp2, vert3(0.0, -100.0, 0.0));
	//ps.collide_plane_y(-100, true);
	ps.tick();

}

if (gamestate == 8)
{


	if (g_keys->keyDown['A'] == TRUE) cam.moveToRel(vert3(seconds * -100, 0.0, 0.0));
	if (g_keys->keyDown['D'] == TRUE) cam.moveToRel(vert3(seconds * 100, 0.0, 0.0));
	if (g_keys->keyDown['W'] == TRUE) cam.moveToRel(vert3(0.0, seconds * 100, 0.0));
	if (g_keys->keyDown['S'] == TRUE) cam.moveToRel(vert3(0.0, seconds * -100, 0.0));
	if (g_keys->keyDown[VK_SPACE] == TRUE) { cam.zoomTo(5.0f); 	}
	else cam.zoomTo(1.0f);

	if (g_keys->keyDown['P'] == TRUE) tempgraph.set(111, glzOBject2DSetvar::ANIMATIONPLAYONCE);

	cam.update(seconds);
}



	n.tick(seconds);
	tempgraph.update(seconds);

	if (g_keys->keyDown['1'] == TRUE) gamestate = 1;
	if (g_keys->keyDown['2'] == TRUE) gamestate = 2;
	if (g_keys->keyDown['3'] == TRUE) gamestate = 3;
	if (g_keys->keyDown['4'] == TRUE) gamestate = 4;
	if (g_keys->keyDown['5'] == TRUE) gamestate = 5;
	if (g_keys->keyDown['6'] == TRUE) gamestate = 6;
	if (g_keys->keyDown['7'] == TRUE) gamestate = 7;
	if (g_keys->keyDown['8'] == TRUE) gamestate = 8;

	

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

//glzShaderUsePasstrough();
	

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

	glBindTexture(GL_TEXTURE_2D,texture[tx]);
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
	unsigned int loc1 = glGetUniformLocation(ProgramObjectFSQ, "projMat");
	unsigned int loc2 = glGetUniformLocation(ProgramObjectFSQ, "texunit0");
	unsigned int loc3 = glGetUniformLocation(ProgramObjectFSQ, "tint");



	float mtemp[16];
	mat.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

	glUniform1i(loc2, 0);
	glUniform4f(loc3, col[0], col[1], col[2], col[3]);
	glBindTexture(GL_TEXTURE_2D, bgtexture);
	glzDrawVAO(vao_num[0], vao[0], GL_TRIANGLES);

}

void draw_backdrop_glitch(unsigned int bgtexture, unsigned int bgtexture2)
{
	glUseProgram(ProgramObjectFSQ_glitch);
	unsigned int loc1 = glGetUniformLocation(ProgramObjectFSQ_glitch, "projMat");
	unsigned int loc2 = glGetUniformLocation(ProgramObjectFSQ_glitch, "texunit0");
	unsigned int loc3 = glGetUniformLocation(ProgramObjectFSQ_glitch, "texunit1");
	unsigned int loc4 = glGetUniformLocation(ProgramObjectFSQ_glitch, "tint");
	unsigned int loc5 = glGetUniformLocation(ProgramObjectFSQ_glitch, "width");
	unsigned int loc6 = glGetUniformLocation(ProgramObjectFSQ_glitch, "height");


	m.LoadIdentity();

	float mtemp[16];
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);
	glUniform1i(loc2, 0);
	glUniform1i(loc3, 1);
	glUniform4f(loc4, 1.0f, 1.0f, 1.0f, 1.0f);
	glUniform1i(loc5, WINDOW_WIDTH);
	glUniform1i(loc6, WINDOW_HEIGHT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bgtexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bgtexture2);
	glActiveTexture(GL_TEXTURE0);
	glzDrawVAO(vao_num[0], vao[0], GL_TRIANGLES);

}


void Draw (void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	


	float mtemp[16];
	glEnable(GL_TEXTURE_2D);
	unsigned int loc1 = glGetUniformLocation(ProgramObject,"projMat");
	unsigned int loc2 = glGetUniformLocation(ProgramObject,"texunit0");
	unsigned int loc3 = glGetUniformLocation(ProgramObject,"tint");

	unsigned int loc4 = glGetUniformLocation(ProgramObjectFT,"projMat");
	unsigned int loc5 = glGetUniformLocation(ProgramObjectFT,"texunit0");

	glUseProgram(ProgramObject);
	glUniform1i(loc2, 0);	
	glUniform4f(loc3, 1.0f,1.0f,1.0f,1.0f);

	// i have used these gamestates in a few games for LD48 now and they are really quick and dirty, but effective.
	// they allow you to quickly make a title screen and end screen at the end of the project without changing that much code, instead you just encapsulate it in a gamestate

	if (gamestate==1)  
	{
		draw_text(-3.9f, 1.9f,0,2,ProgramObject,COL_WHITE);
		draw_text(-3.9f, 1.5f,1,0,ProgramObject,COL_WHITE);
		draw_text(-3.9f, 0.8f,2,2,ProgramObject,COL_WHITE);
		draw_text(-3.9f, 0.5f,3,2,ProgramObject,COL_WHITE);
		draw_text(-3.9f, 0.2f,4,2,ProgramObject,COL_WHITE);

		draw_text(-3.9f, -0.2f,5,1,ProgramObject,COL_WHITE);
		draw_text(-3.9f, -0.4f,6,3,ProgramObject,COL_WHITE);
		draw_text(-3.9f, -0.6f,7,4,ProgramObject,COL_WHITE);

		draw_text(1.7f, -1.8f,15,2,ProgramObject,COL_WHITE);

		glUseProgram(ProgramObjectFT);
	
		glUniform1i(loc5, 0);	
		draw_text(-3.9f, -1.5f,8,0,ProgramObjectFT,COL_WHITE);
	}




	if (gamestate==2)
	{
		draw_backdrop(texture[0]);

		glzMatrix mi;
		mi.LoadIdentity();
		mi.scale(0.17f, 0.17f, 0.17f);
		float col[4]={1.0f,1.0f,1.0f,1.0f};

		mi.translate(-4.7f, -4.7f, 0.0f);


		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);	
		draw_backdrop2(texture[1],mi,col); // the derpy phirana
		glDisable(GL_BLEND);

		
		draw_text(-3.9f, 1.9f,9,2,ProgramObject,COL_BLACK);
		draw_text(1.7f, -1.8f,15,2,ProgramObject,COL_BLACK);

	
	}	


	if (gamestate==3)
	{	
		
		float col[4]={1.0,1.0,1.0,1.0};
		draw_sprite(0,0,2.0f,1,2,spriteframe,ProgramObject,col);

		draw_text(-3.9f, 1.9f,10,2,ProgramObject,COL_WHITE);
		draw_text(1.7f, -1.8f,15,2,ProgramObject,COL_WHITE);
	}

	if (gamestate==4)
	{	
		
	m.LoadIdentity();
	m.ortho(-400, 400, -250, 250, -100, 100);
	m.translate(-250,150,0);

	
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);





	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glzDrawVAO(vao_num[2],vao[2],GL_TRIANGLES);


	draw_text(-3.9f, 1.9f,11,2,ProgramObject,COL_WHITE);
	draw_text(1.7f, -1.8f,15,2,ProgramObject,COL_WHITE);


	}

	if (gamestate == 5)
	{
		glDisable(GL_DEPTH_TEST);
		m.LoadIdentity();
		m.ortho(-400, 400, -250, 250, -100, 100);


		m.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

		glBindTexture(GL_TEXTURE_2D, texture[1]);
	
		glzDirectSpriteRender(m, texture[1], 0, 0, 0, 100, 100, 1, 0, 1.0, 1.0, glzOrigin::TOP_LEFT);
		glzDirectSpriteRender(m, texture[1], 0, 0, 0, 100, 100, 1, 0, 1.0, 1.0, glzOrigin::BOTTOM_LEFT);
		
		glzDirectSpriteRender(m, texture[1], 0, 0, 0, 100, 100, 1, 0, 1.0, 1.0, glzOrigin::BOTTOM_RIGHT);
		glzDirectSpriteRender(m, texture[1], 0, 0, 0, 100, 100, 1, 0, 1.0, 1.0, glzOrigin::TOP_RIGHT);


		glzMatrix mspr;
		mspr.LoadIdentity();
		mspr.scale(100, 100, 100);

		//glzDirectSpriteRender(mspr, texture[1], glzSprite(), glzOrigin::TOP_RIGHT);


		glzMatrix mt;
		mt.LoadIdentity();

		mt.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);
		
		

		tempgraph.draw();


		m.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);


	//	glBlendFunc(GL_ONE, GL_ONE);
		glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR);
		glBlendColor(1, 0, 1, 1.0f);
		glEnable(GL_BLEND);

		glzDrawTexture(texture[3], 0, 0, 0, 200, 200, 0, 0, 0, 1, 1);

		glDisable(GL_BLEND);

		glBindTexture(GL_TEXTURE_2D, texture[3]);

	
		glzDirectSpriteRenderAtlas(0, 0, 0, 100, 100, 4, 4, 14, glzOrigin::CENTERED);
	glUniform4f(loc3, 1.0f, 0.0f, 1.0f, 1.0f);
		glzDirectSpriteRenderAtlasPixelPerfect(192, 192, 1, 64, 64, 4, 4, 1, glzOrigin::BOTTOM_LEFT);
		glzDirectSpriteRenderAtlasPixelPerfectQuantized(208, 192, 1, 64, 64, 4, 4, 1, 16.0f, glzOrigin::BOTTOM_LEFT);

		
		
		draw_text(-3.9f, 1.9f, 12, 2, ProgramObject, COL_WHITE);
		draw_text(1.7f, -1.8f, 15, 2, ProgramObject, COL_WHITE);
		
		glEnable(GL_DEPTH_TEST);

	}

	if (gamestate == 6)
	{

		m.LoadIdentity();
		m.ortho(-400, 400, -250, 250, -100, 100);


		m.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

		glBindTexture(GL_TEXTURE_2D, texture[0]);


		glPointSize(3.0f);
		ps.render_out();


		draw_text2("abcdefghijklmnopqrstuvxyz\nABCDEFGHIJKLMNOPQRSTUVXYZ\n1234567890", 1.0f,210.0f, 32.0f, 1.0f, 2, ProgramObject, COL_WHITE);


		draw_text(-3.9f, 1.9f, 13, 2, ProgramObject, COL_WHITE);
		draw_text(1.7f, -1.8f, 15, 2, ProgramObject, COL_WHITE);

	}


	if (gamestate == 7)
	{
	
		draw_backdrop_glitch(texture[5], texture[4]);

		glzMatrix mi;
		mi.LoadIdentity();
		mi.scale(0.17f, 0.17f, 0.17f);
		float col[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		mi.translate(-4.7f, -4.7f, 0.0f);


		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	//	draw_backdrop2(texture[1], mi, col); // the derpy phirana
		glDisable(GL_BLEND);


		draw_text(-3.9f, 1.9f, 9, 2, ProgramObject, COL_BLACK);
		draw_text(1.7f, -1.8f, 15, 2, ProgramObject, COL_BLACK);


	}

	if (gamestate == 8)
	{

		
		glzMatrix mt;
		mt.LoadIdentity();

		mt.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);


		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_BLEND);
		tempgraph.draw();
		glDisable(GL_BLEND);



		draw_text(-3.9f, 1.9f, 9, 2, ProgramObject, COL_BLACK);
		draw_text(1.7f, -1.8f, 15, 2, ProgramObject, COL_BLACK);


	}


	glUseProgram(0);
	glFlush ();													// Flush The GL Rendering Pipeline
}
