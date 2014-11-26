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

// glz 2d scenegraph base class - i think it's usefull
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git



#include "ztool-type.h"
#include "ztool-vectormath.h"

#ifndef __2dscenegraphbase__
#define __2dscenegraphbase__



class Object2D {
	// position, orientation, speed and such	

public:

	
	node3 *n;
	glzCamera2D *camera;
	virtual void draw() { return; }

};

class obj2d_Sprite : public Object2D
{
	unsigned int texture;
	double scale;
	glzSprite sprite;

public:

	void draw() override;
	obj2d_Sprite() : sprite(glzSprite()), texture({ 0 }), scale({ 1.0 }) {}
	obj2d_Sprite(glzSprite spritein, glzCamera2D *camerain, node3 *nin, unsigned int tex, double scalein)
	{
		sprite=spritein; 
		camera=camerain;
		texture = tex;
		n = nin;
		scale = scalein;
	}

};



#endif /* __2dscenegraphbase__ */
