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

using namespace std;

#include "type.h"
#include "glz.h"
#include "vectormath.h"
#include "sprite.h"



glzSprite::glzSprite( glzOrigin origin) // grid atlas initialization
{

	glzSprite quad = glzAtlasQuad(1, 1, 0, origin);
	a = quad.a;
	b = quad.b;
	c = quad.c;
	d = quad.d;
	depth = 0.0;

}


glzSprite::glzSprite(unsigned int xdim, unsigned int ydim, unsigned int atlas, float depthin) // grid atlas initialization
{

	glzSprite quad = glzAtlasQuad(xdim, ydim, atlas, glzOrigin::BOTTOM_LEFT);
	a = quad.a;
	b = quad.b;
	c = quad.c;
	d = quad.d;
	depth = depthin;

}

glzSprite::glzSprite(unsigned int xdim, unsigned int ydim, unsigned int atlas, float depthin, glzOrigin origin) // grid atlas initialization
{

	glzSprite quad = glzAtlasQuad(xdim, ydim, atlas, origin);
	a = quad.a;
	b = quad.b;
	c = quad.c;
	d = quad.d;
	depth = depthin;

}

void glzSprite::make_polygons(vector<poly3> *pdata, float x, float y, float width, float height, int group, int atlas)
{

	poly3 p1, p2;

	p1.atlas = atlas;
	p2.atlas = atlas;

	p1.group = group;
	p2.group = group;

	vec3 n(0.0, 1.0, 0.0);

	p1.a.n = n;
	p1.b.n = n;
	p1.c.n = n;

	p2.a.n = n;
	p2.b.n = n;
	p2.c.n = n;

	p1.a.t = a;
	p1.b.t = b;
	p1.c.t = c;

	p2.a.t = c;
	p2.b.t = b;
	p2.c.t = d;

	p1.a.v = vert3(0.0 + x, 0.0 + y, 0.0);
	p1.b.v = vert3(width + x, 0.0 + y, 0.0);
	p1.c.v = vert3(0.0 + x, height + y, 0.0);

	p2.a.v = vert3(0.0 + x, height + y, 0.0);
	p2.b.v = vert3(width + x, 0.0 + y, 0.0);
	p2.c.v = vert3(width + x, height + y, 0.0);

	pdata->push_back(p1);
	pdata->push_back(p2);

}


void glzSprite::make_polygons(vector<poly3> *pdata, float x, float y, float width, float height, int group, int atlas, glzMatrix m)
{

	poly3 p1, p2;

	p1.atlas = atlas;
	p2.atlas = atlas;

	p1.group = group;
	p2.group = group;

	vec3 n(0.0, 1.0, 0.0);

	p1.a.n = n;
	p1.b.n = n;
	p1.c.n = n;

	p2.a.n = n;
	p2.b.n = n;
	p2.c.n = n;

	p1.a.t = a;
	p1.b.t = b;
	p1.c.t = c;

	p2.a.t = c;
	p2.b.t = b;
	p2.c.t = d;

	p1.a.v = vert3(0.0 + x, 0.0 + y, 0.0);
	p1.b.v = vert3(width + x, 0.0 + y, 0.0);
	p1.c.v = vert3(0.0 + x, height + y, 0.0);

	p2.a.v = vert3(0.0 + x, height + y, 0.0);
	p2.b.v = vert3(width + x, 0.0 + y, 0.0);
	p2.c.v = vert3(width + x, height + y, 0.0);

	glzProjectVertex(&p1, m, group);
	glzProjectVertex(&p2, m, group);

	pdata->push_back(p1);
	pdata->push_back(p2);

}

void glzSprite::make_polygons(vector<poly3> *pdata, float x, float y, float width, float height, int group, int atlas, glzOrigin origin, glzMatrix m)
{

	poly3 p1, p2;
	vector<poly3> pdata_temp;

	p1.atlas = atlas;
	p2.atlas = atlas;

	p1.group = group;
	p2.group = group;

	vec3 n(0.0, 1.0, 0.0);

	p1.a.n = n;
	p1.b.n = n;
	p1.c.n = n;

	p2.a.n = n;
	p2.b.n = n;
	p2.c.n = n;

	p1.a.t = a;
	p1.b.t = b;
	p1.c.t = c;

	p2.a.t = c;
	p2.b.t = b;
	p2.c.t = d;

	p1.a.v = vert3(0.0 + x, 0.0 + y, 0.0);
	p1.b.v = vert3(width + x, 0.0 + y, 0.0);
	p1.c.v = vert3(0.0 + x, height + y, 0.0);

	p2.a.v = vert3(0.0 + x, height + y, 0.0);
	p2.b.v = vert3(width + x, 0.0 + y, 0.0);
	p2.c.v = vert3(width + x, height + y, 0.0);




	//glzRecenterVectorArray(&p1, group, origin);
	//glzRecenterVectorArray(&p2, group, origin);

	//glzProjectVertex(&p1, m, group);
	//glzProjectVertex(&p2, m, group);

	pdata_temp.push_back(p1);
	pdata_temp.push_back(p2);

	
	glzRecenterVectorArray(&pdata_temp, group, origin);
	glzProjectVertexArray(&pdata_temp, m, group);
	



	//glzProjectVertex(&p1, m, group);
	
	pdata->push_back(pdata_temp.at(0));
	pdata->push_back(pdata_temp.at(1));

}

glzSpriteList::glzSpriteList(int w, int h) // direct initialization with preset
{
	int i = 0;

	while (i < w*h)
	{
		map.push_back(glzSprite(w, h, i, 0.0));
		i++;
	}



	//while (i < w*h) { map.push_back(glzAtlasQuad(w, h, i, glzOrigin::BOTTOM_LEFT));	i++; }
}

glzSpriteList::glzSpriteList(int w, int h, int v[], int n) // direct initialization with preset
{
	int i = 0;

	while (i < n)
	{
		map.push_back(glzSprite(w, h, v[i], 0.0));
		i++;
	}



	//while (i < w*h) { map.push_back(glzAtlasQuad(w, h, i, glzOrigin::BOTTOM_LEFT));	i++; }
}