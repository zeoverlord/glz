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
#include "Example_base.h"
#include <fstream>
#include <math.h>
#include "..\..\glz-core\app\appbase.h"
#include "..\..\glz-core\3d\geo.h"
#include "..\..\glz-core\3d\geo-generate.h"
#include "..\..\glz-core\utilities\vectormath.h"
#include "..\..\glz-core\2d\geo-2d.h"
#include "..\..\glz-core\shader\shader.h"
#include "..\..\glz-core\utilities\glz.h"
#include "..\..\glz-core\image\tex.h"
#include "..\..\glz-core\utilities\resourcemanager.h"
#include "..\..\glz-core\input\input.h"
#include "..\..\glz-core\2d\2d-utilities.h"

using namespace std;										

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors


// User Defined Variables


static PFNGLUSEPROGRAMPROC						glUseProgram;
static PFNGLUNIFORM1IPROC                       glUniform1i;
static PFNGLUNIFORMMATRIX4FVPROC                glUniformMatrix4fv;
static PFNGLUNIFORM4FARBPROC                    glUniform4f;
static PFNGLGETUNIFORMLOCATIONPROC              glGetUniformLocation;

ExampleBaseState::ExampleBaseState() :
e(0),
e2(0)
{

}


bool ExampleBaseState::Initialize(int width, int height)					// Any GL Init Code & User Initialiazation Goes Here
{	
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

	mView.setDisplay(0, 0, width, height);


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

	rm.createTexture("font.digitalstrip_l", "data\\fonts\\digitalstrip_l.tga", glzTexFilter::LINEAR);
	rm.createTexture("texture.mariobox", "data\\mariobox.tga", glzTexFilter::ANSIO_MAX, 2);
	rm.createTexture("texture.cv90base", "data\\cv90base.tga", glzTexFilter::ANSIO_MAX, 2);
	rm.createTexture("texture.gridlines", "data\\gridlines.tga", glzTexFilter::ANSIO_MAX, 2);
	rm.createTexture("atlas.atlas_testpattern", "data\\atlas_testpattern.tga", glzTexFilter::NEAREST, 2);


	delete[] data;
	data=NULL;

	rm.load_all();

	return TRUE;												// Return TRUE (Initialization Successful)
}


void ExampleBaseState::Deinitialize(void)										// Any User DeInitialization Goes Here
{

	// this shouldn't normally be nessecary, but it's better to make it a habit for when you load and unload resources mid game.
	glzKillAllVAO();


}

void ExampleBaseState::Update(float seconds)								// Perform Motion Updates Here
{
	glzResourcemanager rm;
	rm.load_one();

	glzInput input;

	if(input.getKeyState(VK_ESCAPE) == TRUE)					// Is ESC Being Pressed?
	{
		mMessageQuit = true;						// Terminate The Program
	}

	if(input.getKeyState(VK_F1) == TRUE)						// Is F1 Being Pressed?
	{
		mMessageFullscreen = true;							// Toggle Fullscreen Mode
	}

	angle += seconds*50;						// Update angle Based On The Clock
	q.rotate(seconds * 50, 0.0, 1.0, 0.0);
	q.rotate(seconds * 40, 1.0, 0.0, 0.0);

	if (input.getKeyState(VK_UP)) q2.rotate(seconds * 40, 1.0, 0.0, 0.0);
	if (input.getKeyState(VK_DOWN)) q2.rotate(seconds * -40, 1.0, 0.0, 0.0);

	if (input.getKeyState(VK_LEFT)) q2.rotate(seconds * 40, 0.0, 1.0, 0.0);
	if (input.getKeyState(VK_RIGHT)) q2.rotate(seconds * -40, 0.0, 1.0, 0.0);

	//now the rotations are a bit wonky in this example but that is because i dont reset the quaternion for each frame and use angles instead here

	q3.rotate(seconds * 50, 0.0, 1.0, 0.0);

}

void ExampleBaseState::DisplayUpdate(int width, int height)
{
	mView.setDisplay(0, 0, width, height);
}

void ExampleBaseState::Draw(void)
{
	glzResourcemanager rm;


	glBindTexture(GL_TEXTURE_2D, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

    glBindTexture(GL_TEXTURE_2D,rm.gettextureHandle("texture.mariobox"));
	glzDrawVAO(vao_num[1],vao[1],GL_TRIANGLES);



	// draw tank
	m.LoadIdentity();
	m.perspective(45.0f, 1.618f, 1.0f, 100.0f);
	m.translate(7, 2, -30);
	m.scale(0.5,0.5,0.5);
	m.loadQuanternion(q2);

	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

	glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("texture.cv90base"));
	glzDrawVAO(vao_num[2],vao[2],GL_TRIANGLES);

	// draw grid
	m.LoadIdentity();
	m.ortho(-480, 480, -270, 270, -100, 100);
	m.translate(-300, -100, 0);
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

	glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("atlas.atlas_testpattern"));
	glzDrawVAO(vao_num[3],vao[3],GL_TRIANGLES);

	// draw height grid
	m.LoadIdentity();
	m.perspective(45.0f, 1.618f, 1.0f, 100.0f);
	m.translate(2, -2, -7);
	m.loadQuanternion(q3);
	m.transferMatrix(&mtemp[0]);
	glUniformMatrix4fv(loc1, 1, GL_FALSE, mtemp);

	glBindTexture(GL_TEXTURE_2D, rm.gettextureHandle("texture.gridlines"));
	glzDrawVAO(vao_num[4],vao[4],GL_TRIANGLES);
	
	// draw text
	glzDrawText("Geometry generation test, try the arrow keys.", -0.5f, 0.49f, 0.05f, 1.3f, mView.getAspect(), rm.gettexture("font.digitalstrip_l"), COL_WHITE);

	glUseProgram(0);
	glFlush ();													// Flush The GL Rendering Pipeline
}
