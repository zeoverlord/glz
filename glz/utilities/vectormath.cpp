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

// common glz vector and matrix classes
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

using namespace std;

#include "type.h"
#include "glz.h"
#include "vectormath.h"

float vert3::distance(vert3 a)
{ 
	//double r;
	vec3 d = this->vectorTo(a);
	d.absolute();
	return d.magnitude();
	//r = sqrt((d.x*d.x) + (d.y*d.y) + (d.y*d.y));
	//return r;
}

vec3 vert3::vectorTo(vert3 b)
{
	vec3 c;
	c.x = b.x - this->x;
	c.y = b.y - this->y;
	c.z = b.z - this->z;
	
	return c;

}

vec3 vert3::vectorPointsTo(vert3 b)
{
	vec3 c;
	c.x = b.x - this->x;
	c.y = b.y - this->y;
	c.z = b.z - this->z;

	c.normalize(1.0);
	return c;

}

void vert3::project(glzMatrix m)
{

	double xt = (x * m.m[0]) + (y * m.m[4]) + (z * m.m[8]) + m.m[12];
	double yt = (x * m.m[1]) + (y * m.m[5]) + (z * m.m[9]) + m.m[13];
	double zt = (x * m.m[2]) + (y * m.m[6]) + (z * m.m[10]) + m.m[14];

	x = xt;
	y = yt;
	z = zt;

}


void vec3::reflect(vec3 b)
{
	vec3 a(x, y, z);
	vec3 res = -2 * (a.dot(b))*b + a;
	x = res.x;
	y = res.y;
	z = res.z;
}

void vec3::project(glzMatrix m)
{

	double xt = (x * m.m[0]) + (y * m.m[4]) + (z * m.m[8]);
	double yt = (x * m.m[1]) + (y * m.m[5]) + (z * m.m[9]);
	double zt = (x * m.m[2]) + (y * m.m[6]) + (z * m.m[10]);

	x = xt;
	y = yt;
	z = zt;

}



void glzMatrix::LoadIdentity(void)
{
	double b[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };
	int v[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	for (auto i : v)
		m[i] = b[i];

	return;
}

void glzMatrix::LoadIdentityzero(void)
{
	double b[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	int v[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	for (auto i : v)
		m[i] = b[i];

	return;
}



glzMatrix glzMatrix::operator*= (glzMatrix b)
{
	glzMatrix  a(m);
	int v[] = { 0, 1, 2, 3 };

	for (auto i : v){  //Cycle through each vector of first matrix.
		m[i * 4 + 0] = b.m[i * 4] * a.m[0] + b.m[i * 4 + 1] * a.m[4] + b.m[i * 4 + 2] * a.m[8] + b.m[i * 4 + 3] * a.m[12];
		m[i * 4 + 1] = b.m[i * 4] * a.m[1] + b.m[i * 4 + 1] * a.m[5] + b.m[i * 4 + 2] * a.m[9] + b.m[i * 4 + 3] * a.m[13];
		m[i * 4 + 2] = b.m[i * 4] * a.m[2] + b.m[i * 4 + 1] * a.m[6] + b.m[i * 4 + 2] * a.m[10] + b.m[i * 4 + 3] * a.m[14];
		m[i * 4 + 3] = b.m[i * 4] * a.m[3] + b.m[i * 4 + 1] * a.m[7] + b.m[i * 4 + 2] * a.m[11] + b.m[i * 4 + 3] * a.m[15];
	}


	return glzMatrix(m);
}


void glzMatrix::transferMatrix(float *b)
{
	int v[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	for (auto i : v)
		b[i] = (float)m[i];
}

void glzMatrix::translate(float x, float y, float z)
{
	glzMatrix b;

	b.m[0] = 1;
	b.m[1] = 0;
	b.m[2] = 0;
	b.m[3] = 0;

	b.m[4] = 0;
	b.m[5] = 1;
	b.m[6] = 0;
	b.m[7] = 0;

	b.m[8] = 0;
	b.m[9] = 0;
	b.m[10] = 1;
	b.m[11] = 0;

	b.m[12] = x;
	b.m[13] = y;
	b.m[14] = z;
	b.m[15] = 1;

	this->multThis(b);
	return;

}




void glzMatrix::scale(float x, float y, float z)
{
	{

		glzMatrix b;



		b.m[0] = x;
		b.m[1] = 0;
		b.m[2] = 0;
		b.m[3] = 0;

		b.m[4] = 0;
		b.m[5] = y;
		b.m[6] = 0;
		b.m[7] = 0;

		b.m[8] = 0;
		b.m[9] = 0;
		b.m[10] = z;
		b.m[11] = 0;

		b.m[12] = 0;
		b.m[13] = 0;
		b.m[14] = 0;
		b.m[15] = 1;

		this->multThis(b);

		return;

	}

}

void glzMatrix::rotate(float a, float x, float y, float z)
{

	double angle = a*PI_OVER_180;
	glzMatrix b;

	b.m[0] = 1 + (1 - cos(angle))*(x*x - 1);
	b.m[1] = -z*sin(angle) + (1 - cos(angle))*x*y;
	b.m[2] = y*sin(angle) + (1 - cos(angle))*x*z;
	b.m[3] = 0;

	b.m[4] = z*sin(angle) + (1 - cos(angle))*x*y;
	b.m[5] = 1 + (1 - cos(angle))*(y*y - 1);
	b.m[6] = -x*sin(angle) + (1 - cos(angle))*y*z;
	b.m[7] = 0;

	b.m[8] = -y*sin(angle) + (1 - cos(angle))*x*z;
	b.m[9] = x*sin(angle) + (1 - cos(angle))*y*z;
	b.m[10] = 1 + (1 - cos(angle))*(z*z - 1);
	b.m[11] = 0;

	b.m[12] = 0;
	b.m[13] = 0;
	b.m[14] = 0;
	b.m[15] = 1;

	this->multThis(b);

	return;
}

void glzMatrix::perspective(float fov, float aspect, float zNear, float zFar)
{
	const double h = 1.0f / tan(fov*PI_OVER_360);
	double neg_depth = zNear - zFar;

	glzMatrix b;

	b.m[0] = h / aspect;
	b.m[1] = 0;
	b.m[2] = 0;
	b.m[3] = 0;

	b.m[4] = 0;
	b.m[5] = h;
	b.m[6] = 0;
	b.m[7] = 0;

	b.m[8] = 0;
	b.m[9] = 0;
	b.m[10] = (zFar + zNear) / neg_depth;
	b.m[11] = -1;

	b.m[12] = 0;
	b.m[13] = 0;
	b.m[14] = 2.0f*(zNear*zFar) / neg_depth;
	b.m[15] = 0;

	this->multThis(b);

	return;

}

void glzMatrix::ortho(float left, float right, float bottom, float top, float Znear, float Zfar)
{

	glzMatrix b;

	b.m[0] = 2 / (right - left);
	b.m[1] = 0;
	b.m[2] = 0;
	b.m[3] = -((right + left) / (right - left));

	b.m[4] = 0;
	b.m[5] = 2 / (top - bottom);
	b.m[6] = 0;
	b.m[7] = -((top + bottom) / (top - bottom));

	b.m[8] = 0;
	b.m[9] = 0;
	b.m[10] = 2 / (Zfar - Znear);
	b.m[11] = -((Zfar + Znear) / (Zfar - Znear));

	b.m[12] = 0;
	b.m[13] = 0;
	b.m[14] = 0;
	b.m[15] = 1;

	this->multThis(b);

	return;
}


void glzMatrix::ortho2D(float left, float right, float bottom, float top)
{

	double Znear = -100.0f, Zfar = 100.0f;
	glzMatrix b;

	b.m[0] = 2 / (right - left);
	b.m[1] = 0;
	b.m[2] = 0;
	b.m[3] = -((right + left) / (right - left));

	b.m[4] = 0;
	b.m[5] = 2 / (top - bottom);
	b.m[6] = 0;
	b.m[7] = -((top + bottom) / (top - bottom));

	b.m[8] = 0;
	b.m[9] = 0;
	b.m[10] = 2 / (Zfar - Znear);
	b.m[11] = -((Zfar + Znear) / (Zfar - Znear));

	b.m[12] = 0;
	b.m[13] = 0;
	b.m[14] = 0;
	b.m[15] = 1;

	this->multThis(b);

	return;

}

void glzMatrix::ortho2DPixelspace(int x, int y, glzOrigin origin)
{

	glzMatrix b;
	b.LoadIdentity();


	if (origin == glzOrigin::BOTTOM_LEFT)
	{
		b.ortho2D(-(x*0.5f), (x*0.5f), -(y*0.5f), (y*0.5f));
		b.translate(-(x*0.5f), -(y*0.5f), 0);
	}

	if (origin == glzOrigin::TOP_LEFT)
	{
		b.ortho2D((x*0.5f), -(x*0.5f), -(y*0.5f), (y*0.5f));
		b.translate(-(x*0.5f), (y*0.5f), 0);
	}

	this->multThis(b);

	return;

}

void glzMatrix::loadQuanternion(float q[4])
{
	glzMatrix b;

	b.m[0] = 1 - (2 * q[2] * q[2]) - (2 * q[3] * q[3]);
	b.m[1] = (2 * q[1] * q[2]) + (2 * q[0] * q[3]);
	b.m[2] = (2 * q[1] * q[3]) - (2 * q[0] * q[2]);
	b.m[3] = 0;

	b.m[4] = (2 * q[1] * q[2]) - (2 * q[0] * q[3]);
	b.m[5] = 1 - (2 * q[1] * q[1]) - (2 * q[3] * q[3]);
	b.m[6] = (2 * q[2] * q[3]) + (2 * q[0] * q[1]);
	b.m[7] = 0;

	b.m[8] = (2 * q[1] * q[3]) + (2 * q[0] * q[2]);
	b.m[9] = (2 * q[2] * q[3]) - (2 * q[0] * q[1]);
	b.m[10] = 1 - (2 * q[1] * q[1]) - (2 * q[2] * q[2]);
	b.m[11] = 0;

	b.m[12] = 0;
	b.m[13] = 0;
	b.m[14] = 0;
	b.m[15] = 1;

	this->multThis(b);
	return;
}


void glzMatrix::loadQuanternion(glzQuaternion b)
{
	glzMatrix a;

	a.m[0] = 1 - (2 * b.y * b.y) - (2 * b.z * b.z);
	a.m[1] = (2 * b.x * b.y) + (2 * b.w * b.z);
	a.m[2] = (2 * b.x * b.z) - (2 * b.w * b.y);
	a.m[3] = 0;

	a.m[4] = (2 * b.x * b.y) - (2 * b.w * b.z);
	a.m[5] = 1 - (2 * b.x * b.x) - (2 * b.z * b.z);
	a.m[6] = (2 * b.y * b.z) + (2 * b.w * b.x);
	a.m[7] = 0;

	a.m[8] = (2 * b.x * b.z) + (2 * b.w * b.y);
	a.m[9] = (2 * b.y * b.z) - (2 * b.w * b.x);
	a.m[10] = 1 - (2 * b.x * b.x) - (2 * b.y * b.y);
	a.m[11] = 0;

	a.m[12] = 0;
	a.m[13] = 0;
	a.m[14] = 0;
	a.m[15] = 1;

	this->multThis(a);
	return;
}


void glzQuaternion::multQuaternion(glzQuaternion b)
{
	glzQuaternion  nq;

	nq.w = -x * b.x - y * b.y - z * b.z + w * b.w;
	nq.x = x * b.w + y * b.z - z * b.y + w * b.x;
	nq.y = -x * b.z + y * b.w + z * b.x + w * b.y;
	nq.z = x * b.y - y * b.x + z * b.w + w * b.z;
	w = nq.w;
	x = nq.x;
	y = nq.y;
	z = nq.z;

	return;
}

void glzQuaternion::rotate(double a, double x, double y, double z)
{
	double angle = -a*PI_OVER_360;
	glzQuaternion q2;

	q2.w = cos(angle);
	q2.x = sin(angle)*x;
	q2.y = sin(angle)*y;
	q2.z = sin(angle)*z;

	this->normalize();
	this->multQuaternion(q2);

	return;
}


vec3 poly3::getFaceNormal()
{

	vec3 v1 = a.v.vectorTo(b.v);
	vec3 v2 = a.v.vectorTo(c.v);

	vec3 nv;
	nv.crossproduct(v2, v1);
	nv.normalize(1.0);

	return nv;



}

void poly3::generateNormal()
{
	vec3 fn(this->getFaceNormal());
	a.n = fn;
	b.n = fn;
	c.n = fn;
}


void poly3::generateTexture(double scale)
{
	vec3 fn(this->getFaceNormal());
	int l = 0;
	double largest = abs(fn.x);
	if (abs(fn.y) > largest) { l = 1; largest = abs(fn.y); }
	if (abs(fn.z) > largest) { l = 2; largest = abs(fn.z); }

	if (l == 0)
	{
		a.t.u = a.v.y / scale;
		a.t.v = a.v.z / scale;
		b.t.u = b.v.y / scale;
		b.t.v = b.v.z / scale;
		c.t.u = c.v.y / scale;
		c.t.v = c.v.z / scale;
	}

	if (l == 1)
	{
		a.t.u = a.v.x / scale;
		a.t.v = a.v.z / scale;
		b.t.u = b.v.x / scale;
		b.t.v = b.v.z / scale;
		c.t.u = c.v.x / scale;
		c.t.v = c.v.z / scale;
	}

	if (l == 2)
	{
		a.t.u = a.v.x / scale;
		a.t.v = a.v.y / scale;
		b.t.u = b.v.x / scale;
		b.t.v = b.v.y / scale;
		c.t.u = c.v.x / scale;
		c.t.v = c.v.y / scale;
	}
}



plane3::plane3(poly3 a)
{
	vec3 fn = a.getFaceNormal(); // first get the normal
	n = fn;
	d = (a.a.v.x*fn.x)+(a.a.v.y*fn.y)+(a.a.v.z*fn.z);
}

bool plane3::is_infront(vert3 a)
{
			
	double d2 = (a.x*n.x) + (a.y*n.y) + (a.z*n.z);

	if (d2>d) return true;			
	else return false;
}

bool plane3::can_intersect(vert3 a, vec3 b)
{

	if ((this->is_infront(a)) && (b.dot(this->n) <= 0.0)) return true;

	if ((!this->is_infront(a)) && (b.dot(this->n) >= 0.0)) return true;

	return false;
}


vert3 plane3::intesect_line(line3 a)
{
	vert3 origin;
	float nDotA = n.dot(origin.vectorTo(a.p));
	float nDotBA = n.dot(a.n);

	return a.p + (((d - nDotA) / nDotBA) * a.n);  // hope it works
}

//vert3 plane3::intesect_line(line3 a)
//{


//	n.x* (a.p.x + a.n.x*t) + n.y *(a.p.y - a.n.y*t) - n.z* (a.p.z + a.n.z*t) = d.
//}



void poly3::tempAddNormalToVertex()

{
	vec3 fn(this->getFaceNormal());
	a.v += a.n;
	b.v += b.n;
	c.v += c.n;
}



glzSprite::glzSprite( glzOrigin origin) // grid atlas initialization
{

	glzSprite quad = glzAtlasQuad(1, 1, 0, origin);
	a = quad.a;
	b = quad.b;
	c = quad.c;
	d = quad.d;
	depth = 0.0;

}


glzSprite::glzSprite(unsigned int xdim, unsigned int ydim, unsigned int atlas, float depthin) // grid atlas initialization
{

	glzSprite quad = glzAtlasQuad(xdim, ydim, atlas, glzOrigin::BOTTOM_LEFT);
	a = quad.a;
	b = quad.b;
	c = quad.c;
	d = quad.d;
	depth = depthin;

}

glzSprite::glzSprite(unsigned int xdim, unsigned int ydim, unsigned int atlas, float depthin, glzOrigin origin) // grid atlas initialization
{

	glzSprite quad = glzAtlasQuad(xdim, ydim, atlas, origin);
	a = quad.a;
	b = quad.b;
	c = quad.c;
	d = quad.d;
	depth = depthin;

}

void glzSprite::make_polygons(vector<poly3> *pdata, float x, float y, float width, float height, int group, int atlas)
{

	poly3 p1, p2;

	p1.atlas = atlas;
	p2.atlas = atlas;

	p1.group = group;
	p2.group = group;

	vec3 n(0.0, 1.0, 0.0);

	p1.a.n = n;
	p1.b.n = n;
	p1.c.n = n;

	p2.a.n = n;
	p2.b.n = n;
	p2.c.n = n;

	p1.a.t = a;
	p1.b.t = b;
	p1.c.t = c;

	p2.a.t = c;
	p2.b.t = b;
	p2.c.t = d;

	p1.a.v = vert3(0.0 + x, 0.0 + y, 0.0);
	p1.b.v = vert3(width + x, 0.0 + y, 0.0);
	p1.c.v = vert3(0.0 + x, height + y, 0.0);

	p2.a.v = vert3(0.0 + x, height + y, 0.0);
	p2.b.v = vert3(width + x, 0.0 + y, 0.0);
	p2.c.v = vert3(width + x, height + y, 0.0);

	pdata->push_back(p1);
	pdata->push_back(p2);

}


void glzSprite::make_polygons(vector<poly3> *pdata, float x, float y, float width, float height, int group, int atlas, glzMatrix m)
{

	poly3 p1, p2;

	p1.atlas = atlas;
	p2.atlas = atlas;

	p1.group = group;
	p2.group = group;

	vec3 n(0.0, 1.0, 0.0);

	p1.a.n = n;
	p1.b.n = n;
	p1.c.n = n;

	p2.a.n = n;
	p2.b.n = n;
	p2.c.n = n;

	p1.a.t = a;
	p1.b.t = b;
	p1.c.t = c;

	p2.a.t = c;
	p2.b.t = b;
	p2.c.t = d;

	p1.a.v = vert3(0.0 + x, 0.0 + y, 0.0);
	p1.b.v = vert3(width + x, 0.0 + y, 0.0);
	p1.c.v = vert3(0.0 + x, height + y, 0.0);

	p2.a.v = vert3(0.0 + x, height + y, 0.0);
	p2.b.v = vert3(width + x, 0.0 + y, 0.0);
	p2.c.v = vert3(width + x, height + y, 0.0);

	glzProjectVertex(&p1, m, group);
	glzProjectVertex(&p2, m, group);

	pdata->push_back(p1);
	pdata->push_back(p2);

}

void glzSprite::make_polygons(vector<poly3> *pdata, float x, float y, float width, float height, int group, int atlas, glzOrigin origin, glzMatrix m)
{

	poly3 p1, p2;
	vector<poly3> pdata_temp;

	p1.atlas = atlas;
	p2.atlas = atlas;

	p1.group = group;
	p2.group = group;

	vec3 n(0.0, 1.0, 0.0);

	p1.a.n = n;
	p1.b.n = n;
	p1.c.n = n;

	p2.a.n = n;
	p2.b.n = n;
	p2.c.n = n;

	p1.a.t = a;
	p1.b.t = b;
	p1.c.t = c;

	p2.a.t = c;
	p2.b.t = b;
	p2.c.t = d;

	p1.a.v = vert3(0.0 + x, 0.0 + y, 0.0);
	p1.b.v = vert3(width + x, 0.0 + y, 0.0);
	p1.c.v = vert3(0.0 + x, height + y, 0.0);

	p2.a.v = vert3(0.0 + x, height + y, 0.0);
	p2.b.v = vert3(width + x, 0.0 + y, 0.0);
	p2.c.v = vert3(width + x, height + y, 0.0);




	//glzRecenterVectorArray(&p1, group, origin);
	//glzRecenterVectorArray(&p2, group, origin);

	//glzProjectVertex(&p1, m, group);
	//glzProjectVertex(&p2, m, group);

	pdata_temp.push_back(p1);
	pdata_temp.push_back(p2);

	
	glzRecenterVectorArray(&pdata_temp, group, origin);
	glzProjectVertexArray(&pdata_temp, m, group);
	



	//glzProjectVertex(&p1, m, group);
	
	pdata->push_back(pdata_temp.at(0));
	pdata->push_back(pdata_temp.at(1));

}

glzSpriteList::glzSpriteList(int w, int h) // direct initialization with preset
{
	int i = 0;

	while (i < w*h)
	{
		map.push_back(glzSprite(w, h, i, 0.0));
		i++;
	}



	//while (i < w*h) { map.push_back(glzAtlasQuad(w, h, i, glzOrigin::BOTTOM_LEFT));	i++; }
}

glzSpriteList::glzSpriteList(int w, int h, int v[], int n) // direct initialization with preset
{
	int i = 0;

	while (i < n)
	{
		map.push_back(glzSprite(w, h, v[i], 0.0));
		i++;
	}



	//while (i < w*h) { map.push_back(glzAtlasQuad(w, h, i, glzOrigin::BOTTOM_LEFT));	i++; }
}