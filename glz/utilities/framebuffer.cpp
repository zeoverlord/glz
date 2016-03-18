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

// common glz framebuffer class
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

using namespace std;

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>
#include "type.h"
#include "glz.h"
#include "vectormath.h"
#include "framebuffer.h"


static PFNGLBINDFRAMEBUFFERPROC              glBindFramebuffer;
static PFNGLGENFRAMEBUFFERSPROC              glGenFramebuffers;
static PFNGLGENRENDERBUFFERSPROC             glGenRenderbuffers;
static PFNGLFRAMEBUFFERTEXTURE2DPROC         glFramebufferTexture2D;

static PFNGLBINDRENDERBUFFERPROC              glBindRenderbuffer;
static PFNGLRENDERBUFFERSTORAGEPROC             glRenderbufferStorage;
static PFNGLFRAMEBUFFERRENDERBUFFERPROC         glFramebufferRenderbuffer;



void glzFrameBuffer::init(int width, int height, bool inHasColorTexture, bool inHhasDepthTexture)
{


	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");


	hasColorTexture = inHasColorTexture;
	hasDepthTexture = inHhasDepthTexture;



	glGenFramebuffers(1, &frameBufferHandle);

	if(hasColorTexture)
		glGenTextures(1, &ColorBufferHandle);
	else
		glGenRenderbuffers(1, &ColorBufferHandle);

	if(hasDepthTexture)
		glGenTextures(1, &DepthBufferHandle);
	else
		glGenRenderbuffers(1, &DepthBufferHandle);


	glBindFramebuffer(GL_FRAMEBUFFER_EXT, frameBufferHandle);


	if(hasColorTexture)
	{
		glBindTexture(GL_TEXTURE_2D, ColorBufferHandle);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_INT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorBufferHandle, 0);
	}
	else
	{
		glBindRenderbuffer(GL_RENDERBUFFER, ColorBufferHandle);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER_EXT, ColorBufferHandle);
	}

	if(hasDepthTexture)
	{
		glBindTexture(GL_TEXTURE_2D, DepthBufferHandle);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_RGBA, GL_INT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthBufferHandle, 0);
	}
	else
	{
		glBindRenderbuffer(GL_RENDERBUFFER, DepthBufferHandle);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBufferHandle);
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


void glzFrameBuffer::startRendering()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, frameBufferHandle);
}

void glzFrameBuffer::stopRendering()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void glzFrameBuffer::bindColorBuffer()
{
	if(hasColorTexture)
	{
		glBindTexture(GL_TEXTURE_2D, ColorBufferHandle);
	}
}

void glzFrameBuffer::bindDepthBuffer()
{
	if(hasDepthTexture)
	{
		glBindTexture(GL_TEXTURE_2D, DepthBufferHandle);
	}
}


unsigned int glzFrameBuffer::getColorBufferHandle()
{
	if(hasColorTexture)
	{
		return ColorBufferHandle;
	}
	return 0;
}

unsigned int glzFrameBuffer::getDepthBufferHandle()
{
	if(hasDepthTexture)
	{
		return DepthBufferHandle;
	}
	return 0;
}

