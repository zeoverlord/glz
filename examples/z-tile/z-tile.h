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
#include <string>

// how it works
// at a start the viewport its inside it's parent window
// if width is wider than what the origin coordinate allows then the window will be resized

class ZtileState : public glzBaseState
{

public:

	ZtileState();
	~ZtileState(){ Deinitialize(); }
	bool Initialize(int width, int height) override;
	void Deinitialize(void) override;
	void Update(float seconds) override;
	void DisplayUpdate(int width, int height) override;
	void Draw(void) override;

private:
		

	enum ztUIP { NONE, BACKGROUND, PIXELMAP, SPRITESHEET, SPRITE, UIFRAME };

	enum ztLeveltex { L1A, L1B, L2A, L2B, DYNAMIC_A, DYNAMIC_B, DYNAMIC_C, DYNAMIC_D, NO_CHANGE };

	// User Defined Variables
	float		angle, width, height;												// Used To Rotate The Triangles
	unsigned int vao[16], vao_num[16], textvao[16], textvao_num[16];
	glzMatrix m;

	char tbuffer[160];
	char tbuffer2[160];
	float texttimer;
	float spriteframetimer;
	int spriteframe;

	int gamestate, testanim;
	float testanimTimer ;
	float keyTimer;

	int tiles_width, tiles_height;

	float paintarea_x, paintarea_y, paintarea_Zoom;
	float paintarea_pixel_x, paintarea_pixel_y;

	float paintarea_x_s, paintarea_y_s, paintarea_Zoom_s;
	float paintarea_pixel_x_s, paintarea_pixel_y_s;

	unsigned int  ProgramObject, ProgramObjectFT, ProgramObjectFSQ, ProgramObjectAtlas;
//	texture_transform text_tt;


	int Mpos_x_old;
	int Mpos_y_old;
	int Mpos_x_rel;
	int Mpos_y_rel;
	int Mweel_old;
	int Mweel_rel;

	vert3 mwp; // mouse work position, z is ignored

	vert3 muip;

	int z_tileUI_point;


	int cursprite_x, cursprite_y;
	bool cursprite_anim = false, cursprite_extra = false;

	bool dual_view = false;
	bool toggle_extra = false;




	ztLeveltex curlayer;
	bool has_l1 = true;
	bool has_l2 = true;
	bool has_d = true;

	string leveltex_1_filename;
	string leveltex_2_filename;
	string leveltex_d_filename;
	glztiles map_l1;
	glztiles map_l2;
	glztiles map_dynamic;
	

};