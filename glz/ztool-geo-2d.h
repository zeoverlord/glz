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

// glz geometry toolkit 2d- for allmost all your 2d geometry needs
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git


using namespace std;

#include "ztool-type.h"
#include "ztool-geo.h"

#ifndef __glz_geo2d__
#define __glz_geo2d__

long glzVAOMakeText(string text, glzMatrix matrix, float kern,  glzOrigin textorigin, unsigned int *vao);
long glzVAOMakeText2d(string text, float scale, float aspect, float kern,  glzOrigin textorigin, unsigned int *vao);
void glzDirectDrawText(string text, float scale, float aspect, float kern, glzOrigin textorigin);
void glzDrawTexture(unsigned  texture, unsigned  sampler, float X0, float Y0, float X1, float Y1, float Z, float s0, float t0, float s1, float t1);  // mirroring GL_DrawTextureNV exactly
void glzDirectSpriteRender(glzMatrix m, unsigned int texture, float X, float Y, float Z, float W, float H, float spriteX, float spriteY, float spriteW, float spriteH, glzOrigin origin);
void glzDirectSpriteRender(glzMatrix m, unsigned int texture, glzSprite sprite, glzOrigin origin);
void glzDirectSpriteRender(glzMatrix m, unsigned int texture, glzSprite sprite, float width, float height, glzOrigin origin);
void glzDirectSpriteRender(float X, float Y, float Z, float W, float H, float spriteX, float spriteY, float spriteW, float spriteH, glzOrigin origin);
void glzDirectSpriteRenderAtlas(float X, float Y, float Z, float W, float H, unsigned int atlasW, unsigned int atlasH, unsigned int atlasN, glzOrigin origin);
void glzDirectSpriteRenderAtlasPixelPerfect(float X, float Y, float Z, int textureW, int textureH, unsigned int atlasW, unsigned int atlasH, unsigned int atlasN, glzOrigin origin);
void glzDirectSpriteRenderAtlasPixelPerfectQuantized(float X, float Y, float Z, int textureW, int textureH, unsigned int atlasW, unsigned int atlasH, unsigned int atlasN, float q, glzOrigin origin);

/*
plan:

* change glzDirectSpriteRender to use more origins
DONE * change glzOrigin orientation to glzOrigin origin
* create more overloads and generally clean up the glzDirectSpriteRender interface
* add rotation to glzDirectSpriteRender or similar function
* add all the FSQ rendering stuff as well, along with text and text boxes
* add sprite map and sprite animation structures and functions
* make an advanced glzDirectSpriteRender for sprites with depth information
* make a auto animated sprite entety
DONE * move text generation to 2d
DONE * add direct rendering to text
DONE * improve text rendering
DONE * move atlas lookup to the pre computation stage

*/
#endif /* __glz_geo2d__ */