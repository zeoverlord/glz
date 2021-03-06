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

// blank example app - i wouldn't say it produces any meningfull results, but it's a start.
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git


#include "blank_base.h"

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>
#include "..\..\glz-core\app\appbase.h"
#include "..\..\glz-core\input\input.h"


//using namespace std;										

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

using namespace std;
using namespace GLZ;

BlankBaseState::BlankBaseState(){}



bool BlankBaseState::Initialize(int width, int height)					// Any GL Init Code & User Initialiazation Goes Here
{

	GetFocus();
	GetAsyncKeyState(WM_KEYUP);

	// Start Of User Initialization

	glClearColor (0.0f, 1.0f, 1.0f, 0.5f);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate




	return TRUE;												// Return TRUE (Initialization Successful)
}


void BlankBaseState::Deinitialize(void)										// Any User DeInitialization Goes Here
{
}

void BlankBaseState::Update(float seconds)								// Perform Motion Updates Here
{
	glzInput input;

	if(input.getKeyState(VK_ESCAPE) == TRUE)					// Is ESC Being Pressed?
	{
		mMessageQuit = true;						// Terminate The Program
	}

	if(input.getKeyState(VK_F1) == TRUE)						// Is F1 Being Pressed?
	{
		mMessageFullscreen = true;							// Toggle Fullscreen Mode
	}

	
}

void BlankBaseState::DisplayUpdate(int width, int height)
{
	mView.setDisplay(0, 0, width, height);
}

void BlankBaseState::Draw(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	
	
	glFlush ();													// Flush The GL Rendering Pipeline
}
