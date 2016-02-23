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

// glz geometry toolkit - for allmost all your geometry needs
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git


using namespace std;

#include "..\utilities\type.h"
#include "..\utilities\vectormath.h"
#include <vector>


#ifndef __glz_geo__
#define __glz_geo__


		//type signifies the type of data to choose from, if set at GLZ_AUTO it chooses the default settings
		//vao always returns the created VAO
		//every function that creates a VAO returns the amount of Vertics to output
		// every VAO funtion resets the vao to 0 to prevent problems unless otherwise specified

typedef struct{
	int active;
	glzVAOType type;
}vaostatus;

typedef struct
{
	glzIGTType type;
	unsigned int width;
	unsigned int height;
	unsigned int bpp;
	float x_offset;
	float y_offset;
	float z_offset;
	float scale;
	float tscale;
	glzAxis axis;
	unsigned char *data;
} image_geo_transform;

typedef struct
{
	glzTTType type;
	float u_scale;
	float v_scale;
	float u_offset;
	float v_offset;
	unsigned int atlas_width;
	unsigned int atlas_height;
	glzAxis axis;
	unsigned int firstatlas;
	unsigned int *atlas;
	glzOrigin origin;
} texture_transform;

typedef struct
{
	glzPrimitive type;
	glzMatrix matrix;
	texture_transform tt;
	float variation_1;
	float variation_2;
	unsigned int resolution_x;
	unsigned int resolution_y;
	unsigned int resolution_z;

} primitive_gen;



long glzPrimCubeverts(float *v, float *t, float *n);
void glzPrimCubeVector(vector<poly3> *pdata, int group, unsigned int sides);


image_geo_transform glzMakeIGT(glzIGTType type, unsigned int width, unsigned int height, unsigned int bpp, float x_offset, float y_offset, float z_offset, float scale, float tscale, glzAxis axis, unsigned char *data);
	
texture_transform glzMakeTT(glzTTType type, float u_scale, float v_scale, float u_offset, float v_offset, unsigned int atlas_width, unsigned int atlas_height, glzAxis axis, unsigned int firstatlas, unsigned int atlas[], glzOrigin origin);
	texture_transform glzMakeTTDefault();
	texture_transform glzMakeTTAtlas(unsigned int awidth, unsigned int aheight, unsigned int firstatlas, glzOrigin origin);
	texture_transform glzMakeTTAtlasArray(unsigned int awidth, unsigned int aheight, unsigned int atlas[], glzOrigin origin);
	texture_transform glzMakeTTAtlasCubeTBS(unsigned int awidth, unsigned int aheight, unsigned int firstatlas, glzOrigin origin);
	texture_transform glzMakeTTAtlasCubeIndface(unsigned int awidth, unsigned int aheight, unsigned int firstatlas, glzOrigin origin);
	texture_transform glzMakeTTAtlasCubeCross(unsigned int awidth, unsigned int aheight, unsigned int firstatlas, glzOrigin origin);
	texture_transform glzMakeTTVertexCoordAdopt(float uscale, float vscale, glzAxis axis, glzOrigin origin);

	primitive_gen glzMakePG(glzPrimitive type, glzMatrix matrix, texture_transform tt, float variation_1, float variation_2, unsigned int resolution_x, unsigned int resolution_y, unsigned int resolution_z);
	primitive_gen glzMakePGDefault(glzPrimitive type);
	primitive_gen glzMakePGDefaultMatrix(glzPrimitive type, glzMatrix matrix);
	primitive_gen glzMakePGAtlas(glzPrimitive type, unsigned int awidth, unsigned int aheight, unsigned int firstatlas);
	primitive_gen glzMakePGAtlasMatrix(glzPrimitive type, glzMatrix matrix, unsigned int awidth, unsigned int aheight, unsigned int firstatlas);

	void glzIGT(float *vert, image_geo_transform igt, long num);
	void glzIGT(vector<poly3> *pdata, int group, image_geo_transform igt);
	void glzVert2TexcoordRescale(float *vert, float *tex, texture_transform tt, long num);
	void glzVert2TexcoordRescale(vector<poly3> *pdata, int group, texture_transform tt);

	long glzCountFromIndexArrays(long vert_face[], int enteries);
	void glzVAOMakeFromArray(float v[], float t[], float n[], long enteties, unsigned int *vao, glzVAOType type);
	void glzVAOMakeFromVector(vector<poly3> pdata, unsigned int *vao, glzVAOType type);
	void glzVAOMakeFromVector(vector<point3> pdata, unsigned int *vao, glzVAOType type);

	

	long glzConvertVectorToArray(float *v, float *t, float *n, vector<poly3> pdata);
	long glzConvertVectorToArray(float *v, float *t, float *n, vector<point3> pdata);




	void glzDirectPointArrayRender(float v[], float t[], int E);
	void glzDirectPointVectorRender(vector<point3> pdata);
	void glzDirectCubeRender(float X, float Y, float Z, float W, float H, float D, texture_transform tt, unsigned int atlas); // does exactly you think it does

	void glzKillVAO(unsigned int vao);
	void glzKillAllVAO(void);

	void glzDrawVAO(long enteties, unsigned int vao, unsigned int type);
	void glzDrawVAO(long offset, long enteties, unsigned int vao, unsigned int type);


#endif /* __glz_geo__ */

	/* 
	not implemented, is subject to change

	// these two will output to a .vao file for later loading with glzVAOMakeFromFile, it will save the buffers and the vertexatrib indices as they are in the VAO, though not the names
	glzVAOsave(char filename[255], long enteties, unsigned int vao[], unsigned int type);
	glzVAOsaveRange(char filename[255], long offset, long enteties, unsigned int vao[], unsigned int type); 
	
	*/







