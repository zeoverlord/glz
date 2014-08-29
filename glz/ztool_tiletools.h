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

// tile helper functions
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git



#include "ztool-type.h"
#include "ztool-tex.h"
#include "ztool-glz.h"
#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>

#ifndef __glztiletool__
#define global __glztiletool__

enum class glzTileType { DOUBLE_LAYER, QUAD_LAYER };

class glztiles{
public:
	
	int width;
	int height;
	glzTileType type;
	img_head imghdr;
	unsigned char *data;
	unsigned int tex;
	char img_filename[255];
	


	void load(char filename[255]);
	void update_texture(void);
	void save(void);

	void paint_pixel(int x, int y, int px, int py, bool animate, bool flip, int layer);

	bool getTilecolision(float x, float y, int layer, bool flip_y);

};




#endif /* __glztiletool__ */
