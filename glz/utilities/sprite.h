// Copyright 2014 Peter Wallström
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

// common glz vector and matrix classes
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#pragma once

#include "type.h"
#include "vectormath.h"
using namespace std;

class glzSprite{ //atlassprite class

private:


public:

	// an atlas is defined by 2 tex coords, depth is used in case a texture array is used, coord are in the following config
	// c d
	// a b
	// with a being 0,0 and d is 1,1

	tex2 a, b, c, d;
	float depth;

	glzSprite() : a(tex2(0.0, 0.0)), b(tex2(1.0, 0.0)), c(tex2(0.0, 1.0)), d(tex2(1.0, 1.0)), depth(0.0){} // default numbers
	glzSprite(tex2 ain, tex2 bin, tex2 cin, tex2 din, float depthin) : a{ ain }, b{ bin }, c{ cin }, d{ din }, depth{ depthin }{} // direct initialization
	glzSprite(tex2 pos, tex2 dim, float depthin) : a{ tex2(pos.u, pos.v) }, b{ tex2(pos.u + dim.u, pos.v) }, c{ tex2(pos.u, pos.v + dim.v) }, d{ tex2(pos.u + dim.u, pos.v + dim.v) }, depth{ depthin }{} // simpler initialization

	glzSprite(glzOrigin origin);
	glzSprite(unsigned int xdim, unsigned int ydim, unsigned int atlas, float depthin); // grid atlas initialization
	glzSprite(unsigned int xdim, unsigned int ydim, unsigned int atlas, float depthin, glzOrigin origin);  // grid atlas initialization with origin

	void make_polygons(vector<poly3> *pdata, float x, float y, float width, float height, int group, int atlas);
	void make_polygons(vector<poly3> *pdata, float x, float y, float width, float height, int group, int atlas, glzMatrix m);
	void make_polygons(vector<poly3> *pdata, float x, float y, float width, float height, int group, int atlas, glzOrigin origin, glzMatrix m);
};


class glzSpriteList{ //atlassprite class

private:


public:


	vector<glzSprite> map;

	glzSpriteList() {} // default numbers
	glzSpriteList(glzSprite a) { map.push_back(a); } // direct initialization
	glzSpriteList(int w, int h); // direct initialization with preset

	glzSpriteList(int w, int h, int v[], int n); // direct initialization with preset


	void addsprite(glzSprite a) { map.push_back(a); }

	glzSprite get_sprite(int frame)
	{
		return map.at(frame);
	}

};


class glzSpriteanimationList{ //atlassprite class

private:


public:

	vector<glzSpriteList> map;

	glzSpriteanimationList() {} // default numbers
	glzSpriteanimationList(glzSpriteList a) { map.push_back(a); } // direct initialization
	glzSpriteanimationList(glzSprite a) { glzSpriteList b(a); map.push_back(b); } // direct initialization
//	glzSpriteanimationList(int w, int h); // direct initialization with preset


	void addsprite(glzSpriteList a) { map.push_back(a); }

	glzSprite get_sprite(int animation, int frame)
	{
		return map.at(animation).get_sprite(frame);
	}

};