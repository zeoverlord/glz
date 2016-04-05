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

#pragma once


#include "..\utilities\type.h"
#include "..\utilities\glz.h"
#include "..\utilities\sprite.h"
#include "..\utilities\resourcemanager.h"
#include "..\utilities\vectormath.h"
#include "..\layout\viewport.h"
#include <vector>
#include <memory>

// how it works
// at a start the viewport its inside it's parent window
// if width is wider than what the origin coordinate allows then the window will be resized

class glzBaseState
{
public:

	glzViewport view;
	bool mMessageQuit;
	bool mMessageFullscreen;

public:

	glzBaseState(){ mMessageQuit = false; mMessageFullscreen = false; }
	~glzBaseState(){ Deinitialize(); }
	virtual bool Initialize(int width, int height);
	virtual void Deinitialize(void);
	virtual void Update(float seconds);
	virtual void DisplayUpdate(int width, int height);
	virtual void Draw(void);

	bool pollMessageQuit()
	{
		if(!mMessageQuit) 
			return false;
		else
		{
			mMessageQuit = false;
			return true;
		}
	}

	bool pollMessageFullscreen()
	{
		if(!mMessageFullscreen)
			return false;
		else
		{
			mMessageFullscreen = false;
			return true;
		}
	}
	

};