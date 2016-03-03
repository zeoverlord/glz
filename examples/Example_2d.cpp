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



#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>
#include "zeobase2.h"
#include "..\glz\appbase.h"
#include <fstream>
#include <math.h>
#include <string>
#include "..\glz\3d\geo.h"
#include "..\glz\shader\shader.h"
#include "..\glz\utilities\glz.h"
#include "..\glz\utilities\vectormath.h"
#include "..\glz\utilities\color.h"
#include "..\glz\image\tex.h"
#include "..\glz\2d\geo-2d.h"
#include "..\glz\3d\geo-generate.h"
#include "..\glz\effects\particle.h"
#include "..\glz\2d\2d-graph.h"
#include "..\glz\2d\2d-utilities.h"
#include "..\glz\utilities\resourcemanager.h"
#include "..\glz\input\input.h"

using namespace std;										

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors


GL_Window*	g_window;
//Keys*		g_keys;

// User Defined Variables
float		angle=0,width,height;												// Used To Rotate The Triangles
unsigned int vao[16],vao_num[16];
glzMatrix m;


string tbuffer;
string tbuffer2;
float spriteframetimer=0;
int spriteframe=0;

int gamestate=1;

float aspect = 1.0f;

glzCamera2D cam;
Object2DGraph tempgraph(&cam);
//Object2DGraph tempgraph2(&cam);
node3 n;

glztiles tilemap;
glztiles tilemap2;


GLhandleARB  ProgramObject, ProgramObjectFT, ProgramObjectFSQ, ProgramObjectFSQ_glitch;
//texture_transform text_tt;

glzSimpleParticleSystem ps;

static PFNGLUSEPROGRAMPROC						glUseProgram;
static PFNGLUNIFORM1IPROC                       glUniform1i;
static PFNGLUNIFORMMATRIX4FVPROC                glUniformMatrix4fv;
static PFNGLUNIFORM4FARBPROC                    glUniform4f;
static PFNGLGETUNIFORMLOCATIONPROC              glGetUniformLocation;
static PFNGLBLENDCOLORPROC						glBlendColor;
static PFNGLACTIVETEXTUREPROC					glActiveTexture;


#define COL_BLACK	glzColor(0.0f, 0.0f, 0.0f, 1.0f)
#define COL_WHITE	glzColor(1.0f, 1.0f, 1.0f, 1.0f)
#define COL_RED		glzColor(1.0f, 0.0f, 0.0f, 1.0f)
#define COL_GREEN	glzColor(0.0f, 1.0f, 0.0f, 1.0f)
#define COL_BLUE	glzColor(0.0f, 0.0f, 1.0f, 1.0f)


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

BOOL Initialize (GL_Window* window)					// Any GL Init Code & User Initialiazation Goes Here
{
	g_window	= window;
	//g_keys		= keys;

	glzResourcemanager rm;

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

	aspect = (float)window->init.width / (float)window->init.height;
	




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

	mo.LoadIdentity();
	mg.LoadIdentity();
	mh.LoadIdentity();	
	
	//glzTranslatef(mt,-3.9f,1.9f,0);
	mg.translate(0,0,0);	

	mg.scale(32.0f, 32.0f, 32.0f);
	mh.scale(0.4f, 0.4f, 0.4f);


	mh.rotate(90,1.0f,0.0f,0.0f);

	mh.translate(-8.0,8.0,0.0);

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

	rm.createTexture("default", "data\\derpy_phirana.tga", glzTexFilter::LINEAR);
	//fonts
	rm.createTexture("font.arial", "data\\fonts\\arial.tga", glzTexFilter::LINEAR,2);
	rm.createTexture("font.minya_m", "data\\fonts\\minya_m.tga", glzTexFilter::LINEAR);
	rm.createTexture("font.ms_gothic", "data\\fonts\\ms_gothic.tga", glzTexFilter::LINEAR);
	rm.createTexture("font.digitalstrip_l", "data\\fonts\\digitalstrip_l.tga", glzTexFilter::LINEAR);
	rm.createTexture("font.morpheus_l", "data\\fonts\\morpheus_l.tga", glzTexFilter::LINEAR);
	//backgrounds
	rm.createTexture("background.back", "data\\back.tga", glzTexFilter::LINEAR);
	rm.createTexture("background.cv90", "data\\cv90-1080p-04.tga", glzTexFilter::LINEAR,1);
	//sprites
	rm.createTexture("sprite.derpy_phirana", "data\\derpy_phirana.tga", glzTexFilter::LINEAR,2);
	rm.createTexture("sprite.explotion128a", "data\\explotion128a.tga", glzTexFilter::NEAREST,3);
	rm.createTexture("sprite.blob", "data\\blob.tga", glzTexFilter::NEAREST);
	//tilesets
	rm.createTexture("atlas.tinytiles", "data\\tinytiles.tga", glzTexFilter::NEAREST);
	rm.createTexture("atlas.tileset", "data\\tileset.tga", glzTexFilter::NEAREST);



	
	//unsigned int tx;
	//texturecontainer *txx;
	//rm.createTexture("default", "data\\cv90-1080p-04.tga", glzTexFilter::LINEAR);
	//rm.load_all();
	rm.load_all();
	//rm.manipulate("default");





	cam.setsize(800, 500);
//	cam.moveTo(vert3(0.0,100.0, 0.0));
	cam.moveSpeed(100);


	n.rs.rotate(20, 0.0, 0.0, 1.0);

	n.pos = vert3(200.0,-130.0,0.0);
	
	

	tempgraph.add(obj2d_Clear());

	tempgraph.add(obj2d_Fullscreen(-1, rm.gettexture("background.cv90")));
	//tempgraph.add(obj2d_Fullscreen(-1, txx->handle));

	// load the tilemap
	tilemap.load("data\\supertiles1.tga", glzTileType::DOUBLE_LAYER);
	tilemap2.load("data\\supertiles2.tga", glzTileType::DOUBLE_LAYER);

	tempgraph.add(obj2d_Tiles(42, &tilemap, 0, 16, 16, 1.0, nullptr, node3(vert3(-512, -256.0, 0.0)), rm.gettexture("atlas.tileset"), 128, 1.0f));
	tempgraph.add(obj2d_Tiles(42, &tilemap, 1, 16, 16, 1.0, nullptr, node3(vert3(-512, -256.0, 0.0)), rm.gettexture("atlas.tileset"), 128, 1.0f));
	tempgraph.add(obj2d_Tiles(42, &tilemap2, 0, 16, 16, 1.0, nullptr, node3(vert3(-512, -256.0, 0.0)), rm.gettexture("atlas.tileset"), 128, 1.0f));
	tempgraph.add(obj2d_Tiles(42, &tilemap2, 1, 16, 16, 1.0, nullptr, node3(vert3(-512, -256.0, 0.0)), rm.gettexture("atlas.tileset"), 128, 1.0f));

	//tempgraph.set(42, glzOBject2DSetvar::SPRITE, glzSprite(2, 2, 2, 0.0));

	

	


	tempgraph.add(obj2d_Sprite(343, glzSprite(8, 4, 16, 0.0), nullptr, node3(), rm.gettexture("sprite.explotion128a"), 1.0));
	

	//tempgraph.add(obj2d_Sprite(62, glzSprite(), &n, node3(), texture[1], 1.0));
	tempgraph.add(obj2d_Sprite(62, glzSprite(), &n, node3(), rm.gettexture("default"), 1.0));
	

	glzSpriteList expl_spritelist;
	glzSpriteanimationList expl_spritelist_vector;


	expl_spritelist_vector.map.push_back(glzSpriteList(8,4));
	

	int v[] = { 31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 };
	expl_spritelist_vector.map.push_back(glzSpriteList(8, 4, v, 32));




	tempgraph.add(obj2d_Sprite(111, expl_spritelist_vector, nullptr, node3(vert3(100.0, 0.0, 0.0)), rm.gettexture("sprite.explotion128a"), 1.0, 30.0f));

	tempgraph.set(343, glzOBject2DSetvar::SCALE, 2.0f);
	tempgraph.set(111, glzOBject2DSetvar::BLEND, glzBlendingMode::ADDITIVE);
	tempgraph.set(111, glzOBject2DSetvar::CURRENT_ANIMATION, 0);
	tempgraph.set(111, glzOBject2DSetvar::ANIMATIONSTOP);
	tempgraph.set(343, glzOBject2DSetvar::Z_LEVEL, 2.0f);
//	tempgraph.sort_z = true;

	tempgraph.set(62, glzOBject2DSetvar::ORIGIN, glzOrigin::CENTERED);

//	tempgraph2.add(obj2d_Clear(66, glzColor(0.5, 0.0, 0.0, 0.5)));
	//tempgraph.add(obj2d_Clear(66,glzColor(0.0,0.0,0.0,0.5)));
//	tempgraph.add(obj2d_Object2DGraph(67, &tempgraph2));
	
//	tempgraph.add(obj2d_Background(-1, expl_spritelist_vector, glzBlendingMode::ADDITIVE, 200.0, 30.0f, 1.0, texture[2]));


	//tempgraph.add(obj2d_ColorTint(66, glzBlendingMode::MULTIPLY, glzColor(1.0, 0.2, 0.0, 1.0)));

	tempgraph.add(obj2d_Text(99, "testing text", nullptr, node3(vert3(-400.0, -250.0, 0.0)), rm.gettexture("font.arial"), 40.0, 1.0, 1.0, glzOrigin::BOTTOM_LEFT));
	tempgraph.set(99, glzOBject2DSetvar::BLENDCOLOR, glzColor(1.0,0.5,0.0,1.0));
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
	glzInput input;
	WINDOW_HEIGHT = app.data.WINDOW_HEIGHT;
	WINDOW_WIDTH = app.data.WINDOW_WIDTH;
		glzMatrix mt;
		mt.LoadIdentity();
		mt.scale(0.17f,0.17f,0.17f);


		glzResourcemanager rm;
		rm.load_one();



	if (input.getKeyState(VK_ESCAPE) == TRUE)					// Is ESC Being Pressed?
	{
		TerminateApplication (g_window);						// Terminate The Program
	}

	if (input.getKeyState(VK_F1) == TRUE)						// Is F1 Being Pressed?
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

		tbuffer = string("fast changing text:") + to_string(testc);


		if (input.getPulsar1())
		{
			tbuffer2 = string("slow changing text:") + to_string(testc);		
		}

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



	if (input.getKeyState(VK_SPACE))
	{	
		ps.spawn_burst((int)(1000*seconds), vert3(0.0f, 100.0f, 0.0f), 0.1f, 2.0f, 1.0f, 0.5f, 3.0f, 0.3f, 1.0f, 1.0f);
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


	if (input.getKeyState('A') == TRUE) cam.moveToRel(vert3(seconds * -100, 0.0, 0.0));
	if (input.getKeyState('D') == TRUE) cam.moveToRel(vert3(seconds * 100, 0.0, 0.0));
	if (input.getKeyState('W') == TRUE) cam.moveToRel(vert3(0.0, seconds * 100, 0.0));
	if (input.getKeyState('S') == TRUE) cam.moveToRel(vert3(0.0, seconds * -100, 0.0));
	if (input.getKeyState(VK_SPACE) == TRUE) { cam.zoomTo(5.0f); }
	else cam.zoomTo(1.0f);

	if (input.getKeyState('P') == TRUE) tempgraph.set(111, glzOBject2DSetvar::ANIMATIONPLAYONCE);

	cam.update(seconds);
}
	n.tick(seconds);
	tempgraph.update(seconds);

	if (input.getKeyState('1') == TRUE) gamestate = 1;
	if (input.getKeyState('2') == TRUE) gamestate = 2;
	if (input.getKeyState('3') == TRUE) gamestate = 3;
	if (input.getKeyState('4') == TRUE) gamestate = 4;
	if (input.getKeyState('5') == TRUE) gamestate = 5;
	if (input.getKeyState('6') == TRUE) gamestate = 6;
	if (input.getKeyState('7') == TRUE) gamestate = 7;
	if (input.getKeyState('8') == TRUE) gamestate = 8;
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
	
	glzResourcemanager rm;

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
		glzDrawText("Normal text", -0.8f, 0.49f, 0.05f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE);
		glzDrawText("Tabs and endlines work just as well\nTab\t\t\t\\t\nEndline \t\t\\n", -0.8f, 0.42f, 0.05f, 1.0f, aspect, rm.gettexture("font.arial"), COL_WHITE);
		glzDrawText(tbuffer, -0.8f, 0.25f, 0.05f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE);
		glzDrawText(tbuffer2, -0.8f, 0.19f, 0.05f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE);
		glzDrawText("You can also play with the kerning if you want", -0.8f, 0.07f, 0.05f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE);
		glzDrawText("Using", -0.8f, -0.03f, 0.05f, 1.0f, aspect, rm.gettexture("font.minya_m"), COL_WHITE);
		glzDrawText("all kinds", -0.8f, -0.1f, 0.05f, 1.0f, aspect, rm.gettexture("font.digitalstrip_l"), COL_WHITE);
		glzDrawText("of fonts", -0.8f, -0.17f, 0.05f, 1.0f, aspect, rm.gettexture("font.morpheus_l"), COL_WHITE);
		glzDrawText("Switch screens with 1, 2, 3, 4, 5, 6", 0.8f, -0.5f, 0.04f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE, glzOrigin::BOTTOM_RIGHT);
	}




	if (gamestate==2)
	{
		glzBackdrop(rm.gettexture("background.back"), glzBlendingMode::NONE);

		glzMatrix mi;
		mi.LoadIdentity();
		mi.scale(0.27f*0.8f, 0.27f, 0.27f);
		mi.translate(-2.7f, -2.7f, 0.0f);
		glzColor blendcolor(1.0, 1.0, 1.0, 1.0);

		glzDrawSprite(rm.gettexture("sprite.derpy_phirana"), glzBlendingMode::ALPHA, -0.5f, -0.5f, 0.5f, aspect);
		

		glzDrawText("Full screen quads", -0.8f, 0.49f, 0.05f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_BLACK);
		glzDrawText("Switch screens with 1, 2, 3, 4, 5, 6", 0.8f, -0.5f, 0.04f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_BLACK, glzOrigin::BOTTOM_RIGHT);

	
	}	


	if (gamestate==3)
	{	
		
		glzColor col(1.0,1.0,1.0,1.0);
	//	draw_sprite(0, 0, 2.0f, 1, rm.gettextureHandle("sprite.explotion128a"), spriteframe, ProgramObject, col);

		glzDrawSprite(rm.gettexture("sprite.explotion128a"), glzSprite(8, 4, spriteframe, 0.0f), glzBlendingMode::ALPHA, -0.0f, -0.0f, 2.0f, aspect);
		
		glzDrawText("Sprites", -0.8f, 0.49f, 0.05f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE);
		glzDrawText("Switch screens with 1, 2, 3, 4, 5, 6", 0.8f, -0.5f, 0.04f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE, glzOrigin::BOTTOM_RIGHT);

	}

	if (gamestate==4)
	{	
		
	m.LoadIdentity();
	m.ortho(-400, 400, -250, 250, -100, 100);
	m.translate(-250,150,0);

	
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);
	
	glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("atlas.tinytiles"));
	glzDrawVAO(vao_num[2],vao[2],GL_TRIANGLES);

	glzDrawText("Atlas grid", -0.8f, 0.49f, 0.05f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE);
	glzDrawText("Switch screens with 1, 2, 3, 4, 5, 6", 0.8f, -0.5f, 0.04f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE, glzOrigin::BOTTOM_RIGHT);


	}

	if (gamestate == 5)
	{
		glDisable(GL_DEPTH_TEST);
		m.LoadIdentity();
		m.ortho(-400, 400, -250, 250, -100, 100);


		m.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

		glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("sprite.derpy_phirana"));
	
		glzDirectSpriteRender(m, rm.gettextureHandle("sprite.derpy_phirana"), 0, 0, 0, 100, 100, 1, 0, 1.0, 1.0, glzOrigin::TOP_LEFT);
		glzDirectSpriteRender(m, rm.gettextureHandle("sprite.derpy_phirana"), 0, 0, 0, 100, 100, 1, 0, 1.0, 1.0, glzOrigin::BOTTOM_LEFT);
		
		glzDirectSpriteRender(m, rm.gettextureHandle("sprite.derpy_phirana"), 0, 0, 0, 100, 100, 1, 0, 1.0, 1.0, glzOrigin::BOTTOM_RIGHT);
		glzDirectSpriteRender(m, rm.gettextureHandle("sprite.derpy_phirana"), 0, 0, 0, 100, 100, 1, 0, 1.0, 1.0, glzOrigin::TOP_RIGHT);


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

		glzDrawTexture(rm.gettextureHandle("atlas.tinytiles"), 0, 0, 0, 200, 200, 0, 0, 0, 1, 1);

		glDisable(GL_BLEND);

		glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("atlas.tinytiles"));

	
		glzDirectSpriteRenderAtlas(0, 0, 0, 100, 100, 4, 4, 14, glzOrigin::CENTERED);
	glUniform4f(loc3, 1.0f, 0.0f, 1.0f, 1.0f);
		glzDirectSpriteRenderAtlasPixelPerfect(192, 192, 1, 64, 64, 4, 4, 1, glzOrigin::BOTTOM_LEFT);
		glzDirectSpriteRenderAtlasPixelPerfectQuantized(208, 192, 1, 64, 64, 4, 4, 1, 16.0f, glzOrigin::BOTTOM_LEFT);		
			
		glzDrawText("Direct draw sprites in various modes", -0.8f, 0.49f, 0.05f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE);
		glzDrawText("Switch screens with 1, 2, 3, 4, 5, 6", 0.8f, -0.5f, 0.04f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE, glzOrigin::BOTTOM_RIGHT);
		
		glEnable(GL_DEPTH_TEST);

	}

	if (gamestate == 6)
	{
		glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("background.back"));


		glPointSize(3.0f);
		ps.render_out();

		glzDrawText("Simple 2D particle system, easy to use, not hard to master",-0.8f, 0.49f, 0.05f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE);
		glzDrawText("Switch screens with 1, 2, 3, 4, 5, 6", 0.8f, -0.5f, 0.04f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE, glzOrigin::BOTTOM_RIGHT);

	}


	if (gamestate == 7)
	{
	
		draw_backdrop_glitch(rm.gettextureHandle("background.cv90"), rm.gettextureHandle("sprite.blob"));

		glzDrawText("Switch screens with 1, 2, 3, 4, 5, 6", 0.8f, -0.5f, 0.04f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE, glzOrigin::BOTTOM_RIGHT);

	}

	if (gamestate == 8)
	{
	/*	glzMatrix mt;
		mt.LoadIdentity();

		mt.transferMatrix(&mtemp[0]);
		glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_BLEND);*/
		tempgraph.draw();
	//	glDisable(GL_BLEND);

		glzDrawText("Switch screens with 1, 2, 3, 4, 5, 6", 0.8f, -0.5f, 0.04f, 1.0f, aspect, rm.gettexture("font.ms_gothic"), COL_WHITE, glzOrigin::BOTTOM_RIGHT);



	}


	glUseProgram(0);
	glFlush ();													// Flush The GL Rendering Pipeline
}
