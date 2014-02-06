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
#include <fstream>
#include <math.h>
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

// User Defined Variables
float		angle=0,width,height;												// Used To Rotate The Triangles
unsigned int vao[16],vao_num[16],textvao[16],textvao_num[16];
float m[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int texture[5],fonttexture[15];


char tbuffer[160];
char tbuffer2[160];
float texttimer=0;
float spriteframetimer=0;
int spriteframe=0;

int gamestate=5;


GLhandleARB  ProgramObject,ProgramObjectFT,ProgramObjectFSQ;
texture_transform text_tt;



glzSimpleParticleSystem ps;

static PFNGLUSEPROGRAMPROC						glUseProgram;
static PFNGLUNIFORM1IPROC                       glUniform1i;
static PFNGLUNIFORMMATRIX4FVPROC                glUniformMatrix4fv;
static PFNGLUNIFORM4FARBPROC                    glUniform4f;
static PFNGLGETUNIFORMLOCATIONPROC              glGetUniformLocation;
static PFNGLBLENDCOLORPROC						glBlendColor;


#define COL_BLACK	0
#define COL_WHITE	1
#define COL_RED		2
#define COL_GREEN	3
#define COL_BLUE	4



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

	




	float mt[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	float mt2[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	float mt3[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	float mo[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	float mg[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	float mh[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	unsigned int ad[64]={2,2,2,1,2,2,2,2,
						 2,2,4,1,4,2,2,2,
						 3,3,0,1,0,3,3,3,
						 3,3,0,1,0,3,3,3,
						 2,2,4,1,4,2,2,2,
						 2,2,2,1,2,2,2,2,
						 2,2,2,1,1,1,1,1,
						 2,2,2,2,2,2,2,2};





	glzLoadIdentity(mt);
	glzLoadIdentity(mt2);
	glzLoadIdentity(mt3);
	glzLoadIdentity(mo);
	glzLoadIdentity(mg);
	glzLoadIdentity(mh);

	
	
	//glzTranslatef(mt,-3.9f,1.9f,0);
	glzTranslatef(mg,0,0,0);	

	glzScalef(mt,0.17f,0.17f,0.17f);
	glzScalef(mt2,0.3f,0.3f,0.3f);
	glzScalef(mt3,0.13f,0.13f,0.13f);
	glzScalef(mg,32.0f,32.0f,32.0f);
	glzScalef(mh,0.4f,0.4f,0.4f);

	glzRotatef(mh,90,1.0f,0.0f,0.0f);

	glzTranslatef(mh,-8.0,8.0,0.0);

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
	glzShaderLink(ProgramObject);
	glzShaderLink(ProgramObjectFT);
	glzShaderLink(ProgramObjectFSQ);
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

	


	return TRUE;												// Return TRUE (Initialization Successful)
}


void Deinitialize (void)										// Any User DeInitialization Goes Here
{

	// this shouldn't normally be nessecary, but it's better to make it a habit to delete data for when you start to load and unload resources mid game.

	glzKillAllVAO();		// deletes all vaos in existance


}

void Update (float seconds)								// Perform Motion Updates Here
{

		float mt[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		glzLoadIdentity(mt);
		glzScalef(mt,0.17f,0.17f,0.17f);

	if (g_keys->keyDown [VK_ESCAPE] == TRUE)					// Is ESC Being Pressed?
	{
		TerminateApplication (g_window);						// Terminate The Program
	}

	if (g_keys->keyDown [VK_F1] == TRUE)						// Is F1 Being Pressed?
	{
		ToggleFullscreen (g_window);							// Toggle Fullscreen Mode
	}

	if (gamestate==1)
	{
		angle += seconds;						// Update angle Based On The Clock
		if (angle>90001) angle=0;

		
		sprintf_s (tbuffer,160,"fast changing text:%f", angle);  
		textvao_num[2] = glzVAOMakeText(tbuffer, mt, 1.0f, text_tt, glzOrigin::TOP_LEFT, &textvao[2]);  // this updates text once every frame

		if (texttimer>1.0)
		{
			sprintf_s (tbuffer2,160,"slow changing text:%f", angle); 
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
	
		ps.spawn_burst(100, 0.0f, 0.0f, 0.0f, 0.1f, 2.0f, 3.0f, 0.5f, 3.0f, 0.3f, 1.0, 2.0f);
		
}
ps.run(seconds);

}
	

	if (g_keys->keyDown['1'] == TRUE) gamestate = 1;
	if (g_keys->keyDown['2'] == TRUE) gamestate = 2;
	if (g_keys->keyDown['3'] == TRUE) gamestate = 3;
	if (g_keys->keyDown['4'] == TRUE) gamestate = 4;
	if (g_keys->keyDown['5'] == TRUE) gamestate = 5;
	if (g_keys->keyDown['6'] == TRUE) gamestate = 6;

}


void draw_text(float x, float y, int text, int font, unsigned int po, unsigned int col)
{
	glUseProgram(po);

	unsigned int loc1 = glGetUniformLocation(po,"projMat");
	unsigned int loc2 = glGetUniformLocation(po,"texunit0");
	unsigned int loc3 = glGetUniformLocation(po,"tint");
	glzLoadIdentity(m);
	glzOrtho(m, -4, 4, -2, 2, -100, 100);
	glzTranslatef(m,x,y,0);

	glUniformMatrix4fv(loc1, 1, GL_FALSE, m);

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
	glzLoadIdentity(m);
	glzOrtho2DPixelspace(m, WINDOW_HEIGHT, WINDOW_WIDTH, glzOrigin::BOTTOM_LEFT);
	
	glzTranslatef(m, x, y, 0);

	glUniformMatrix4fv(loc1, 1, GL_FALSE, m);

	if (col == COL_BLACK)	glUniform4f(loc3, 0.0f, 0.0f, 0.0f, 1.0f);
	if (col == COL_WHITE)	glUniform4f(loc3, 1.0f, 1.0f, 1.0f, 1.0f);
	if (col == COL_RED)	glUniform4f(loc3, 1.0f, 0.0f, 0.0f, 1.0f);
	if (col == COL_GREEN)	glUniform4f(loc3, 0.0f, 1.0f, 0.0f, 1.0f);
	if (col == COL_BLUE)	glUniform4f(loc3, 0.0f, 0.0f, 1.0f, 1.0f);



	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, fonttexture[font]);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glzDirectDrawText(text, scale, kern, glzOrigin::BOTTOM_LEFT);
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
	glzLoadIdentity(m);
	glzOrtho(m, -4, 4, -2, 2, -100, 100);
	glzTranslatef(m,x,y,0);
	glzScalef(m,s,s,s);

	glUniformMatrix4fv(loc1, 1, GL_FALSE, m);

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
	
	glzLoadIdentity(m);

	glUniformMatrix4fv(loc1, 1, GL_FALSE, m);
	glUniform1i(loc2, 0);
	glUniform4f(loc3, 1.0f,1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D,bgtexture);
	glzDrawVAO(vao_num[0],vao[0],GL_TRIANGLES);

}

void draw_backdrop2(unsigned int bgtexture,float mat[16],float col[4])
{
	glUseProgram(ProgramObjectFSQ);
	unsigned int loc1 = glGetUniformLocation(ProgramObjectFSQ,"projMat");
	unsigned int loc2 = glGetUniformLocation(ProgramObjectFSQ,"texunit0");
	unsigned int loc3 = glGetUniformLocation(ProgramObjectFSQ,"tint");
	
	glzLoadIdentity(m);

	glUniformMatrix4fv(loc1, 1, GL_FALSE, mat);
	glUniform1i(loc2, 0);
	glUniform4f(loc3, col[0],col[1],col[2],col[3]);
	glBindTexture(GL_TEXTURE_2D,bgtexture);
	glzDrawVAO(vao_num[0],vao[0],GL_TRIANGLES);

}


void Draw (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	
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

		float mi[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		glzLoadIdentity(mi);
		glzScalef(mi,0.17f,0.17f,0.17f);
		float col[4]={1.0f,1.0f,1.0f,1.0f};

		glzTranslatef(mi,-4.7f,-4.7f,0.0f);


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
		
	glzLoadIdentity(m);
	glzOrtho(m, -400, 400, -250, 250, -100, 100);
	glzTranslatef(m,-250,150,0);

	glUniformMatrix4fv(loc1, 1, GL_FALSE, m);



	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glzDrawVAO(vao_num[2],vao[2],GL_TRIANGLES);


	draw_text(-3.9f, 1.9f,11,2,ProgramObject,COL_WHITE);
	draw_text(1.7f, -1.8f,15,2,ProgramObject,COL_WHITE);


	}

	if (gamestate == 5)
	{

		glzLoadIdentity(m);
		glzOrtho(m, -400, 400, -250, 250, -100, 100);


		glUniformMatrix4fv(loc1, 1, GL_FALSE, m);

		glBindTexture(GL_TEXTURE_2D, texture[1]);
	
		glzDirectSpriteRender(m, texture[1], 0, 0, 2, 100, 100, 0, 0, 1.0, 1.0, glzOrigin::TOP_LEFT);
		glzDirectSpriteRender(m, texture[1], 0, 0, 2, 100, 100, 0, 0, 1.0, 1.0, glzOrigin::BOTTOM_LEFT);
		
		glzDirectSpriteRender(m, texture[1], 0, 0, 2, 100, 100, 0, 0, 1.0, 1.0, glzOrigin::BOTTOM_RIGHT);
		glzDirectSpriteRender(m, texture[1], 0, 0, 2, 100, 100, 0, 0, 1.0, 1.0, glzOrigin::TOP_RIGHT);

		glBlendFunc(GL_ONE, GL_ONE);
		glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR);
		glBlendColor(1, 0, 1, 1.0f);
		glEnable(GL_BLEND);

		glzDrawTexture(texture[3], 0, 0, 0, 200, 200, 3, 0, 0, 1, 1);

		glDisable(GL_BLEND);

		glBindTexture(GL_TEXTURE_2D, texture[3]);

	//	glUniform4f(loc3, 1.0f, 0.0f, 1.0f, 1.0f);
		glzDirectSpriteRenderAtlas(0, 0, 1, 100, 100, 4, 4, 14, glzOrigin::CENTERED);

		glzDirectSpriteRenderAtlasPixelPerfect(192, 192, 1, 64, 64, 4, 4, 1, glzOrigin::BOTTOM_LEFT);
		glzDirectSpriteRenderAtlasPixelPerfectQuantized(208, 192, 1, 64, 64, 4, 4, 1, 16.0f, glzOrigin::BOTTOM_LEFT);



		
		draw_text(-3.9f, 1.9f, 12, 2, ProgramObject, COL_WHITE);
		draw_text(1.7f, -1.8f, 15, 2, ProgramObject, COL_WHITE);

	}

	if (gamestate == 6)
	{

		glzLoadIdentity(m);
		glzOrtho(m, -400, 400, -250, 250, -100, 100);


		glUniformMatrix4fv(loc1, 1, GL_FALSE, m);

		glBindTexture(GL_TEXTURE_2D, texture[0]);


		glPointSize(3.0f);
		ps.render_out();


		draw_text2("abcdefghijklmnopqrstuvxyz\nABCDEFGHIJKLMNOPQRSTUVXYZ\n1234567890", 1.0f,210.0f, 32.0f, 1.0f, 2, ProgramObject, COL_WHITE);


		draw_text(-3.9f, 1.9f, 13, 2, ProgramObject, COL_WHITE);
		draw_text(1.7f, -1.8f, 15, 2, ProgramObject, COL_WHITE);

	}


	glUseProgram(0);
	glFlush ();													// Flush The GL Rendering Pipeline
}
