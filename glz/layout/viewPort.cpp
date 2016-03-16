// Copyright 2016 Peter Wallstr�m
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

// glz viewport
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>
#include "ViewPort.h"



void glzViewPort::resizeView(void)
{
	float tempLeft = left;
	float tempRight = right;
	float tempTop = top;
	float tempBottom = bottom;

	float tempWidth = right - left;
	float tempHeight = top - bottom;


	float resizedTempWidth = tempWidth * width;
	float resizedTempHeight = tempHeight * height;

	if(ReferenceOrigin == glzOrigin::TOP_LEFT)
	{
		right = tempLeft + resizedTempWidth;
		top = tempBottom + resizedTempHeight;
	}

	if(ReferenceOrigin == glzOrigin::TOP_RIGHT)
	{
		left = tempRight - resizedTempWidth;
		top = tempBottom + resizedTempHeight;
	}
	if(ReferenceOrigin == glzOrigin::BOTTOM_LEFT)
	{
		right = tempLeft + resizedTempWidth;
		bottom = tempTop - resizedTempHeight;
	}

	if(ReferenceOrigin == glzOrigin::BOTTOM_RIGHT)
	{
		left = tempRight - resizedTempWidth;
		bottom = tempTop - resizedTempHeight;
	}

	if(ReferenceOrigin == glzOrigin::CENTERED)
	{
		right = tempLeft + (resizedTempWidth*0.5f);
		top = tempBottom + (resizedTempHeight*0.5f);

		left = tempRight - (resizedTempWidth*0.5f);
		bottom = tempTop - (resizedTempHeight*0.5f);
	}

	aspect = resizedTempWidth / resizedTempHeight;
}


void glzViewPort::update()
{
	if(parent != nullptr)
	{
		copyParentView();
		pushInPaddingParent();
	}	
	resizeView();
}

glzMatrix glzViewPort::returnOrthoMatrix()
{
	glzMatrix m;

	m.LoadIdentity();
	m.ortho2D(left,right,bottom,top);
	return m;
}

glzSprite glzViewPort::returnSprite()
{
	return glzSprite(tex2(left, right), tex2(bottom, top), 0.0f);
}