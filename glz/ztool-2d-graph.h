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
#include "ztool_tiletools.h"
#include "ztool-shader.h"
#include <algorithm>
#include <vector>
#include <memory>

#ifndef __2dscenegraphbase__
#define __2dscenegraphbase__

enum class glzBlendingMode { NONE, ADDITIVE, ALPHA, MULTIPLY };
enum class glzOBject2DSetvar { NONE, ALPHA, SCALE, BLEND, BLENDCOLOR, WIDTH, HEIGHT, TEXTURE, SPRITE, CURRENT_ANIMATION, CURRENT_FRAME, FRAMESPEED, NODE_LOCAL, NODE_PARENT, KILL, Z_LEVEL, RENDEREGRAPH, ANIMATIONSTOP, ANIMATIONPLAY, ANIMATIONPLAYONCE,VISIBLE};
enum class glzOBject2DAnimationstate { STOPPED, PLAYING, PLAYINGONCE };


class Object2DGraph;

class Object2D {
	// position, orientation, speed and such	

public:

	unsigned int texture;
	node3 *n_parent;
	node3 n_local;
	int label;
	glzBlendingMode blend;
	glzColor blendcolor;
	float alpha;
	bool visible;
	float width;
	float height;
	float scale;
	bool tobekilled;
	float z_order;
	glzOBject2DAnimationstate animationstate;
	int current_animation;
	int current_frame;
	float framespeed;
	float frametime;

	Object2D()
	{
		n_parent = nullptr;
		label = -1;
		n_local = node3();
		blend = glzBlendingMode::NONE;
		blendcolor = glzColor();
		alpha = 1.0f;
		width = 100.0f;
		height = 100.0f;
		scale = 1.0f;
		texture = 0;
		tobekilled = false;
		z_order = 0.0;
		animationstate = glzOBject2DAnimationstate::STOPPED;
		current_animation = 0;
		current_frame = 0;
		framespeed = 0.0;
		frametime = 0.0;
		visible = true;
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

	virtual void set_r(glzOBject2DSetvar type, Object2DGraph *v)
	{
		return;
	}



};


class obj2d_Sprite : public Object2D
{
	glzSpriteanimationList sprite;


public:

	virtual void draw(glzCamera2D *camera) override;
	virtual void update(float seconds) override;
	virtual void set_i(glzOBject2DSetvar type, int v) override;
	virtual void set_f(glzOBject2DSetvar type, float v) override;
	

	


	obj2d_Sprite()
	{
		texture = 0;
		scale = 1.0;
		sprite = glzSpriteanimationList(glzSprite());
		animationstate = glzOBject2DAnimationstate::STOPPED;
		current_animation = 0;
		current_frame = 0;
		framespeed = 0.0f;
		frametime = 0.0f;
		visible = true;
		
	} 


	obj2d_Sprite(int labelin, glzSprite spritein, node3 *nin, node3 nLin, unsigned int tex, double scalein)
	{
		label = labelin;
		sprite = glzSpriteanimationList(spritein);
		texture = tex;
		n_parent = nin;
		n_local = nLin;
		scale = scalein;
		visible = true;

		current_animation = 0;
		current_frame = 0;
		framespeed = 0.0f;
		frametime = 0.0f;
		animationstate = glzOBject2DAnimationstate::STOPPED;
	}


	obj2d_Sprite(int labelin, glzSpriteanimationList spritein, node3 *nin, node3 nLin, unsigned int tex, double scalein, float framespeedin)
	{
		label = labelin;
		sprite = spritein;
		texture = tex;
		n_parent = nin;
		n_local = nLin;
		scale = scalein;
		visible = true;

		current_animation = 0;
		current_frame=0;
		framespeed = framespeedin;
		frametime=0.0f;	
		animationstate = glzOBject2DAnimationstate::PLAYING;
	}


};


class obj2d_Fullscreen : public Object2D
{
	glzSprite sprite;

public:

	virtual void draw(glzCamera2D *camera) override;
	virtual void update(float seconds) override;


	virtual void set_i(glzOBject2DSetvar type, int v) override;
	virtual void set_f(glzOBject2DSetvar type, float v) override;

	obj2d_Fullscreen()
	{
		sprite = glzSprite();
		texture = 0;
		scale = 1.0;
		visible = true;

	}

	obj2d_Fullscreen(int labelin, glzSprite spritein, unsigned int tex)
	{
		label = labelin;
		sprite = spritein;
		texture = tex;
		visible = true;

	}
	obj2d_Fullscreen(int labelin, glzSprite spritein, glzBlendingMode b, unsigned int tex)
	{
		label = labelin;
		blend = b;
		sprite = spritein;
		texture = tex;
		visible = true;

	}

	obj2d_Fullscreen(int labelin, unsigned int tex)
	{
		label = labelin;
		sprite = glzSprite();
		texture = tex;
		visible = true;

	}

	obj2d_Fullscreen(int labelin, glzBlendingMode b, unsigned int tex)
	{
		label = labelin;
		blend = b;
		sprite = glzSprite();
		texture = tex;
		visible = true;

	}

};


class obj2d_Background : public Object2D
{
	glzSpriteanimationList sprite;
	float paralax;



public:



	virtual void draw(glzCamera2D *camera) override;
	virtual void update(float seconds) override;


	virtual void set_i(glzOBject2DSetvar type, int v) override;
	virtual void set_f(glzOBject2DSetvar type, float v) override;

	obj2d_Background()
	{
		sprite = glzSprite();
		texture = 0;
		scale = 1.0;
		paralax = 1.0;
		current_animation = 0;
		current_frame = 0;
		framespeed = 1.0;
		frametime = 0.0;
		visible = true;
	}
	

	obj2d_Background(int labelin, unsigned int tex)
	{
		label = labelin;
		texture = tex;
		current_animation = 0;
		current_frame = 0;
		framespeed = 1.0;
		frametime = 0.0;
		visible = true;
	}


	obj2d_Background(int labelin, glzSprite spritein, float scalein, unsigned int tex)
	{
		label = labelin;
		sprite = glzSpriteanimationList(spritein);
		texture = tex;
		scale = scalein;
		current_animation = 0;
		current_frame = 0;
		framespeed = 0.0;
		frametime = 0.0;
		visible = true;
	}


	obj2d_Background(int labelin, glzSpriteanimationList spritein, float scalein, float framespeedin, unsigned int tex)
	{
		label = labelin;
		sprite = spritein;
		texture = tex;
		scale = scalein;
		current_animation = 0;
		current_frame = 0;
		framespeed = framespeedin;
		if (framespeedin) animationstate = glzOBject2DAnimationstate::PLAYING;
		frametime =0.0;
		visible = true;
	}



	obj2d_Background(int labelin, glzSprite spritein, glzBlendingMode b, float scalein, float paralaxin, unsigned int tex)
	{
		label = labelin;
		blend = b;
		sprite = glzSpriteanimationList(spritein);
		texture = tex;
		scale = scalein;
		paralax = paralaxin;
		current_animation = 0;
		current_frame = 0;
		framespeed = 0.0;
		frametime = 0.0;
		visible = true;

	}

	obj2d_Background(int labelin, glzSpriteanimationList spritein, glzBlendingMode b, float scalein, float framespeedin, float paralaxin, unsigned int tex)
	{
		label = labelin;
		blend = b;
		sprite = spritein;
		texture = tex;
		scale = scalein;
		paralax = paralaxin;
		current_animation = 0;
		current_frame = 0;
		framespeed = framespeedin;
		if (framespeedin) animationstate = glzOBject2DAnimationstate::PLAYING;
		frametime = 0.0;
		visible = true;

	}

};

class obj2d_Tiles : public Object2D
{
	glztiles *map;
	int layer;	
	int tileWidth;
	int tileHeight;

public:

	virtual void draw(glzCamera2D *camera) override;
	virtual void update(float seconds) override;
	virtual void set_i(glzOBject2DSetvar type, int v) override;
	virtual void set_f(glzOBject2DSetvar type, float v) override;


	obj2d_Tiles()
	{
		texture = 0;
		scale = 1.0;
		map = nullptr;
		current_frame = 0;
		framespeed = 0.0;
		frametime = 0.0;
		layer=0;		
		tileWidth = 16;
		tileHeight = 16;
		blend = glzBlendingMode::ALPHA;
		visible = true;
		
	}


	obj2d_Tiles(int labelin, glztiles *mapin, int layerin, int tilewidthin, int tileheightin, float framespeedin, node3 *nin, node3 nLin, unsigned int tex, int spritesize, double scalein)
	{
		label = labelin;		
		texture = tex;
		map = mapin;
		n_parent = nin;
		n_local = nLin;
		scale = scalein;
		layer = layerin;		
		tileWidth = tilewidthin;
		tileHeight = tileheightin;
		current_frame = 0;
		framespeed = framespeedin;
		if (framespeedin) animationstate = glzOBject2DAnimationstate::PLAYING;
		frametime = 0.0f;
		blend = glzBlendingMode::ALPHA;
		width = map->width*(spritesize / tilewidthin);
		height = map->height*(spritesize / tileheightin);
		visible = true;
	
	}

};



class obj2d_Clear : public Object2D
{
	

public:

	virtual void draw(glzCamera2D *camera) override;
	virtual void update(float seconds) override;


	//virtual void set_i(glzOBject2DSetvar type, int v) override;
//	virtual void set_f(glzOBject2DSetvar type, float v) override;

	obj2d_Clear()
	{
		blendcolor = glzColor();		
	}

	obj2d_Clear(glzColor c)
	{
		blendcolor = c;
	}

	obj2d_Clear(int labelin)
	{
		label = labelin;
		blendcolor = glzColor();
	}

	obj2d_Clear(int labelin, glzColor c)
	{
		label = labelin;
		blendcolor = c;
	}
	
};


class obj2d_ColorTint : public Object2D
{

public:

	virtual void draw(glzCamera2D *camera) override;
	virtual void update(float seconds) override;



	obj2d_ColorTint()
	{
		
		texture = 0;
		scale = 1.0;
		visible = true;

	}



	obj2d_ColorTint(int labelin,  glzBlendingMode b, glzColor c)
	{
		label = labelin;
		blend = b;
		blendcolor = c;
		visible = true;

	}
	
};


class obj2d_Object2DGraph : public Object2D
{
	
private:
	Object2DGraph *rendergraph;

public:

	virtual void draw(glzCamera2D *camera) override;
	virtual void update(float seconds) override;

	
	virtual void set_r(glzOBject2DSetvar type, Object2DGraph *v) override;
	//virtual void set_i(glzOBject2DSetvar type, int v) override;
	//	virtual void set_f(glzOBject2DSetvar type, float v) override;

	obj2d_Object2DGraph()
	{
		rendergraph = nullptr;
		visible = true;
	}

	obj2d_Object2DGraph(int labelin, Object2DGraph *c)
	{
		label = labelin;
		rendergraph = c;
		visible = true;
	}

	

};




// ************************** OBject 2D graph **********************************



class Object2DGraph {
	// position, orientation, speed and such	

private:

	

public:


	vector<shared_ptr<Object2D>> objects;
	glzCamera2D *camera;
	bool sort_z;
	
	Object2DGraph(glzCamera2D *cam) { camera = cam; sort_z = false; }

	template<typename T> // fun with templates
	void add(T obj)
	{
		objects.push_back(make_shared<T>(obj));
		
	}

	void draw()
	{
		glzShaderProgramPush();

		for (auto a : objects)
		{
			if (a->visible)
				a->draw(camera);
		}
			

		glzShaderProgramPop();

		return;
	}

	void update(float seconds)
	{

		for (auto a : objects)
			a->update(seconds);

		if (sort_z)
			sort(objects.begin(), objects.end(), [](const shared_ptr<Object2D>& a, const shared_ptr<Object2D>& b) {	return a.get()->z_order < b.get()->z_order;	});
	

		
		int i2 = 0;

		auto i = objects.begin();

		// o yes this is way uggly, but i think it works
		while (i < objects.end()) {

			if (objects[i2]->tobekilled)
			{
				i = objects.erase(i);
			}
			else {
				++i;
				i2++;
			}
		}

		return;
	}


	

	void set(int l, glzOBject2DSetvar type)
	{

		for (auto &a : objects)
			if (a->label == l)
				switch (type)
			{
				case glzOBject2DSetvar::ANIMATIONPLAY:
					a->current_frame = 0;
					a->frametime = 0.0f;
					a->animationstate = glzOBject2DAnimationstate::PLAYING;
					break;	

				case glzOBject2DSetvar::ANIMATIONSTOP:
					a->animationstate = glzOBject2DAnimationstate::STOPPED;
					break;

				case glzOBject2DSetvar::ANIMATIONPLAYONCE:
					a->current_frame = 0;
					a->frametime = 0.0f;
					a->animationstate = glzOBject2DAnimationstate::PLAYINGONCE;
					break;


			}
	}


	void set(int l, glzOBject2DSetvar type, bool v)
	{

		for (auto &a : objects)
			if (a->label == l)
				switch (type)
			{
				case glzOBject2DSetvar::KILL:
					a->tobekilled = v;
					break;

				case glzOBject2DSetvar::VISIBLE:
					a->visible = v;
					break;

			}




		return;
	}

	void set(int l, glzOBject2DSetvar type, int v)
	{

		for (auto &a : objects)
			if (a->label == l)
			switch (type)
			{
				case glzOBject2DSetvar::TEXTURE:
					a->texture = v;
					break;

				case glzOBject2DSetvar::CURRENT_ANIMATION:
					a->current_animation = v;
					break;

				case glzOBject2DSetvar::CURRENT_FRAME:
					a->current_frame = v;
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
					a->scale = v;
					break;

				case glzOBject2DSetvar::FRAMESPEED:
					a->framespeed = v;
					break;
				case glzOBject2DSetvar::WIDTH:
					a->width = v;
					break;
				case glzOBject2DSetvar::HEIGHT:
					a->height = v;
					break;
				case glzOBject2DSetvar::Z_LEVEL:
					a->z_order = v;
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

	void set(int l, glzOBject2DSetvar type, Object2DGraph *v)
	{
		for (auto &a : objects)
			if ((a->label == l) && (type == glzOBject2DSetvar::RENDEREGRAPH))
				a->set_r(type, v);
		return;
	}
	
	void set(int l, glzOBject2DSetvar type, glzColor v)
	{
		for (auto &a : objects)
			if ((a->label == l) && (type == glzOBject2DSetvar::BLENDCOLOR))
				a->blendcolor = v;
		return;
	}


	

	void kill(int l)
	{

		for (auto &a : objects)
			if (a->label == l)
				a->tobekilled = true;
		return;
	}



	

};




/*
obj2d_Sprite
obj2d_Sprite_Animated
obj2d_Fullscreen
obj2d_Background 
obj2d_Tiles
obj2d_Clear


unimplemented 



obj2d_Isomesh
obj2d_Hexmesh
------ these generate a bunch of sprites that are z sorted, but the object never render itself.


obj2d_Sidescroll - similar to the above but is restricted in it's dimentions
---- these use a new shader called "repeating texmap" or something equally daft


obj2d_Particles - takes a pointer to a glzSimpleParticleSystem or similar and a glzSpriteanimationList

obj2d_Object2DGraph -- i know it's a bit self referetial but it could be useful

// non obvious objects


obj2d_colorOverlay


// non rendering objects


*/




#endif /* __2dscenegraphbase__ */
