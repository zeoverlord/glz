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

enum class glzBlendingMode { NONE, ADDITIVE, ALPHA };
enum class glzOBject2DSetvar { NONE, ALPHA, SCALE, BLEND, TEXTURE, SPRITE, CURRENT_ANIMATION,CURRENT_FRAME, FRAMESPEED, NODE_LOCAL, NODE_PARENT};

class Object2D {
	// position, orientation, speed and such	

public:

	
	node3 *n_parent;
	node3 n_local;
	int label;
	glzBlendingMode blend;
	float alpha;

	Object2D()
	{
		n_parent = nullptr;
		label = -1;
		n_local = node3();
		blend = glzBlendingMode::NONE;
		alpha = 1.0f;
	}
	
	virtual void draw(glzCamera2D *camera) 
	{ 
		return;
	}

	virtual void update(float seconds)
	{
		return;
	}

	
	virtual void set_i(glzOBject2DSetvar type, int v)
	{
		return;
	}

	virtual void set_f(glzOBject2DSetvar type, float v)
	{
		return;
	}



};

class obj2d_Sprite : public Object2D
{
	unsigned int texture;
	float scale;
	glzSprite sprite;

public:

	virtual void draw(glzCamera2D *camera) override;
	virtual void update(float seconds) override;

	
	virtual void set_i(glzOBject2DSetvar type, int v) override;
	virtual void set_f(glzOBject2DSetvar type, float v) override;

	obj2d_Sprite()
	{
		sprite = glzSprite();		
		texture = 0;
		scale = 1.0;

	}
		
	obj2d_Sprite(int labelin, glzSprite spritein, node3 *nin, node3 nLin, unsigned int tex, double scalein)
	{
		label = labelin;
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
	float scale;
	glzSpriteanimationList sprite;
	int current_animation;
	int current_frame;	
	float framespeed;
	float frametime;

public:

	virtual void draw(glzCamera2D *camera) override;
	virtual void update(float seconds) override;
	virtual void set_i(glzOBject2DSetvar type, int v) override;
	virtual void set_f(glzOBject2DSetvar type, float v) override;
	

	obj2d_Sprite_Animated()
	{
		texture = 0;
		scale = 1.0;
		current_animation = 0;
		current_frame = 0;
		framespeed = 1.0;
		frametime = 0.0;
	}


	obj2d_Sprite_Animated(int labelin, glzSpriteanimationList spritein, node3 *nin, node3 nLin, unsigned int tex, double scalein, float framespeedin)
	{
		label = labelin;
		sprite = spritein;
		texture = tex;
		n_parent = nin;
		n_local = nLin;
		scale = scalein;

		current_animation = 0;
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


	void set(int l, glzOBject2DSetvar type, int v)
	{

		for (auto &a : objects)
			if (a->label == l)
			switch (type)
			{
				case glzOBject2DSetvar::TEXTURE:
				case glzOBject2DSetvar::CURRENT_ANIMATION:
				case glzOBject2DSetvar::CURRENT_FRAME:
					a->set_i(type, v);
					break;
			}
		return;
	}


	void set(int l, glzOBject2DSetvar type, float v)
	{

		for (auto &a : objects)
			if (a->label == l)
			switch (type)
			{
			case glzOBject2DSetvar::ALPHA:
				a->alpha = v;
				break;

			case glzOBject2DSetvar::SCALE:
			case glzOBject2DSetvar::FRAMESPEED:								
				a->set_f(type, v);
				break;

			}
		return;
	}

	void set(int l, glzOBject2DSetvar type, glzBlendingMode v)
	{
		for (auto &a : objects)
			if ((a->label == l) && (type == glzOBject2DSetvar::BLEND)) 
				a->blend = v;
		return;
	}

	void set(int l, glzOBject2DSetvar type, node3 v)
	{
		for (auto &a : objects)
			if ((a->label == l) && (type == glzOBject2DSetvar::NODE_LOCAL))
				a->n_local = v;
		return;
	}

	void set(int l, glzOBject2DSetvar type, node3 *v)
	{
		for (auto &a : objects)
			if ((a->label == l) && (type == glzOBject2DSetvar::NODE_PARENT))
				a->n_parent = v;
		return;
	}


	

};






#endif /* __2dscenegraphbase__ */
