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
#include "..\utilities\sprite.h"
#include "..\utilities\resourcemanager.h"
#include "..\utilities\vectormath.h"
#include <vector>
#include <memory>

// how it works
// at a start the viewport its inside it's parent window
// if width is wider than what the origin coordinate allows then the window will be resized

class glzViewport
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

	int displayX;
	int displayY;
	int displayWidth;
	int displayHeight;
	

	glzOrigin ReferenceOrigin;

	glzViewport *parent;

private:

	void copyParentView(void)
	{
		left = parent->left;
		right = parent->right;
		top = parent->top;
		bottom = parent->bottom;

		displayX = parent->getDisplayX();
		displayY = parent->getDisplayY();
		displayWidth = parent->getDisplayWidth();
		displayHeight = parent->getDisplayHeigth();
	}
	void pushInPaddingParent(void)
	{
		left += parent->paddingLeft;
		right += parent->paddingRight;
		top += parent->paddingTop;
		bottom += parent->paddingBottom;
	}

	void resizeView(void);
	

public:

	glzViewport(float inAspect=1.0f, float padding=0.0f)
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

		displayX=0; 
		displayY=0;

		displayWidth = 100; // non realistic screen, but should not cause problems if abused
		displayHeight = 100;

		ReferenceOrigin=glzOrigin::TOP_LEFT;
	}
	~glzViewport(){ parent = nullptr; }

	void init(float inAspect = 1.0f, float padding = 0.0f)
	{
		left = -inAspect*0.5f;
		right = inAspect*0.5f;
		top = 0.5f;
		bottom = -0.5f;

		aspect = inAspect;

		paddingLeft = inAspect*padding;
		paddingRight = -inAspect*padding;
		paddingTop = -padding;
		paddingBottom = padding;

		width = 1.0f;
		height = 1.0f;

		originX = 0.0f;
		originY = 0.0f;

		displayX = 0;
		displayY = 0;

		displayWidth = 100; // non realistic screen, but should not cause problems if abused
		displayHeight = 100;

		ReferenceOrigin = glzOrigin::TOP_LEFT;
	}

	void setParent(glzViewport *inParent)
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

	void setDisplay(int inX, int inY, int inWidth, int inHeight)
	{
		displayX = inX;
		displayY = inY;
		displayWidth = inWidth; 
		displayHeight = inHeight;
		update();
	}

	int getDisplayX(void) { return displayX; }
	int getDisplayY(void) { return displayY; }
	int getDisplayWidth(void) { return displayWidth; }
	int getDisplayHeigth(void) { return displayHeight; }



	void setOrigin(glzOrigin origin) { ReferenceOrigin = origin; update(); }
	glzOrigin getOrigin(void) { return ReferenceOrigin; }

	glzViewport *getParent()	{ return parent; }
	float getWidth(){return width;}
	float getHeight(){ return height; }
	float getOriginX(){ return originX; }
	float getOriginY(){ return originY; }
	float getAspect(){ return aspect; }

	void update();
	void setupViewport(); //set up cliping
	void disableViewport();
	glzMatrix returnOrthoMatrix();
	glzSprite returnSprite();

};