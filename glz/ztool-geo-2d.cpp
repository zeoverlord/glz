// Copyright 2013 Peter Wallstr�m
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

// glz geometry toolkit 2d- for allmost all your 2d geometry needs
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git


#include <fstream>
#include <math.h>
#include <string.h>
#include "ztool-glz.h"
#include "ztool-geo.h"
#include "ztool-geo-2d.h"
#include "ztool-shader.h"
#include <stdio.h>
#include <windows.h>													// Header File For The Windows Library
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library
#include <gl/glext.h>

using namespace std;

static PFNGLISVERTEXARRAYPROC					glIsVertexArray;
static PFNGLDRAWTEXTURENVPROC					glDrawTextureNV;
static PFNGLBINDSAMPLERPROC						glBindSampler;
static PFNGLDELETEBUFFERSPROC					glDeleteBuffers;
static PFNGLUSEPROGRAMPROC						glUseProgram;


static bool isinited_geo_2d;
static bool has_drawtexture;



void ini_geo_2d(void)
{
	has_drawtexture = false;
	glIsVertexArray = (PFNGLISVERTEXARRAYPROC)wglGetProcAddress("glIsVertexArray");
	glDrawTextureNV = (PFNGLDRAWTEXTURENVPROC)wglGetProcAddress("glDrawTextureNV");
	glBindSampler = (PFNGLBINDSAMPLERPROC)wglGetProcAddress("glBindSampler");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");



	if (glDrawTextureNV != NULL) has_drawtexture = true;

	isinited_geo_2d = true;

}





long glzCountPrimText(char *text)
{
	unsigned int c = 0, i = 0;


	while (c<strlen(text))
	{

		if ((text[c] == '\n') || (text[c] == '\t') || (text[c] == ' '))
		{
			c++;
		}
		else
		{
			i++;
			c++;
		}

	}

	return i * 6;
}

// glzPrimText is only the first itteration of this, i pan on adding support for more modern font texture generators, currently the function is compattible with "bitmap font builder" since this is
// a leftover from the old NeHe days, eventually i allso plan on adding things like truetype and things like that

long glzPrimText(char *text, float k, float *v, float *t, float *n, glzOrigin origin)
{
	
	float kern = k*0.2500f;
	float medium_kern = kern*0.75f;
	float small_kern = kern*0.6f;
	float st = 0.33f, x = 0.0f, y = 0.0f;
	float xp = 0, yp = -1.0f;
	unsigned int c = 0, iv = 0, it = 0, i = 0;
	bool newline = true;


	float textkern[256][10];
	i = 0;

	// precomputation stage
	while (i < 256)
	{
		// get the uv coordinates

		glzAtlasQuad(16, 16, i, origin, &textkern[i][0]);

		// get the  pre and post spacing
		switch (i)
		{
		case 'i':

		case 'I':
		case 'l':
		case '1':
		case 'j':
		case '!':
		case '\"':
		case '\\':

			textkern[i][8] = small_kern;//prekern
			textkern[i][9] = small_kern;//postkern
			break;

		case 'r':
		case 't':
		case 'f':
		case '7':
		case ' ':

			textkern[i][8] = medium_kern;//prekern
			textkern[i][9] = medium_kern;//postkern
			break;

		default:
			textkern[i][8] = kern;//prekern
			textkern[i][9] = kern;//postkern
			break;
		}



		i++;
	}

	i = 0;

	while (c<strlen(text))
	{

		if (text[c] == '\n') { yp -= 1; xp = 0; newline = true; c++; }
		else if (text[c] == '\t')
		{
			if (xp>90 * st)	xp = 100 * st;
			else if (xp >= 80 * st)	xp = 90 * st;
			else if (xp >= 70 * st)	xp = 80 * st;
			else if (xp >= 60 * st)	xp = 70 * st;
			else if (xp >= 50 * st)	xp = 60 * st;
			else if (xp >= 40 * st)	xp = 50 * st;
			else if (xp >= 30 * st)	xp = 40 * st;
			else if (xp >= 20 * st)	xp = 30 * st;
			else if (xp >= 10 * st)	xp = 20 * st;
			else if (xp >= 0)		xp = 10 * st;


			c++;
			newline = true;

		}
		else if (text[c] == ' ') { xp += textkern[text[c]][8];  c++; }
		else
		{
			if(!newline) xp += textkern[text[c]][8];

			newline = false;

		

			// t face 1 (124)
			t[it + 0] = textkern[text[c]][0];
			t[it + 1] = textkern[text[c]][1];

			t[it + 2] = textkern[text[c]][6];
			t[it + 3] = textkern[text[c]][7];

			t[it + 4] = textkern[text[c]][2];
			t[it + 5] = textkern[text[c]][3];

			

			// t face 2 (234)
			t[it + 6] = textkern[text[c]][2];
			t[it + 7] = textkern[text[c]][3];

			t[it + 8] = textkern[text[c]][6];
			t[it + 9] = textkern[text[c]][7];

			t[it + 10] = textkern[text[c]][4];
			t[it + 11] = textkern[text[c]][5];

			
			it += 12;


			v[iv + 0] = 0 + x + xp;
			v[iv + 1] = 0 + y + yp;
			v[iv + 2] = 0.0f;

			v[iv + 3] = 1 + x + xp;
			v[iv + 4] = 0 + y + yp;
			v[iv + 5] = 0.0f;

			v[iv + 6] = 0 + x + xp;
			v[iv + 7] = 1 + y + yp;
			v[iv + 8] = 0.0f;

			

			// v face 2 (234)

			v[iv + 9] = 0 + x + xp;
			v[iv + 10] = 1 + y + yp;
			v[iv + 11] = 0.0f;

			v[iv + 12] = 1 + x + xp;
			v[iv + 13] = 0 + y + yp;
			v[iv + 14] = 0.0f;

			v[iv + 15] = 1 + x + xp;
			v[iv + 16] = 1 + y + yp;
			v[iv + 17] = 0.0f;

			



			n[iv + 0] = 0;
			n[iv + 1] = 1;
			n[iv + 2] = 0;

			n[iv + 3] = 0;
			n[iv + 4] = 1;
			n[iv + 5] = 0;

			n[iv + 6] = 0;
			n[iv + 7] = 1;
			n[iv + 8] = 0;

			n[iv + 9] = 0;
			n[iv + 10] = 1;
			n[iv + 11] = 0;

			n[iv + 12] = 0;
			n[iv + 13] = 1;
			n[iv + 14] = 0;

			n[iv + 15] = 0;
			n[iv + 16] = 1;
			n[iv + 17] = 0;

			iv += 18;

			xp += textkern[text[c]][9];

			i++;

			c++;

		}



	}


	return i * 6;
}


long glzVAOMakeText(char text[255], float matrix[], float kern, texture_transform tt, glzOrigin textorigin, unsigned int *vao)
{
	if (!isinited_geo_2d) ini_geo_2d();

	unsigned int vaopoint;
	vaopoint = *vao;
	if (glIsVertexArray((GLuint)&vao) == GL_FALSE) glzKillVAO(vaopoint, glzVAOType::AUTO);
	float *v, *t, *n;

	long verts = glzCountPrimText(text);

	v = new float[verts * 3];
	t = new float[verts * 2];
	n = new float[verts * 3];


	glzPrimText(text, kern, v, t, n, tt.origin);



	float xo = 0, yo = 0;

	float m[16];
	glzLoadIdentity(m);


	switch (textorigin)
	{
	case glzOrigin::TOP_LEFT:
		//do nothing
		break;

	case glzOrigin::BOTTOM_LEFT:

		yo = glzScanVertexArray(v, verts, glzBoundingScan::HEIGHT);
		glzTranslatef(m, xo, -yo, 0);
		break;

	case glzOrigin::BOTTOM_RIGHT:

		xo = glzScanVertexArray(v, verts, glzBoundingScan::WIDTH);
		yo = glzScanVertexArray(v, verts, glzBoundingScan::HEIGHT);
		glzTranslatef(m, xo, -yo, 0);
		break;

	case glzOrigin::TOP_RIGHT:

		xo = glzScanVertexArray(v, verts, glzBoundingScan::WIDTH);
		glzTranslatef(m, xo, yo, 0);
		break;

	case glzOrigin::CENTERED:

		xo = glzScanVertexArray(v, verts, glzBoundingScan::WIDTH);
		yo = glzScanVertexArray(v, verts, glzBoundingScan::HEIGHT);
		glzTranslatef(m, xo*0.5f, -yo*0.5f, 0);
		break;
	}


	glzMultMatrix(matrix, m);

	glzProjectVertexArray(v, matrix, verts);
	glzVAOMakeFromArray(v, t, n, verts, vao, glzVAOType::AUTO);


	delete[] v;
	delete[] t;
	delete[] n;
	v = NULL;
	t = NULL;
	n = NULL;

	return verts;

}


long glzVAOMakeText2d(char text[255], float scale, float aspect, float kern, texture_transform tt, glzOrigin textorigin, unsigned int *vao)
{
	if (!isinited_geo_2d) ini_geo_2d();

	float m[16];

	

	unsigned int vaopoint;
	vaopoint = *vao;
	if (glIsVertexArray((GLuint)&vao) == GL_FALSE) glzKillVAO(vaopoint, glzVAOType::AUTO);
	float *v, *t, *n;

	long verts = glzCountPrimText(text);

	v = new float[verts * 3];
	t = new float[verts * 2];
	n = new float[verts * 3];


	glzPrimText(text, kern, v, t, n, tt.origin);

	// text always generate with top_left origin in bottom_left space the folowing code fixes that

	float xo = 0, yo = 0;

	glzLoadIdentity(m);
	switch (textorigin)
	{
		case glzOrigin::TOP_LEFT:
		//do nothing
			break;

		case glzOrigin::BOTTOM_LEFT:
			
			yo = glzScanVertexArray(v, verts, glzBoundingScan::HEIGHT);
			glzTranslatef(m, xo, -yo, 0);
			break;

		case glzOrigin::BOTTOM_RIGHT:
			
			xo = glzScanVertexArray(v, verts, glzBoundingScan::WIDTH);
			yo = glzScanVertexArray(v, verts, glzBoundingScan::HEIGHT);
			glzTranslatef(m, xo, -yo, 0);
			break;

		case glzOrigin::TOP_RIGHT:

			xo = glzScanVertexArray(v, verts, glzBoundingScan::WIDTH);
			glzTranslatef(m, xo, yo, 0);
			break;

		case glzOrigin::CENTERED:

			xo = glzScanVertexArray(v, verts, glzBoundingScan::WIDTH);
			yo = glzScanVertexArray(v, verts, glzBoundingScan::HEIGHT);
			glzTranslatef(m, xo*0.5f, -yo*0.5f, 0);
			break;
	}
	// get scaling right
	glzScalef(m, scale, scale*aspect, 1.0f);
	glzProjectVertexArray(v, m, verts);	
	
		
	glzVAOMakeFromArray(v, t, n, verts, vao, glzVAOType::AUTO);


	delete[] v;
	delete[] t;
	delete[] n;
	v = NULL;
	t = NULL;
	n = NULL;

	return verts;

}


void glzDirectDrawText(char text[255], float scale, float aspect, float kern, glzOrigin textorigin)
{
	unsigned int localVAO;
	long verts;
	verts = glzVAOMakeText2d(text, scale, aspect, kern, glzMakeTTAtlas(16, 16, 0, glzOrigin::BOTTOM_LEFT), textorigin, &localVAO);
	glzDrawVAO(0, verts, localVAO, GL_TRIANGLES);
	glzKillVAO(localVAO, glzVAOType::AUTO);

}

// direct rendering functions for sprites and such, mostly 2D stuff
 
void glzDrawTexture(unsigned int texture, unsigned int sampler, float X0, float Y0, float X1, float Y1, float Z, float s0, float t0, float s1, float t1)
{
	if (has_drawtexture) 
	{
		glDrawTextureNV(texture, sampler, X0, Y0, X1, Y1, Z, s0, t0, s1, t1);
		return;
	}


	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);

	float m[16];
	glzLoadIdentity(m);
	glzOrtho2DPixelspace(m, viewport[2], viewport[3], glzOrigin::BOTTOM_LEFT);

	unsigned int localVAO;

	float v[] = {
		X0, Y0, Z,
		X1, Y0, Z,
		X1, Y1, Z,
		X0, Y1, Z,
		X0, Y0, Z,
		X1, Y1, Z };

	float t[] = {
		s0, t0,
		s1, t0,
		s1, t1,
		s0, t1,
		s0, t0,
		s1, t1 };

	float n[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f };

	glzProjectVertexArray(v, m, 6);
	 
	glzVAOMakeFromArray(v, t, n, 6, &localVAO, glzVAOType::AUTO);


	GLint c_program=0;

	glGetIntegerv(GL_CURRENT_PROGRAM, &c_program);

	glzShaderUsePasstrough();

	glBindSampler(0,sampler);
	glBindTexture(GL_TEXTURE_2D, texture);

	glzDrawVAO(0, 6, localVAO, GL_TRIANGLES);
	glzKillVAO(localVAO, glzVAOType::AUTO);

	glUseProgram(c_program);

	return;
}

void glzDirectSpriteRender(float m[16], unsigned int texture, float X, float Y, float Z, float W, float H, float spriteX, float spriteY, float spriteW, float spriteH, glzOrigin origin)
{


	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	
	X -= m[12];
	Y -= m[13];

	X /= m[0];
	Y /= m[5];

	// xy is now in -1 +1 space

	X /= viewport[2] * 0.5f;
	Y /= viewport[3] * 0.5f;

	X += viewport[2] * 0.5f;
	Y += viewport[3] * 0.5f;

	//xy should now be in pixel space



	W = (W / (m[0] * (viewport[2] * 0.5f)));
	H = (H / (m[5] * (viewport[3] * 0.5f)));


	float x0 = X, y0 = Y, x1 = X + W, y1 = Y + H;




	switch (origin)
	{


	case glzOrigin::CENTERED:

		x0 = X - W*0.5f;
		x1 = X + W*0.5f;
		y0 = Y - H*0.5f;
		y1 = Y + W*0.5f;

		break;

	case glzOrigin::TOP_RIGHT:
	
		x0 = X - W;
		x1 = X;
		y0 = Y - H;
		y1 = Y;
		
		break;

	case glzOrigin::BOTTOM_RIGHT:

		x0 = X - W;
		x1 = X;
		y0 = Y;
		y1 = Y + H;

		break;

	case glzOrigin::TOP_LEFT:

		x0 = X;
		x1 = X + W;
		y0 = Y - H;
		y1 = Y;
		break;

	case glzOrigin::BOTTOM_LEFT:
	default:

		// do nothing

		break;

	}	
	
	
	glzDrawTexture(texture, 0, x0, y0, x1, y1, Z, spriteX, spriteY, spriteX + spriteW, spriteY + spriteH);
//	glzDrawVAO(0, 6, localVAO, GL_TRIANGLES);
//	glzKillVAO(localVAO, glzVAOType::AUTO);

	return;
}

void glzDirectSpriteRender(float X, float Y, float Z, float W, float H, float spriteX, float spriteY, float spriteW, float spriteH, glzOrigin origin)
{

	unsigned int localVAO;


	float vc[] = {
		X - W*0.5f, Y - H*0.5f, Z,
		X + W*0.5f, Y - H*0.5f, Z,
		X + W*0.5f, Y + H*0.5f, Z,
		X - W*0.5f, Y + H*0.5f, Z,
		X - W*0.5f, Y - H*0.5f, Z,
		X + W*0.5f, Y + H*0.5f, Z };

	float vtr[] = {
		X - W, Y - H, Z,
		X, Y - H, Z,
		X, Y, Z,
		X - W, Y, Z,
		X - W, Y - H, Z,
		X, Y, Z };

	float vbr[] = {
		X - W, Y, Z,
		X, Y, Z,
		X, Y + H, Z,
		X - W, Y + H, Z,
		X - W, Y, Z,
		X, Y + H, Z };

	float vtl[] = {
		X, Y - H, Z,
		X + W, Y - H, Z,
		X + W, Y, Z,
		X, Y, Z,
		X, Y - H, Z,
		X + W, Y, Z };

	float vbl[] = {
		X, Y, Z,
		X + W, Y, Z,
		X + W, Y + H, Z,
		X, Y + H, Z,
		X, Y, Z,
		X + W, Y + H, Z };

	float t[] = {
		spriteX, spriteY,
		spriteX + spriteW, spriteY,
		spriteX + spriteW, spriteY + spriteH,
		spriteX, spriteY + spriteH,
		spriteX, spriteY,
		spriteX + spriteW, spriteY + spriteH };

	float n[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f };

	switch (origin)
	{


	case glzOrigin::CENTERED:
		glzVAOMakeFromArray(vc, t, n, 6, &localVAO, glzVAOType::AUTO);
		break;

	case glzOrigin::TOP_RIGHT:
		glzVAOMakeFromArray(vtr, t, n, 6, &localVAO, glzVAOType::AUTO);
		break;

	case glzOrigin::BOTTOM_RIGHT:
		glzVAOMakeFromArray(vbr, t, n, 6, &localVAO, glzVAOType::AUTO);
		break;

	case glzOrigin::TOP_LEFT:
		glzVAOMakeFromArray(vtl, t, n, 6, &localVAO, glzVAOType::AUTO);
		break;

	case glzOrigin::BOTTOM_LEFT:
	default:
		glzVAOMakeFromArray(vbl, t, n, 6, &localVAO, glzVAOType::AUTO);
		break;

	}
	glzDrawVAO(0, 6, localVAO, GL_TRIANGLES);
	glzKillVAO(localVAO, glzVAOType::AUTO);

	return;
}


void glzDirectSpriteRenderAtlas(float X, float Y, float Z, float W, float H, unsigned int atlasW, unsigned int atlasH, unsigned int atlasN, glzOrigin origin)
{

	float quv[8];
	glzAtlasQuad(atlasW, atlasH, atlasN, origin, quv);
	glzDirectSpriteRender(X, Y, Z, W, H, quv[2], quv[3], quv[6] - quv[2], quv[7] - quv[3], origin);

	return;
}

void glzDirectSpriteRenderAtlasPixelPerfect(float X, float Y, float Z, int textureW, int textureH, unsigned int atlasW, unsigned int atlasH, unsigned int atlasN, glzOrigin origin)
{

	glzDirectSpriteRenderAtlas(X, Y, Z, (float)textureW / (float)atlasW, (float)textureH / (float)atlasH, atlasW, atlasH, atlasN, origin);

	return;
}

void glzDirectSpriteRenderAtlasPixelPerfectQuantized(float X, float Y, float Z, int textureW, int textureH, unsigned int atlasW, unsigned int atlasH, unsigned int atlasN, float q, glzOrigin origin)
{

	glzDirectSpriteRenderAtlas(quantize(X, q), quantize(Y, q), Z, (float)textureW / (float)atlasW, (float)textureH / (float)atlasH, atlasW, atlasH, atlasN, origin);

	return;
}