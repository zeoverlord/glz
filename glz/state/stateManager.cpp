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

// glz base state class
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>
#include "stateManager.h"
#include "..\appbase.h"
#include "..\input\input.h"



static vector<glzStateManagerData> states;
static string CurrentStateString;
static std::shared_ptr<glzBaseState> CurrentState;
static glzViewport view;


bool glzStateManager::addState(std::shared_ptr<glzBaseState> inState, string inName)
{			
	states.push_back(glzStateManagerData(inState, inName));
	return true;
}



void glzStateManager::removeState(string inName)
{

	if(states.empty())
		return;

	int i2 = 0;

	auto i = states.begin();

	while(i < states.end()) {

		if(states[i2].mStateName == inName)
		{
			i = states.erase(i);			
		}
		else {
			++i;
			i2++;
		}

	}

	if(CurrentStateString == inName)
	{
		CurrentStateString = states.back().mStateName;
		CurrentState = states.back().mState;
	}


}
bool glzStateManager::switchState(string inName)
{
	for(auto a : states)
		if(a.mStateName == inName)
		{
			CurrentStateString = inName;
			CurrentState = a.mState;
			CurrentState->DisplayUpdate(view.getDisplayWidth(), view.getDisplayHeigth());
			return true;
		}			
	return false;
}


bool glzStateManager::hasState()
{
	if(states.size() > 0)
		return true;
	else 
		return false;
}

bool glzStateManager::stateExists(string inName)
{
	for(auto a : states)
		if(a.mStateName == inName)
		{		
			return true;
		}
	return false;
}

bool glzStateManager::Initialize(int width, int height)
{
	view.setDisplay(0, 0, width, height);
	return CurrentState->Initialize(view.getDisplayWidth(), view.getDisplayHeigth());
}

void glzStateManager::Deinitialize(void)
{
	for(auto a : states)
		a.mState->Deinitialize();

	CurrentStateString = "";
	CurrentState = nullptr;
	states.clear();
}

void glzStateManager::Update(float seconds)
{
	CurrentState->Update(seconds);
}

void glzStateManager::DisplayUpdate(int width, int height)
{
	view.setDisplay(0, 0, width, height);
	CurrentState->DisplayUpdate(width, height);
}
void glzStateManager::Draw(void)
{
	CurrentState->Draw();
}

bool glzStateManager::pollMessageQuit()
{
	return CurrentState->pollMessageQuit();
}

bool glzStateManager::pollMessageFullscreen()
{
	return CurrentState->pollMessageFullscreen();
}