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

// glz example 2d class
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#pragma once


#include "..\..\glz-core\utilities\type.h"
#include "..\..\glz-core\utilities\glz.h"
#include "..\..\glz-core\utilities\sprite.h"
#include "..\..\glz-core\utilities\resourcemanager.h"
#include "..\..\glz-core\utilities\vectormath.h"
#include "..\..\glz-core\layout\viewport.h"
#include "..\..\glz-core\state\baseState.h"
#include "..\..\glz-core\effects\particle.h"
#include "..\..\glz-core\2d\2d-graph.h"
#include <vector>
#include <memory>

// how it works
// at a start the viewport its inside it's parent window
// if width is wider than what the origin coordinate allows then the window will be resized

class Example2DState : public glzBaseState
{

public:

	Example2DState();
	~Example2DState(){ Deinitialize(); }
	bool Initialize(int width, int height) override;
	void Deinitialize(void) override;
	void Update(float seconds) override;
	void DisplayUpdate(int width, int height) override;
	void Draw(void) override;

private:
		
	float		angle, width, height;												// Used To Rotate The Triangles
	unsigned int vao[4], vao_num[4];
	string tbuffer;
	string tbuffer2;
	float spriteframetimer;
	int spriteframe;
	int gamestate;
	glzCamera2D cam;
	Object2DGraph tempgraph;
	node3 n;
	glztiles tilemap;
	glztiles tilemap2;
	glzViewport view1, view2, view3;
	GLhandleARB  ProgramObject, ProgramObjectFT, ProgramObjectFSQ, ProgramObjectFSQ_glitch;
	glzSimpleParticleSystem ps;


public:

	void draw_backdrop_glitch(unsigned int bgtexture, unsigned int bgtexture2);
	

};