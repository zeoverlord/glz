// Copyright 2016 Peter Wallström
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

// glz input - i think it's usefull
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>
#include "input.h"


glzInputData::glzInputData()
{
	LMdown = false;
	MMdown = false;
	RMdown = false;
	Mpos_x = 0;
	Mpos_y = 0;
	Mweel = 0;
	Mactive = false;
}

static glzInputData inputData;


glzInput::glzInput()
{}

glzInput::~glzInput()
{}

// keys
void glzInput::addKeyEvent(int key, bool keydown) { inputData.keyDown[key]=keydown; }
bool glzInput::getKeyState(int key) { return inputData.keyDown[key]; }

//mouse

void glzInput::SetMouseL(bool keydown) { inputData.LMdown = keydown; }
void glzInput::SetMouseM(bool keydown) { inputData.MMdown = keydown; }
void glzInput::SetMouseR(bool keydown) { inputData.RMdown = keydown; }

bool glzInput::getMouseL(void) { return inputData.LMdown; }
bool glzInput::getMouseM(void) { return inputData.MMdown; }
bool glzInput::getMouseR(void) { return inputData.RMdown; }

void glzInput::setMouseX(int x){ inputData.Mpos_x = x; }
void glzInput::setMouseY(int y){ inputData.Mpos_y = y; }
int glzInput::getMouseX(void){ return inputData.Mpos_x; }
int glzInput::getMouseY(void){ return inputData.Mpos_y; }

void glzInput::setMouseWeel(int m) { inputData.Mweel = m; }
void glzInput::addMouseWeel(int m) { inputData.Mweel += m; }
int glzInput::getMouseWeel(void) { return inputData.Mweel; }