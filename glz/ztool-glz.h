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

// glz toolkit - this file contains may usefull little functions and many more useless ones
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#include "ztool-type.h"
#include "ztool-vectormath.h"

#ifndef __glz__
#define __glz__


const auto  GLZ_MIRROR_X = 1;
const auto  GLZ_MIRROR_Y = 2;
const auto  GLZ_REPEAT_X = 4;
const auto  GLZ_REPEAT_Y = 8;
const auto  GLZ_CLAMP_X = 16;
const auto  GLZ_CLAMP_Y = 32;


// color channels


const auto GLZ_RGB = 3;
const auto GLZ_RGBA = 4;



const auto PI = 3.1415926535897932384626433832795;
const auto PI_OVER_180 = 0.017453292519943295769236907684886;
const auto PI_OVER_360 = 0.0087266462599716478846184538424431;
const auto PI_OVER_180_REVERSE = 57.2957795131;



		//type signifies the type of data to choose from, if set at GLZ_AUTO it chooses the default settings



// usefull stuff


double glzRandfs(void);  //produces a value between 1 and -1
double glzRandfs(glzDistribution D);

float glzRandf(void);  //produces a value between 1 and 0
float glzRandf(glzDistribution D);


vec3 glzRandfs_vec3(void);
vec3 glzRandfs_vec3(glzDistribution D);

vert3 glzRandfs_vert3(void);
vert3 glzRandfs_vert3(glzDistribution D);

// static noise functions
float glzMersienneNoise(float seed);
float glzMersienneNoise(float seed, float x);
float glzMersienneNoise(float seed, float x, float y);
float glzMersienneNoise(float seed, float x, float y, float z);

float glzModF(float f, float m);  //float modulo function
int glzModFStep(float f, float m);  //special float modulo function that returns the number of times m fits in f
int glzTimeCounter(float *t, float m);  //timing function
float quantize(float f, float s); //quantized steping formula
float glzIntegral(float f);


// some general math functions


float glzDotproduct(float a[3], float b[3]);
double glzDotproduct(double a[3], double b[3]);
double glzDotproduct(vec3 a, vec3 b);
float glzMagnitude(float a[3]);
double glzMagnitude(double a[3]);

void glzNormalize(float *a, float le); //le is the length to normaliz to, normaly should be left at 1.0
void glzNormalize(double *a, double le);

void glzCrossproduct(float a[3], float b[3], float *r);
void glzCrossproduct(double a[3], double b[3], double *r);
vec3 glzCrossproduct(vec3 a, vec3 b);


// image functions

unsigned int glz2dTo1dImageRemap(unsigned int x, unsigned int y, unsigned int col, unsigned int step, unsigned int width, unsigned int height, bool invert);
float glzImageReadBilinear(float x, float y, unsigned int col, unsigned int step, int width, int height, unsigned int mirror, unsigned char *data, bool invert);

float glzColorConvert(float a, float b, float c, glzColorType intype, glzColorChannels outtype);

// specialty code

float glzRemapToRange(float curmin,float cur,float curmax, float rmin, float rmax);
float glzLerpRange(float curmin, float cur, float curmax, float rmin, float rmax);
float glzLerpRange2D(float curmin[2], float cur[2], float curmax[2], float rmin[2], float rmax[2]);
float glzLerpRange3D(float curmin[4], float cur[3], float curmax[4], float rmin[4], float rmax[4]);
float glzLerp(float cur, float rmin, float rmax);
float glzLerp2D(float cur[2], float rmin[2], float rmax[2]);
float glzLerp3D(float cur[3], float rmin[4], float rmax[4]);
void glzRemapToRangeArray(float curmin,float curmax, float rmin, float rmax, float *curdata, unsigned int num);


void glzCubicCurve(float pos, float l, float p1[3], float v1[3], float p2[3], float v2[3], float *ret);
void glzAtlasQuad(unsigned int xres, unsigned int yres, unsigned int atlas, glzOrigin origin, float *uvOut);
void glzAtlasAniQuad(unsigned int xres, unsigned int yres, float time, glzOrigin origin, float *uvout);
glzSprite glzAtlasQuad(unsigned int xres, unsigned int yres, unsigned int atlas, glzOrigin origin);
void glzAtlasUVarrayRemap(unsigned int atlas, unsigned int num, unsigned int aw, unsigned int ah, glzOrigin origin, float *uv);
void glzAtlasUVarrayRemap(unsigned int atlas, unsigned int aw, unsigned int ah, glzOrigin origin, vector<poly3> *p, int group);
void glzAtlasUVarrayRemap(unsigned int atlas, unsigned int aw, unsigned int ah, glzOrigin origin, vector<poly3> *p, int group, int side);
void glzAtlasUVarrayRemapRotate(unsigned int r, unsigned int atlas, unsigned int num, unsigned int aw, unsigned int ah, glzOrigin origin, float *uv);
void glzAtlasUVarrayRemapRotate(unsigned int r, unsigned int atlas, unsigned int aw, unsigned int ah, glzOrigin origin, vector<poly3> *p, int group, int side);


// matrix stuff

void glzProjectVertex(float  *vert, float Matrix[16]);
void glzProjectVertexArray(float  *vert, float Matrix[16], int num);
void glzProjectVertex(poly3 *p, float Matrix[16], int group);
void glzProjectVertex(poly3 *p, glzMatrix m, int group);
void glzProjectVertexArray(vector<poly3> *p, float Matrix[16], int group);
void glzProjectVertexArray(vector<poly3> *p, glzMatrix m, int group);


float glzScanVertexArray(float  *vert, long num, glzBoundingScan scan);
double glzScanVectorArray(vector<poly3> pdata, int group, glzBoundingScan scan);

void glzMultMatrix(float *MatrixB,float  MatrixA[16]);
void glzLoadIdentity(float *m);
void glzPerspective(float *m, float fov, float aspect,float zNear, float zFar);
void glzOrtho(float *m, float left, float right, float bottom, float top, float Znear, float Zfar);
void glzOrtho2D(float *m, float left, float right, float bottom, float top);
void glzOrtho2DPixelspace(float *m, int x, int y, glzOrigin origin);
void glzTranslatef(float *m,float x,float y, float z);
void glzScalef(float *m,float x,float y, float z);
void glzRotatef(float *m, float a, float x,float y, float z);


// Quaternion stuff
void glzLoadQuaternionIdentity(float *q);

void glzMultQuaternion(float *qa, float qb[4]);

void glzNormalizeQuaternion(float *q);

void glzRotateQuaternionf(float *q, float a, float x,float y, float z);

void glzQuaternionToMatrixf(float *m, float q[4]);


// 2D colisions
unsigned int glzPhys2DColideBox2(float x1,float y1,float r1, float x2,float y2,float r2);


#endif /* __glz__ */





