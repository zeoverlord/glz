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
#include <vector>
#include <memory>

#ifndef __2dscenegraphbase__
#define __2dscenegraphbase__



class Object2D {
	// position, orientation, speed and such	

public:

	
	node3 *n_parent;
	node3 n_local;
	virtual void draw(glzCamera2D *camera) 
	{ 
		return;
	}

	virtual void update(float seconds)
	{
		return;
	}

};

class obj2d_Sprite : public Object2D
{
	unsigned int texture;
	double scale;
	glzSprite sprite;

public:

	virtual void draw(glzCamera2D *camera) override;
	virtual void update(float seconds) override;
	obj2d_Sprite() : sprite(glzSprite()), texture({ 0 }), scale({ 1.0 }) {}
	obj2d_Sprite(glzSprite spritein, node3 *nin, node3 nLin, unsigned int tex, double scalein)
	{
		sprite=spritein; 		
		texture = tex;
		n_parent = nin;
		n_local = nLin;
		scale = scalein;
	}

};

class obj2d_Sprite_Animated : public Object2D
{
	unsigned int texture;
	double scale;
	vector<glzSprite> sprite;
	int current_frame;
	float framespeed;
	float frametime;

public:

	virtual void draw(glzCamera2D *camera) override;
	virtual void update(float seconds) override;
	obj2d_Sprite_Animated()
	{
		sprite.push_back(glzSprite());
		texture = 0;
		scale = 1.0;
		current_frame = 0;
		framespeed = 1.0;
		frametime = 0.0;
	}


	obj2d_Sprite_Animated(vector<glzSprite> spritein, node3 *nin, node3 nLin, unsigned int tex, double scalein, float framespeedin)
	{
		sprite = spritein;
		texture = tex;
		n_parent = nin;
		n_local = nLin;
		scale = scalein;

		current_frame=0;
		framespeed = framespeedin;
		frametime=0.0f;

	}

};






class Object2DGraph {
	// position, orientation, speed and such	

public:


	vector<shared_ptr<Object2D>> objects;
	glzCamera2D *camera;
	
	Object2DGraph(glzCamera2D *cam) { camera = cam; }

	template<typename T> // fun with templates
	void add(T obj)
	{
		objects.push_back(make_shared<T>(obj));
	}

	void draw()
	{

		for (auto a : objects)
			a->draw(camera);

		return;
	}

	void update(float seconds)
	{

		for (auto a : objects)
			a->update(seconds);

		return;
	}


};






#endif /* __2dscenegraphbase__ */
