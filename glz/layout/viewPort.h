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

// glz vieport
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#pragma once


#include "..\utilities\type.h"
#include "..\utilities\glz.h"
#include "..\utilities\resourcemanager.h"
#include "..\utilities\vectormath.h"
#include <vector>
#include <memory>

// how it works
// at a start the viewport its inside it's parent window
// if width is wider than what the origin coordinate allows then the window will be resized

class glzViewPort
{
private:
	float left;
	float right;
	float top;
	float bottom;

	float aspect;

	float paddingLeft;
	float paddingRight;
	float paddingTop;
	float paddingBottom;

	float width;  //percentage of the parent to cover
	float height;
	float originX;
	float originY;

	glzOrigin ReferenceOrigin;

	shared_ptr<glzViewPort> parent;

public:

	glzViewPort(float inAspect=1.0f, float padding=1.0f)
	{
		left = -inAspect*0.5f;
		right = inAspect*0.5f;
		top = 0.5f;
		bottom = -0.5f;

		aspect = inAspect;

		paddingLeft = -inAspect*padding;
		paddingRight = inAspect*padding;
		paddingTop = padding;
		paddingBottom = -padding;

		width = 1.0f;  
		height = 1.0f;

		originX = 0.0f;
		originY = 0.0f;

		ReferenceOrigin=glzOrigin::TOP_LEFT;
	}
	~glzViewPort(){ parent = nullptr; }

	void setParent(shared_ptr<glzViewPort> inParent)
	{
		parent = inParent;
		update();
	}
	
	void clearParent()
	{
		parent = nullptr;
		update();
	}

	void setSize(float inWidth, float inHeight)
	{
		width = inWidth;
		height = inHeight;
		update();
	}

	void setOriginOffset(float inOriginX, float inOriginY)
	{
		originX = inOriginX;
		originY = inOriginY;
		update();
	}


	shared_ptr<glzViewPort> getParent()	{ return parent; }
	float getWidth(){return width;}
	float getHeight(){ return height; }
	float getOriginX(){ return originX; }
	float getOriginY(){ return originY; }
	float getAspect(){ return aspect; }

	void update();
	void setupRendering();

};