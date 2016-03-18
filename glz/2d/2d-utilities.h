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

// glz 2d utilities - i think it's usefull
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#pragma once


#include "..\utilities\type.h"
#include "..\utilities\glz.h"
#include "..\utilities\resourcemanager.h"
#include "..\utilities\vectormath.h"
#include "..\utilities\sprite.h"
#include "..\utilities\color.h"
#include "..\shader\shader.h"
#include "geo-2d.h"

//void glzBackdrop(texturecontainer *texture);

void glzBackdrop(unsigned int texture, glzBlendingMode blend = glzBlendingMode::NONE, glzColor color = COL_WHITE);
void glzBackdrop(unsigned int texture, glzMatrix mat, glzBlendingMode blend = glzBlendingMode::NONE, glzColor color = COL_WHITE);

void glzBackdrop(texturecontainer *texture, glzBlendingMode blend = glzBlendingMode::NONE, glzColor color = COL_WHITE);
void glzBackdrop(texturecontainer *texture, glzMatrix mat, glzBlendingMode blend = glzBlendingMode::NONE, glzColor color = COL_WHITE);

//void glzDrawSprite(texturecontainer *texture, glzBlendingMode blend, float x, float y, float scale, float aspect);
//void glzDrawSprite(texturecontainer *texture, glzSprite sprite, glzBlendingMode blend, float x, float y, float scale, float aspect);


void glzDrawSprite(texturecontainer *texture, float x, float y, float scale, float aspect, glzBlendingMode blend = glzBlendingMode::NONE, glzColor color = COL_WHITE);
void glzDrawSprite(texturecontainer *texture, glzSprite sprite, float x, float y, float scale, float aspect, glzBlendingMode blend = glzBlendingMode::NONE, glzColor color = COL_WHITE);


void glzDrawText(string text, vert3 pos, float scale, float kern, float aspect, texturecontainer *font, glzColor color = COL_WHITE, glzOrigin origin = glzOrigin::TOP_LEFT);
void glzDrawText(string text, float x, float y, float scale, float kern, float aspect, texturecontainer *font, glzColor color = COL_WHITE, glzOrigin origin = glzOrigin::TOP_LEFT);