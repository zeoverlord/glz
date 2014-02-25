// Copyright 2014 Peter Wallstr�m
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

#include "ztool-type.h"

using namespace std;


#ifndef __glzvectormath__
#define __glzvectormath__


// todo, change all instances to return *this instead

class vec2{ //vector2 class


public:
	double x, y;
	vec2() : x(0.0), y(0.0) {}
	vec2(double xin, double yin) : x{ xin }, y{ yin }{}
	vec2 vec2::operator+ (vec2 b) { return vec2(x + b.x, y + b.y); }
	vec2 vec2::operator+ (double b) { return vec2(x + b, y + b); }
	vec2 vec2::operator+= (vec2 b) { return vec2(b.x + x, b.y + y); }
	vec2 vec2::operator+= (double b) { return vec2(x + b, y + b); }
	vec2 vec2::operator- (vec2 b) { return vec2(x - b.x, y - b.y); }
	vec2 vec2::operator- (double b) { return vec2(x - b, y - b); }
	vec2 vec2::operator-= (vec2 b) { return vec2(b.x - x, b.y - y); }
	vec2 vec2::operator-= (double b) { return vec2(x - b, y - b); }
	vec2 vec2::operator* (vec2 b) { return vec2(b.x * x, b.y * y); }
	vec2 vec2::operator* (double b) { return vec2(b * x, b * y); }
	vec2 vec2::operator*= (vec2 b) { return vec2(b.x * x, b.y * y); }
	vec2 vec2::operator*= (double b) { return vec2(b * x, b * y); }
	vec2 vec2::operator/ (vec2 b) { return vec2(x / b.x, y / b.y); }
	vec2 vec2::operator/ (double b) { return vec2(x / b, y / b); }
	vec2 vec2::operator/= (vec2 b) { return vec2(x / b.x, y / b.y); }
	vec2 vec2::operator/= (double b) { return vec2(x / b, y / b); }

	double magnitude(void) { return sqrt((x * x) + (y * y)); }
	double dot(vec2 a) { return x*a.x + y*a.y; }
	void normalize(double l) { if (!this->magnitude()) return; double m = l / this->magnitude(); x *= m; y *= m; }



};

inline vec2 operator+ (double b, vec2 a) { return vec2(a.x + b, a.y + b); }
inline vec2 operator- (double b, vec2 a) { return vec2(a.x - b, a.y - b); }
inline vec2 operator* (double b, vec2 a) { return vec2(a.x * b, a.y * b); }
inline vec2 operator/ (double b, vec2 a) { return vec2(b / a.x, b / a.y); }

/*
typedef struct vertex2 {
double u, v;
} vec2;*/


class vec3{ //vector3 class


public:
	double x, y, z;
	vec3() : x(0.0), y(0.0), z(0.0) {}
	vec3(double xin, double yin, double zin) : x{ xin }, y{ yin }, z{ zin } {}

	vec3 vec3::operator+ (vec3 b) { x += b.x; y += b.y; z += b.z; return *this; }
	vec3 vec3::operator+ (double b) { x += b; y += b; z += b; return *this; }
	vec3 vec3::operator+= (vec3 b) { x += b.x; y += b.y; z += b.z; return *this; }
	vec3 vec3::operator+= (double b) { x += b; y += b; z += b; return *this; }
	vec3 vec3::operator- (vec3 b) { x -= b.x; y -= b.y; z -= b.z; return *this; }
	vec3 vec3::operator- (double b) { x -= b; y -= b; z -= b; return *this; }
	vec3 vec3::operator-= (vec3 b) { x -= b.x; y -= b.y; z -= b.z; return *this; }
	vec3 vec3::operator-= (double b) { x -= b; y -= b; z -= b; return *this; }

	//vec3 vec3::operator* (vert3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vec3 vec3::operator* (vec3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vec3 vec3::operator* (double b) { x *= b; y *= b; z *= b; return *this; }
	//vec3 vec3::operator*= (vert3 b) { a.x *= b.x; a.y *= b.y; a.z *= b.z; return *this; }
	vec3 vec3::operator*= (vec3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vec3 vec3::operator*= (double b) { x *= b; y *= b; z *= b; return *this; }

	vec3 vec3::operator/ (vec3 b) { x /= b.x; y /= b.y; z /= b.z; return *this; }
	vec3 vec3::operator/ (double b) { x /= b; y /= b; z /= b; return *this; }
	vec3 vec3::operator/= (vec3 b) { x /= b.x; y /= b.y; z /= b.z; return *this; }
	vec3 vec3::operator/= (double b) { x /= b; y /= b; z /= b; return *this; }

	double magnitude(void) { return sqrt((x * x) + (y * y) + (z * z)); }
	//double distance(vec3 a) { return sqrt((x-a.x * x-a.x) + (y-a.y * y-a.y) + (z-a.z * z-a.z)); }
	double dot(vec3 a) { return x*a.x + y*a.y + z*a.z; }
	void normalize(double l) { if (!this->magnitude()) return; double m = l / this->magnitude(); x *= m; y *= m; z *= m; }
	void crossproduct(vec3 a, vec3 b) { x = b.y * a.z - a.y * b.z; y = b.z * a.x - a.z * b.x; z = b.x * a.y - a.x * b.y; }



};

inline vec3 operator+ (double b, vec3 a) { return vec3(a.x + b, a.y + b, a.z + b); }
inline vec3 operator- (double b, vec3 a) { return vec3(a.x - b, a.y - b, a.z - b); }
inline vec3 operator* (double b, vec3 a) { return vec3(a.x * b, a.y * b, a.z * b); }
inline vec3 operator/ (double b, vec3 a) { return vec3(b / a.x, b / a.y, b / a.z); }





class vert3{ //vertex3 class

private:
	double magnitude(void) { return sqrt((x * x) + (y * y) + (z * z)); }

public:
	double x, y, z;
	vert3() : x(0.0), y(0.0), z(0.0) {}
	vert3(double xin, double yin, double zin) : x{ xin }, y{ yin }, z{ zin } {}
	vert3 vert3::operator+ (vec3 b) { x += b.x; y += b.y; z += b.z; return *this; }
	vert3 vert3::operator+= (vec3 b) { x += b.x; y += b.y; z += b.z; return *this; }
	vert3 vert3::operator- (vec3 b) { x -= b.x; y -= b.y; z -= b.z; return *this; }
	vert3 vert3::operator-= (vec3 b) { x -= b.x; y -= b.y; z -= b.z; return *this; }

	vert3 vert3::operator* (vert3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vert3 vert3::operator* (vec3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vert3 vert3::operator* (double b) { x *= b; y *= b; z *= b; return *this; }
	vert3 vert3::operator*= (vert3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vert3 vert3::operator*= (vec3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vert3 vert3::operator*= (double b) { x *= b; y *= b; z *= b; return *this; }

	vert3 vert3::operator/ (vert3 b) { x /= b.x; y /= b.y; z /= b.z; return *this; }
	vert3 vert3::operator/ (vec3 b) { x /= b.x; y /= b.y; z /= b.z; return *this; }
	vert3 vert3::operator/ (double b) { x /= b; y /= b; z /= b; return *this; }
	vert3 vert3::operator/= (vert3 b) { x /= b.x; y /= b.y; z /= b.z; return *this; }
	vert3 vert3::operator/= (vec3 b) { x /= b.x; y /= b.y; z /= b.z; return *this; }
	vert3 vert3::operator/= (double b) { x /= b; y/= b; z /= b; return *this; }


	double distance(vert3 a) { return sqrt((x - a.x * x - a.x) + (y - a.y * y - a.y) + (z - a.z * z - a.z)); }
	void normalizeOrigin(double l) { if (!this->magnitude()) return; double m = l / this->magnitude(); x *= m; y *= m; z *= m; }
	vec3 vectorTo(vert3 b);

};




class tex2{ //texture coordinate class

public:
	double u, v;
	tex2() : u(0.0), v(0.0) {}
	tex2(double uin, double vin) : u{ uin }, v{ vin }{}
	tex2 tex2::operator+ (tex2 b) { return tex2(u + b.u, v + b.v); }
	tex2 tex2::operator+ (double b) { return tex2(u + b, v + b); }
	tex2 tex2::operator+= (tex2 b) { return tex2(b.u + u, b.v + v); }
	tex2 tex2::operator+= (double b) { return tex2(u + b, v + b); }
	tex2 tex2::operator- (tex2 b) { return tex2(u - b.u, v - b.v); }
	tex2 tex2::operator- (double b) { return tex2(u - b, v - b); }
	tex2 tex2::operator-= (tex2 b) { return tex2(b.u - u, b.v - v); }
	tex2 tex2::operator-= (double b) { return tex2(u - b, v - b); }
	tex2 tex2::operator* (tex2 b) { return tex2(b.u * u, b.v * v); }
	tex2 tex2::operator* (double b) { return tex2(b * u, b * v); }
	tex2 tex2::operator*= (tex2 b) { return tex2(b.u * u, b.v * v); }
	tex2 tex2::operator*= (double b) { return tex2(b * u, b * v); }
	tex2 tex2::operator/ (tex2 b) { return tex2(u / b.u, v / b.v); }
	tex2 tex2::operator/ (double b) { return tex2(u / b, v / b); }
	tex2 tex2::operator/= (tex2 b) { return tex2(u / b.u, v / b.v); }
	tex2 tex2::operator/= (double b) { return tex2(u / b, v / b); }

	double magnitude(void) { return sqrt((u * u) + (v * v)); }
	double distance(tex2 a) { return sqrt((u - a.u * u - a.u) + (v - a.v * v - a.v)); }
	void normalize(double l) { if (!this->magnitude()) return; double m = l / this->magnitude(); u *= m; v *= m; }
};


class glzMatrix{ //matrix class

private:
	void multThis(glzMatrix b) { *this *= b; }

	//glzMatrix() : glzMatrix((double[16]){ 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 }) {}
public:
	double m[16];
	glzMatrix() { this->LoadIdentity(); }

	glzMatrix(double b[16]) { int v[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }; for (auto i : v) m[i] = b[i]; }
	void LoadIdentity(void);
	void LoadIdentityzero(void);
	void transferMatrix(float *b); // possibly a temporary method untill i can figure out something better
	//operators
	glzMatrix glzMatrix::operator*= (glzMatrix b);
	//glzMatrix glzMatrix::operator*= (glzMatrix c, glzQuaternion b);
	// movements
	void translate(double x, double y, double z);// {m[12] += x; m[13] += y;  m[14] += z; }
	void scale(double x, double y, double z) { m[0] *= x; m[5] *= y;  m[10] *= z; }
	void rotate(double a, double x, double y, double z);
	void loadQuanternion(float q[4]);
	void loadQuanternion(glzQuaternion b);
	// views
	void perspective(double fov, double aspect, double zNear, double zFar);
	void ortho(double left, double right, double bottom, double top, double Znear, double Zfar);
	void ortho2D(double left, double right, double bottom, double top);
	void ortho2DPixelspace(int x, int y, glzOrigin origin);

};

inline glzMatrix operator* (glzMatrix lhs, glzMatrix rhs) { lhs *= rhs;	return lhs; }



// vertex projection
inline vert3 operator* (glzMatrix a, vert3 b)
{
	return vert3((b.x * a.m[0]) + (b.y * a.m[4]) + (b.z * a.m[8]) + a.m[12],
		(b.x * a.m[1]) + (b.y * a.m[5]) + (b.z * a.m[9]) + a.m[13],
		(b.x * a.m[2]) + (b.y * a.m[6]) + (b.z * a.m[10]) + a.m[14]);
}


// vector projection
inline vec3 operator* (glzMatrix a, vec3 b)
{
	return vec3((b.x * a.m[0]) + (b.y * a.m[4]) + (b.z * a.m[8]) + a.m[12],
		(b.x * a.m[1]) + (b.y * a.m[5]) + (b.z * a.m[9]) + a.m[13],
		(b.x * a.m[2]) + (b.y * a.m[6]) + (b.z * a.m[10]) + a.m[14]);
}




class glzQuaternion{ //Quaternion class

private:

	void multQuaternion(glzQuaternion b);

public:
	double w, x, y, z;
	glzQuaternion() : w(1.0), x(0.0), y(0.0), z(0.0) {}
	glzQuaternion(double win, double xin, double yin, double zin) : w{ win }, x{ xin }, y{ yin }, z{ zin } {}

	void identity(void) { glzQuaternion(1.0, 0.0, 0.0, 0.0); }
	void normalize(void) { double n = sqrt(w * w + x * x + y * y + z * z);	w /= n;	x /= n;	y /= n;	z /= n; }
	void rotate(double a, double x, double y, double z);
};

//inline glzMatrix operator* (glzMatrix lhs, glzQuaternion rhs) { lhs *= rhs;	return lhs; }

/*
typedef struct point_3 {
	vert3 v;
	tex2 t;
	vec3 n;
} point3;


typedef struct polygon3 {
	point3 a, b, c;
	int group;
	int atlas;
} poly3;*/


class point3{ //polygon3 class

private:


public:
	vert3 v;
	tex2 t;
	vec3 n;

	point3() : v(vert3()), t(tex2()), n(vec3()) {}
	point3(vert3 vin, tex2 tin, vec3 nin) : v{ vin }, t{ tin }, n{ nin }{}


};


class poly3{ //polygon3 class

private:


public:
	point3 a, b, c;
	int group;
	int atlas;
	
	poly3() : a(point3()), b(point3()), c(point3()), group(0), atlas(0) {}
	poly3(point3 ain, point3 bin, point3 cin, int groupin, int atlasin) : a{ ain }, b{ bin }, c{ cin }, group{ groupin }, atlas{ atlasin }{}
	vec3 getFaceNormal();
	void generateNormal();
	void generateTexture(double scale);
	

	void tempAddNormalToVertex();  // only for testing that normals work

	

};



#endif /* __glzvectormath__ */
