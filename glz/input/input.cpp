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

	pulsar1 = false;
	pulsar2 = false;
	pulsar4 = false;
	pulsar8 = false;
	pulsar16 = false;
	pulsartimer = 0.0f;
	pulsarcounter = 0;
}

static glzInputData inputData;


glzInput::glzInput()
{}

glzInput::~glzInput()
{}

void glzInput::updateKeys(float inSeconds)
{
	const float pulsestep = 1.0f / 16.0f;

	inputData.pulsartimer += inSeconds;

	inputData.pulsar1 = false;
	inputData.pulsar2 = false;
	inputData.pulsar4 = false;
	inputData.pulsar8 = false;
	inputData.pulsar16 = false;

	if (inputData.pulsartimer >= pulsestep)
	{ 		
		while (inputData.pulsartimer >= pulsestep)
		{
			inputData.pulsarcounter++;
			inputData.pulsartimer -= pulsestep;

			if (inputData.pulsarcounter > 15)
			{
				inputData.pulsarcounter -= 16;
			}
		}

		if (inputData.pulsarcounter == 0)
			{
				inputData.pulsar1 = true;
			}

		if (inputData.pulsarcounter%2 == 0)
			{
				inputData.pulsar2 = true;
			}

		if (inputData.pulsarcounter % 4 == 0)
			{
				inputData.pulsar4 = true;
			}

		if (inputData.pulsarcounter % 8 == 0)
			{
				inputData.pulsar8 = true;
			}

		if (inputData.pulsarcounter % 16 == 0)
			{
				inputData.pulsar16 = true;
			}
	}
	return;
}

// pulsars
bool glzInput::getPulsar1(void) { return inputData.pulsar1; }
bool glzInput::getPulsar2(void) { return inputData.pulsar2; }
bool glzInput::getPulsar4(void) { return inputData.pulsar4; }
bool glzInput::getPulsar8(void) { return inputData.pulsar8; }
bool glzInput::getPulsar16(void) { return inputData.pulsar16; }

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