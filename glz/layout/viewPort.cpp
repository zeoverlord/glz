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

// glz viewport
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>
#include "ViewPort.h"



void glzViewport::resizeView(void)
{
	const float	tempLeft = left;
	const float	tempRight = right;
	const float	tempTop = top;
	const float	tempBottom = bottom;
	const float	tempWidth = right - left;
	const float	tempHeight = top - bottom;
	const float resizedTempWidth = tempWidth * width;
	const float resizedTempHeight = tempHeight * height;

	
	if(ReferenceOrigin == glzOrigin::TOP_LEFT)
	{
		right = tempLeft + resizedTempWidth;
		left = tempLeft;
		top = tempTop;
		bottom = tempTop - resizedTempHeight;	
	}

	if(ReferenceOrigin == glzOrigin::TOP_RIGHT)
	{
		right = tempRight ;
		left = tempRight - resizedTempWidth;
		top = tempTop;
		bottom = tempTop - resizedTempHeight;
	}

	if(ReferenceOrigin == glzOrigin::BOTTOM_LEFT)
	{
		right = tempLeft + resizedTempWidth;
		left = tempLeft;
		top = tempBottom + resizedTempHeight;
		bottom = tempBottom;
	}

	if(ReferenceOrigin == glzOrigin::BOTTOM_RIGHT)
	{
		right = tempRight;
		left = tempRight - resizedTempWidth;
		top = tempBottom + resizedTempHeight;
		bottom = tempBottom;
	}

	if(ReferenceOrigin == glzOrigin::CENTERED)
	{
		right = ((tempRight + tempLeft)*0.5) + (resizedTempWidth*0.5f);
		left = ((tempRight + tempLeft)*0.5) - (resizedTempWidth*0.5f);

		top = ((tempTop + tempBottom)*0.5) + (resizedTempHeight*0.5f);
		bottom = ((tempTop + tempBottom)*0.5) - (resizedTempHeight*0.5f);
	}

	if (ReferenceOrigin == glzOrigin::LEFT)
	{
		right = tempLeft + resizedTempWidth;
		left = tempLeft;
		top = ((tempTop + tempBottom)*0.5) + (resizedTempHeight*0.5f);
		bottom = ((tempTop + tempBottom)*0.5) - (resizedTempHeight*0.5f);
	}

	if (ReferenceOrigin == glzOrigin::RIGHT)
	{
		right = tempRight;
		left = tempRight - resizedTempWidth;
		top = ((tempTop + tempBottom)*0.5) + (resizedTempHeight*0.5f);
		bottom = ((tempTop + tempBottom)*0.5) - (resizedTempHeight*0.5f);
	}

	if (ReferenceOrigin == glzOrigin::TOP)
	{
		right = ((tempRight + tempLeft)*0.5) + (resizedTempWidth*0.5f);
		left = ((tempRight + tempLeft)*0.5) - (resizedTempWidth*0.5f);
		top = tempTop;
		bottom = tempTop - resizedTempHeight;
	}

	if (ReferenceOrigin == glzOrigin::BOTTOM)
	{
		right = ((tempRight + tempLeft)*0.5) + (resizedTempWidth*0.5f);
		left = ((tempRight + tempLeft)*0.5) - (resizedTempWidth*0.5f);
		top = tempBottom + resizedTempHeight;
		bottom = tempBottom;
	}

	aspect = resizedTempWidth / resizedTempHeight;
}


void glzViewport::update()
{
	if(parent != nullptr)
	{
		copyParentView();
		pushInPaddingParent();
	}	
	resizeView();
}

void glzViewport::setupViewport()
{
	float oAspect = (float)displayWidth / (float)displayHeight;

	int newDisplayX = ((left + (oAspect*0.5)) / oAspect) * displayWidth;
	int newDisplayY = (bottom + 0.5) * displayHeight;	
	int newDisplayWidth = (((right + (oAspect*0.5)) / oAspect) * displayWidth) - newDisplayX;
	int newDisplayHeight = ((top+0.5) * displayHeight) - newDisplayY;

	glViewport(newDisplayX, newDisplayY, newDisplayWidth, newDisplayHeight);
	
}

void glzViewport::disableViewport()
{
	glViewport(0, 0, displayWidth, displayHeight);
}



glzMatrix glzViewport::returnOrthoMatrix()
{
	glzMatrix m;

	m.LoadIdentity();		
	m.ortho2D(-aspect*0.5f, aspect*0.5f, -0.5f, 0.5f);
	return m;
}

glzSprite glzViewport::returnSprite()
{
	return glzSprite(tex2(left, right), tex2(bottom, top), 0.0f);
}