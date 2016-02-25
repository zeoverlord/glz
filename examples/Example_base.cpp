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

// glz example app - i wouldn't say it produces any meningfull results, but it's a start.
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
#include "..\glz\appbase.h"
#include "..\glz\3d\geo.h"
#include "..\glz\3d\geo-generate.h"
#include "..\glz\utilities\vectormath.h"
#include "..\glz\2d\geo-2d.h"
#include "..\glz\shader\shader.h"
#include "..\glz\utilities\glz.h"
#include "..\glz\image\tex.h"
#include "..\glz\input\input.h"

using namespace std;										

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors


GL_Window*	g_window;
Keys*		g_keys;

// User Defined Variables
float		angle,width,height;												// Used To Rotate The Triangles
int			rot1, rot2;											// Counter Variables
unsigned int vao[5],vao_num[5];
glzMatrix m;
int e=0,e2=0;
unsigned int texture[5],fonttexture;

glzQuaternion q;
glzQuaternion q2;
glzQuaternion q3;

img_head img;
unsigned char *data;

GLhandleARB  ProgramObject,ProgramObjectFSQ;

static PFNGLUSEPROGRAMPROC						glUseProgram;
static PFNGLUNIFORM1IPROC                       glUniform1i;
static PFNGLUNIFORMMATRIX4FVPROC                glUniformMatrix4fv;
static PFNGLUNIFORM4FARBPROC                    glUniform4f;
static PFNGLGETUNIFORMLOCATIONPROC              glGetUniformLocation;


int WINDOW_HEIGHT;
int WINDOW_WIDTH;

void preInitialize(void)
{
	glzAppinitialization app;
	app.set_title(L"ZeoBase GL Framework");
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



	glUseProgram				= (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
	glGetUniformLocation		= (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress("glGetUniformLocation");                                                                     
	glUniform1i= (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
	glUniform4f= (PFNGLUNIFORM4FARBPROC) wglGetProcAddress("glUniform4fARB");
	glUniformMatrix4fv= (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");




	glzMatrix mt;

	glzMatrix mo;
	glzMatrix mg;
	glzMatrix mh;

	unsigned int ad[16]={0,1,1,0,
						 1,1,2,0,
						 3,3,2,0,
						 0,0,2,3};
	mt.LoadIdentity();
	mo.LoadIdentity();
	mg.LoadIdentity();
	mh.LoadIdentity();

	mg.translate(0, 0, 0);

	mt.translate(-3.9f, 0.9f, 0);

	mt.scale(0.17f, 0.17f, 0.17f);

	mg.scale(32.0f, 32.0f, 32.0f);
	mh.scale(0.4f, 0.4f, 0.4f);

	mh.rotate(90, 1.0f, 0.0f, 0.0f);

	mh.translate(-8.0, 8.0, 0.0);

	



	// read our heightmap
	glzReadTgaHead(&img, "data\\heightmap.tga");
	data = new unsigned char[img.imageSize];
	glzLoadTga(&img,"data\\heightmap.tga", data);
	

	primitive_gen prim_pg[2];

	texture_transform text_tt = glzMakeTTAtlas(16, 16, 0, glzOrigin::BOTTOM_LEFT);

	prim_pg[0] = glzMakePGDefault(glzPrimitive::CUBE);
	prim_pg[0].matrix.scale(5.0f, 5.0f, 5.0f);

	prim_pg[1] = glzMakePGDefault(glzPrimitive::ICOSIDODECAHEDRON);
	prim_pg[1].matrix.translate(-3, 0, 0);
	prim_pg[1].matrix.scale(7.0f, 7.0f, 7.0f);

	
	prim_pg[0].tt = glzMakeTTAtlasCubeCross(4, 4, 0, glzOrigin::BOTTOM_LEFT);
	prim_pg[1].tt = glzMakeTTAtlas(4, 4, 5, glzOrigin::BOTTOM_LEFT);

	texture_transform obj_tt=glzMakeTTDefault();
	texture_transform grid_tt = glzMakeTTAtlasArray(2, 2, ad, glzOrigin::BOTTOM_LEFT);
	texture_transform height_tt = glzMakeTTVertexCoordAdopt(1.0f, 1.0f, glzAxis::Z, glzOrigin::BOTTOM_LEFT);

	image_geo_transform igt = glzMakeIGT(glzIGTType::DISPLACE_ADD, img.m_width, img.m_height, img.m_bpp, 0, 0, 0, 2.0f, 32.0, glzAxis::Z, data);

	vao_num[0] = glzVAOMakeText("Geometry generation test, try the arrow keys.", mt, 1.3f, glzOrigin::BOTTOM_LEFT, &vao[0]);
	vao_num[1]=glzVAOMakePrimitives(1,prim_pg, &vao[1]); // change the first argument to 2 for an extra object, this is subject to some major redecorating
	vao_num[2]=glzVAOMakeFromFile("data\\objects\\cv9040c.obj",mo, obj_tt, &vao[2]);
	vao_num[3]=glzVAOMakeAtlasGrid(4, 4, mg, grid_tt, &vao[3]);
	vao_num[4]=glzVAOMakeHeightAtlasGrid(16, 16, mh, height_tt, igt, &vao[4]);


	//glzKillVAO(vao2, GLZ_AUTO);  // kills any vao created by the glz toolkit and all buffers within

	ProgramObject = glzShaderLoad("data\\glsl.vert", "data\\glsl.frag", glzVAOType::AUTO);
	ProgramObjectFSQ = glzShaderLoad("data\\fsq.vert", "data\\fsq.frag", glzVAOType::AUTO);
	glzShaderLink(ProgramObject);
	glzShaderLink(ProgramObjectFSQ);

	// load the textures
	texture[0] = glzLoadTexture("data\\fonts\\digitalstrip_l.tga", glzTexFilter::LINEAR);
	texture[1] = glzLoadTexture("data\\mariobox.tga", glzTexFilter::ANSIO_MAX);
	texture[2] = glzLoadTexture("data\\cv90base.tga", glzTexFilter::ANSIO_MAX);
	texture[3] = glzLoadTexture("data\\atlas_testpattern.tga", glzTexFilter::NEAREST);
	texture[4] = glzLoadTexture("data\\gridlines.tga", glzTexFilter::ANSIO_MAX);


	delete[] data;
	data=NULL;


	return TRUE;												// Return TRUE (Initialization Successful)
}


void Deinitialize (void)										// Any User DeInitialization Goes Here
{

	// this shouldn't normally be nessecary, but it's better to make it a habit for when you load and unload resources mid game.
	glzKillAllVAO();


}

void Update (float seconds)								// Perform Motion Updates Here
{


	if (g_keys->keyDown [VK_ESCAPE] == TRUE)					// Is ESC Being Pressed?
	{
		TerminateApplication (g_window);						// Terminate The Program
	}

	if (g_keys->keyDown [VK_F1] == TRUE)						// Is F1 Being Pressed?
	{
		ToggleFullscreen (g_window);							// Toggle Fullscreen Mode
	}

	angle += seconds*50;						// Update angle Based On The Clock
	q.rotate(seconds * 50, 0.0, 1.0, 0.0);
	q.rotate(seconds * 40, 1.0, 0.0, 0.0);

	if (g_keys->keyDown[VK_UP] == TRUE)q2.rotate(seconds * 40, 1.0, 0.0, 0.0);
	if (g_keys->keyDown[VK_DOWN] == TRUE)q2.rotate(seconds * -40, 1.0, 0.0, 0.0);

	if (g_keys->keyDown[VK_LEFT] == TRUE)q2.rotate(seconds * 40, 0.0, 1.0, 0.0);
	if (g_keys->keyDown[VK_RIGHT] == TRUE)q2.rotate(seconds * -40, 0.0, 1.0, 0.0);

	//now the rotations are a bit wonky in this example but that is because i dont reset the quaternion for each frame and use angles instead here

	q3.rotate(seconds * 50, 0.0, 1.0, 0.0);

}


void Draw (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	float mtemp[16];
	glEnable(GL_TEXTURE_2D);
	unsigned int loc1 = glGetUniformLocation(ProgramObject,"projMat");
	unsigned int loc2 = glGetUniformLocation(ProgramObject,"texunit0");
	unsigned int loc3 = glGetUniformLocation(ProgramObject,"tint");

	glUseProgram(ProgramObject);
	glUniform1i(loc2, 0);	
	glUniform4f(loc3, 1.0f,1.0f,1.0f,1.0f);

	// draw objects
	m.LoadIdentity();
	m.perspective(45.0f, 1.618f, 1.0f, 1000.0f);
	//glzOrtho(m, -480, 480, -270, 270, -100, 100);

	m.translate(-10, 5, -30);
	//glzScalef(m,0.1,0.1,0.1);
	m.loadQuanternion(q);
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

    glBindTexture(GL_TEXTURE_2D,texture[1]);
	glzDrawVAO(vao_num[1],vao[1],GL_TRIANGLES);



	// draw tank
	m.LoadIdentity();
	m.perspective(45.0f, 1.618f, 1.0f, 100.0f);
	m.translate(7, 2, -30);
	m.scale(0.5,0.5,0.5);
	m.loadQuanternion(q2);

	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

    glBindTexture(GL_TEXTURE_2D,texture[2]);
	glzDrawVAO(vao_num[2],vao[2],GL_TRIANGLES);

	// draw grid
	m.LoadIdentity();
	m.ortho(-480, 480, -270, 270, -100, 100);
	m.translate(-300, -100, 0);
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

    glBindTexture(GL_TEXTURE_2D,texture[3]);
	glzDrawVAO(vao_num[3],vao[3],GL_TRIANGLES);

	// draw height grid
	m.LoadIdentity();
	m.perspective(45.0f, 1.618f, 1.0f, 100.0f);
	m.translate(2, -2, -7);
	m.loadQuanternion(q3);
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

    glBindTexture(GL_TEXTURE_2D,texture[4]);
	glzDrawVAO(vao_num[4],vao[4],GL_TRIANGLES);


	// draw text
	m.LoadIdentity();
	m.ortho(-4, 4, -2, 2, -100, 100);
//	m.translate(-3, y, 0);
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glzDrawVAO(vao_num[0],vao[0],GL_TRIANGLES);
	glDisable(GL_BLEND);



	glUseProgram(0);
	glFlush ();													// Flush The GL Rendering Pipeline
}
