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

#pragma once

#include "type.h"
using namespace std;

enum class glzLinkStates { VAR, LINK, BOTH };
enum class glzCurvetype {CONSTANT,LINEAR,BEIZER,CUBIC};

template<typename T>
class Link{

private:
	T value;
	Link *linkptr;
	

public:

	glzLinkStates state;

	Link()
	{
		state= glzLinkStates::VAR;
		linkptr = nullptr;			
	}

	Link(T v)
	{
		state = glzLinkStates::VAR;
		linkptr = nullptr;
		value = v;
	}

	Link(Link *lp) 
	{
		if (lp == this) return;
		//value = 0.0;		
		state = glzLinkStates::LINK;
		linkptr = lp;
	}

	Link(T v, Link *lp)
	{
		if (lp == this) return;
		value =v;		
		state = glzLinkStates::BOTH;
		linkptr = lp;
	}

	Link(Link *lp, glzLinkStates st)
	{
		if (lp == this) return;
	//	value = 0.0;
		state = st;
		linkptr = lp;
	}

	T get()
	{
		T lnkval;
		if (state == glzLinkStates::VAR) lnkval = value;
		if (state == glzLinkStates::LINK) lnkval = linkptr->get();
		if (state == glzLinkStates::BOTH) lnkval = value + linkptr->get();
		return lnkval;
	}
	
	void set(T v) { var = v; }


	T *operator->() { return value; }


	operator T() { return this->get(); }
		
	template<typename T2>
	Link<T> operator = (T2 v) { value = v; return *this; }
	template<typename T2>
	Link<T> operator += (T2 v) { value += v; return *this; }
	template<typename T, typename T2>
	Link<T> operator -= (T2 v) { value -= v; return *this; }
	template<typename T, typename T2>
	Link<T> operator *= (T2 v) { value *= v; return *this; }
	template<typename T, typename T2>
	Link<T> operator /= (T2 v) { value /= v; return *this; }
	template<typename T, typename T2>
	Link<T> operator + (T2 v) { value += v; return *this; }

};

template<typename T1,typename T2>
inline Link<T1> operator+(Link<T1> lhs, T2 rhs) { lhs.value += rhs;	return lhs; }

template<typename T1, typename T2>
inline Link<T1> operator-(Link<T1> lhs, T2 rhs) { lhs.value -= rhs;	return lhs; }

template<typename T1, typename T2>
inline Link<T1> operator*(Link<T1> lhs, T2 rhs) { lhs.value *= rhs;	return lhs; }

template<typename T1, typename T2>
inline Link<T1> operator/(Link<T1> lhs, T2 rhs) { lhs.value /= rhs;	return lhs; }









// todo, change all instances to return *this instead

class glzMatrix;

class vec2{ //vector2 class


public:
	float x, y;
	vec2() : x(0.0), y(0.0) {}
	vec2(float xin, float yin) : x{ xin }, y{ yin }{}
	vec2 vec2::operator+ (vec2 b) { return vec2(x + b.x, y + b.y); }
	vec2 vec2::operator+ (float b) { return vec2(x + b, y + b); }
	vec2 vec2::operator+= (vec2 b) { x += b.x; y += b.y; return *this; }
	vec2 vec2::operator+= (float b) { x += b; y += b; return *this; }

	vec2 vec2::operator- (vec2 b) { return vec2(x - b.x, y - b.y); }
	vec2 vec2::operator- (float b) { return vec2(x - b, y - b); }
	vec2 vec2::operator-= (vec2 b) { x -= b.x; y -= b.y; return *this; }
	vec2 vec2::operator-= (float b) { x -= b; y -= b; return *this; }

	vec2 vec2::operator* (vec2 b) { return vec2(x * b.x, y * b.y); }
	vec2 vec2::operator* (float b) { return vec2(x * b, y * b); }
	vec2 vec2::operator*= (vec2 b) { x *= b.x; y *= b.y; return *this; }
	vec2 vec2::operator*= (float b) { x *= b; y *= b; return *this; }

	vec2 vec2::operator/ (vec2 b) { return vec2(x / b.x, y / b.y); }
	vec2 vec2::operator/ (float b) { return vec2(x / b, y / b); }
	vec2 vec2::operator/= (vec2 b) { x /= b.x; y /= b.y; return *this; }
	vec2 vec2::operator/= (float b) { x /= b; y /= b; return *this; }
	

	float magnitude(void) { return sqrt((x * x) + (y * y)); }
	float dot(vec2 a) { return x*a.x + y*a.y; }
	void normalize(float l) { if (!this->magnitude()) return; float m = l / this->magnitude(); x *= m; y *= m; }



};

inline vec2 operator+ (float b, vec2 a) { return vec2(a.x + b, a.y + b); }
inline vec2 operator- (float b, vec2 a) { return vec2(a.x - b, a.y - b); }
inline vec2 operator* (float b, vec2 a) { return vec2(a.x * b, a.y * b); }
inline vec2 operator/ (float b, vec2 a) { return vec2(b / a.x, b / a.y); }

/*
typedef struct vertex2 {
double u, v;
} vec2;*/


class vec3{ //vector3 class


public:
	float x, y, z;
	vec3() : x(0.0f), y(0.0f), z(0.0f) {}
//	vec3(float xin, float yin, float zin) : x{ xin }, y{ yin }, z{ zin } {}
	vec3(double xin, double yin, double zin)
	{
		x = (float)xin;
		y = (float)yin;
		z = (float)zin;
	}

	vec3 vec3::operator+ (vec3 b) { return vec3(x + b.x, y + b.y, z + b.z); }
	vec3 vec3::operator+ (float b) { return vec3(x + b, y + b, z + b); }
	vec3 vec3::operator+= (vec3 b) { x += b.x; y += b.y; z += b.z; return *this; }
	vec3 vec3::operator+= (float b) { x += b; y += b; z += b; return *this; }

	vec3 vec3::operator- (vec3 b) { return vec3(x - b.x, y - b.y, z - b.z); }
	vec3 vec3::operator- (float b) { return vec3(x - b, y - b, z - b); }
	vec3 vec3::operator-= (vec3 b) { x -= b.x; y -= b.y; z -= b.z; return *this; }
	vec3 vec3::operator-= (float b) { x -= b; y -= b; z -= b; return *this; }

	//vec3 vec3::operator* (vert3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vec3 vec3::operator* (vec3 b) { return vec3(x * b.x, y * b.y, z * b.z); }
	vec3 vec3::operator* (float b) { return vec3(x * b, y * b, z * b); }
	//vec3 vec3::operator*= (vert3 b) { a.x *= b.x; a.y *= b.y; a.z *= b.z; return *this; }
	vec3 vec3::operator*= (vec3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vec3 vec3::operator*= (float b) { x *= b; y *= b; z *= b; return *this; }

	vec3 vec3::operator/ (vec3 b) { return vec3(x / b.x, y / b.y, z / b.z); }
	vec3 vec3::operator/ (float b) { return vec3(x / b, y / b, z / b); }
	vec3 vec3::operator/= (vec3 b) { x /= b.x; y /= b.y; z /= b.z; return *this; }
	vec3 vec3::operator/= (float b) { x /= b; y /= b; z /= b; return *this; }

	float magnitude(void) { return sqrt((abs(x) * abs(x)) + (abs(y) * abs(y)) + (abs(z)* abs(z))); }
	float dot(vec3 a) { return x*a.x + y*a.y + z*a.z; }
	vec3 inverse() { return vec3(-1 * x, -1 * y, -1 * z); }
	void reflect(vec3 b);
	void absolute(void) { x = abs(x); y = abs(y); z = abs(z); }
	void normalize(float l) { if (!this->magnitude()) return; float m = l / this->magnitude(); x *= m; y *= m; z *= m; }
	void crossproduct(vec3 a, vec3 b) { x = b.y * a.z - a.y * b.z; y = b.z * a.x - a.z * b.x; z = b.x * a.y - a.x * b.y; }
	void project(glzMatrix m);


};

inline vec3 operator+ (float b, vec3 a) { return vec3(a.x + b, a.y + b, a.z + b); }
inline vec3 operator- (float b, vec3 a) { return vec3(a.x - b, a.y - b, a.z - b); }
inline vec3 operator* (float b, vec3 a) { return vec3(a.x * b, a.y * b, a.z * b); }
inline vec3 operator/ (float b, vec3 a) { return vec3(b / a.x, b / a.y, b / a.z); }





class vert3{ //vertex3 class

private:
	float magnitude(void) { return sqrt((x * x) + (y * y) + (z * z)); }

public:
	float x, y, z;
	vert3() : x(0.0f), y(0.0f), z(0.0f) {}
	//vert3(float xin, float yin, float zin) : x{ xin }, y{ yin }, z{ zin } {}
	vert3(double xin, double yin, double zin=0.0)
	{
		x = (float)xin;
		y = (float)yin;
		z = (float)zin;
	}

	vert3 vert3::operator+ (vec3 b) { return vert3(x + b.x, y + b.y, z + b.z); }
	vert3 vert3::operator+= (vec3 b) { x += b.x; y += b.y; z += b.z; return *this; }
	vert3 vert3::operator- (vec3 b) { return vert3(x-b.x,y-b.y,z-b.z); }
	vert3 vert3::operator-= (vec3 b) { x -= b.x; y -= b.y; z -= b.z; return *this; }

	vert3 vert3::operator+ (vert3 b) { return vert3(x + b.x, y + b.y, z + b.z); }
	vert3 vert3::operator+= (vert3 b) { x += b.x; y += b.y; z += b.z; return *this; }
	vert3 vert3::operator- (vert3 b) { return vert3(x - b.x, y - b.y, z - b.z); }
	vert3 vert3::operator-= (vert3 b) { x -= b.x; y -= b.y; z -= b.z; return *this; }


	vert3 vert3::operator* (vert3 b) { return vert3(x * b.x, y * b.y, z * b.z); }
	vert3 vert3::operator* (vec3 b) { return vert3(x * b.x, y * b.y, z * b.z); }
	vert3 vert3::operator* (float b) { return vert3(x * b, y * b, z * b); }
	vert3 vert3::operator*= (vert3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vert3 vert3::operator*= (vec3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vert3 vert3::operator*= (float b) { x *= b; y *= b; z *= b; return *this; }

	vert3 vert3::operator/ (vert3 b) { return vert3(x / b.x, y / b.y, z / b.z); }
	vert3 vert3::operator/ (vec3 b) { return vert3(x / b.x, y / b.y, z / b.z); }
	vert3 vert3::operator/ (float b) { return vert3(x / b, y / b, z / b); }
	vert3 vert3::operator/= (vert3 b) { x /= b.x; y /= b.y; z /= b.z; return *this; }
	vert3 vert3::operator/= (vec3 b) { x /= b.x; y /= b.y; z /= b.z; return *this; }
	vert3 vert3::operator/= (float b) { x /= b; y /= b; z /= b; return *this; }


	float distance(vert3 a);
	void normalizeOrigin(float l) { if (!this->magnitude()) return; float m = l / this->magnitude(); x *= m; y *= m; z *= m; }
	vec3 vectorTo(vert3 b);
	vec3 vectorPointsTo(vert3 b);	
	void project(glzMatrix m);

};



class tex2{ //texture coordinate class

public:
	float u, v;
	tex2() : u(0.0f), v(0.0f) {}
	tex2(double uin, double vin) : u{ (float)uin }, v{ (float)vin }{}

	
	tex2 tex2::operator+ (tex2 b) { return tex2(u + b.u, v + b.v); }
	tex2 tex2::operator+ (float b) { return tex2(u + b, v + b); }
	tex2 tex2::operator+= (tex2 b) { u += b.u; v += b.v; return *this; }
	tex2 tex2::operator+= (float b) { u += b; v += b; return *this; }

	tex2 tex2::operator- (tex2 b) { return tex2(u - b.u, v - b.v); }
	tex2 tex2::operator- (float b) { return tex2(u - b, v - b); }
	tex2 tex2::operator-= (tex2 b) { u -= b.u; v -= b.v; return *this; }
	tex2 tex2::operator-= (float b) { u -= b; v -= b; return *this; }

	tex2 tex2::operator* (tex2 b) { return tex2(u * b.u, v * b.v); }
	tex2 tex2::operator* (float b) { return tex2(u * b, v * b); }
	tex2 tex2::operator*= (tex2 b) { u *= b.u; v *= b.v; return *this; }
	tex2 tex2::operator*= (float b) { u *= b; v *= b; return *this; }

	tex2 tex2::operator/ (tex2 b) { return tex2(u / b.u, v / b.v); }
	tex2 tex2::operator/ (float b) { return tex2(u / b, v / b); }
	tex2 tex2::operator/= (tex2 b) { u /= b.u; v /= b.v; return *this; }
	tex2 tex2::operator/= (float b) { u /= b; v /= b; return *this; }


	float magnitude(void) { return sqrt((abs(u*u)) + (abs(v*v))); }
	float distance(tex2 a) { return sqrt((abs(u - a.u) * abs(u - a.u)) + (abs(v - a.v) * abs(v - a.v))); }
	void normalize(float l) { if (!this->magnitude()) return; float m = l / this->magnitude(); u *= m; v *= m; }
};


class glzMatrix{ //matrix class

private:
	void multThis(glzMatrix b) { *this *= b; }

public:
	double m[16];
	//double inverted_m[16]; 
	//todo:: for each operation generate the oposite inverted matrix and keep it at hand if needed, and it will be needed if i don't do this

	glzMatrix() { this->LoadIdentity(); }

	glzMatrix(double b[16]) { int v[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }; for (auto i : v) m[i] = b[i]; }
	void LoadIdentity(void);
	void LoadIdentityzero(void);
	void transferMatrix(float *b); // possibly a temporary method untill i can figure out something better
	//operators
	glzMatrix glzMatrix::operator*= (glzMatrix b);
	glzMatrix glzMatrix::operator* (glzMatrix b) { *this *= b; }

	//glzMatrix glzMatrix::operator*= (glzMatrix c, glzQuaternion b);
	// movements
	void translate(float x, float y, float z);// {m[12] += x; m[13] += y;  m[14] += z; }
	void translate(vec2 p) { this->translate(p.x, p.y, 0.0f); }
	void translate(vec3 p) { this->translate(p.x, p.y, p.z); }
	void translate(vert3 p) { this->translate(p.x, p.y, p.z); }

	void scale(float x, float y, float z);
	void scale(vec3 s) { this->scale(s.x, s.y, s.z); }
	void rotate(float a, float x, float y, float z);
	void loadQuanternion(float q[4]);
	void loadQuanternion(glzQuaternion b);
	// views
	void perspective(float fov, float aspect, float zNear, float zFar);
	void ortho(float left, float right, float bottom, float top, float Znear, float Zfar);
	void ortho2D(float left, float right, float bottom, float top);
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
	float w, x, y, z;
	glzQuaternion() : w(1.0), x(0.0), y(0.0), z(0.0) {}
	glzQuaternion(float win, float xin, float yin, float zin) : w{ win }, x{ xin }, y{ yin }, z{ zin } {}

	glzQuaternion glzQuaternion::operator * (float a)	{ x *= a; y *= a; z *= a; this->normalize(); return *this; }
	glzQuaternion glzQuaternion::operator *= (float a)	{ x *= a; y *= a; z *= a; this->normalize(); return *this; }

	glzQuaternion glzQuaternion::operator * (glzQuaternion a)	{ this->multQuaternion(a); return *this; }
	glzQuaternion glzQuaternion::operator *= (glzQuaternion a)	{ this->multQuaternion(a); return *this; }

	void identity(void) { w = 1.0; x=0.0; y=0.0; z=0.0; }
	void normalize(void) { float n = sqrt(w * w + x * x + y * y + z * z);	w /= n;	x /= n;	y /= n;	z /= n; }
	void rotate(float a, float x, float y, float z);
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


/*

class glzCurvenode
{
public:

	float time;
	float position;
	glzCurvetype type;
	vec2 intangent;
	vec2 outtangent;


	glzCurvenode() : time(0.0f), float(0.0f), type(glzCurvetype::CONSTANT), intangent(vec2()), outtangent(vec2()) {}
//	glzCurvenode(float timein, float possitionin, glzCurvetype ctin) : time{ timein }, position{ possitionin }, type{ ctin } {}
//	glzCurvenode(float timein, float possitionin, glzCurvetype ctin, vec2 intin, vec2 outtin) : time{ timein }, position{ possitionin }, type{ ctin } intangent{ intin } outtangent{ outtin } {}

};


class glzCurve{

private:
	float t;
	float v;
	float pretime;
	float posttime;
	float predata;
	float postdata;

	void ordernodes(void)
	{
		sort(curvenodes.begin(), curvenodes.end(), [](glzCurvenode a, glzCurvenode b) {	return a.time > b.time;	});

		pretime = curvenodes.at(0).time;
		posttime = pretime;

		predata = curvenodes.at(0).position;
		postdata = predata;

		// get the highest and lowest keyframe times
		for (auto a : curvenodes)
		{
			if (a.time < pretime) { pretime = a.time; predata = a.position; }
			if (a.time > posttime) { posttime = a.time; postdata = a.position; }
		}

	}

	void resolve()
	{	
		glzCurvenode a_n, b_n;
		if (t < pretime) { v = predata; return; }
		if (t > posttime) { v = postdata; return; }

		int i=0;
		for (auto a : curvenodes)
		{
			if (a.time < t) { i++; }
		}

		a_n = curvenodes.at(i-1);
		b_n = curvenodes.at(i);

		switch (a_n.type)
		{
			case glzCurvetype::CONSTANT:
				v = a_n.position;
				break;

			case glzCurvetype::LINEAR:
			//	v = glzLerpRange(a_n.time, t, ba_n.time, a_n.position, b_n.position)
				break;
		}




	}

public:
	glzCurvetype preinf;
	glzCurvetype postinf;
	vector<glzCurvenode> curvenodes;

	glzCurve() : t(0.0f), pretime(0.0f), posttime(0.0f), preinf(glzCurvetype::CONSTANT), postinf(glzCurvetype::CONSTANT) {}

	glzCurve(glzCurvenode n) 
	{ 
		t = 0.0f; 
		pretime=0.0f; 
		posttime = 0.0f; 
		curvenodes.push_back(n); 
		preinf = glzCurvetype::CONSTANT; 
		postinf = glzCurvetype::CONSTANT; 

		pretime = curvenodes.at(0).time;
		posttime = pretime;

		predata = curvenodes.at(0).position;
		postdata = predata;

		v = curvenodes.at(0).position;

	}

	void addNode(glzCurvenode n) 
	{
		curvenodes.push_back(n); 
		ordernodes();
		resolve();
	}


	void settime(float time) { t = time; return };
};



*/



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
	void generateTexture(float scale);
	

	void tempAddNormalToVertex();  // only for testing that normals work

	

};

class line3{ //a mathematical line

private:

public:
		
	vec3 n;
	vert3 p;

	line3() : n(vec3()), p(vert3()) {}
	line3(vec3 nin, vert3 pin) : n{ nin }, p{ pin }{}
	
	line3(vert3 a, vert3 b) { p = a; n = a.vectorTo(b); }
};



class plane3{ //a mathematical plane

private:


public:
	vec3 n;
	float d;

	plane3() : n(vec3()), d(0.0) {}
	plane3(vec3 nin, float din) : n{ nin }, d{ din }{}
	plane3(poly3 a);
	
	bool is_infront(vert3 a);
	bool does_Straddle(vert3 a, vert3 b) { if (this->is_infront(a) != this->is_infront(b)) return true; else return false; }

	bool can_intersect(vert3 a, vec3 b);

	vert3 intesect_line(line3 a);

};





class node3{ //3-Dimetional possition class

private:

	

public:
	vert3 pos;
	vec3 dir;
	glzQuaternion r;
	glzQuaternion rs;
	vec3 scale;
	glzMatrix m;


	node3() : pos(vert3()), dir(vec3()), r(glzQuaternion()), rs(glzQuaternion()), scale(vec3(1.0, 1.0, 1.0)) { update_matrix(); }
	node3(vert3 posin, vec3 dirin, glzQuaternion rin, glzQuaternion rsin) : pos{ posin }, dir{ dirin }, r{ rin }, rs{ rsin }, scale(vec3(1.0, 1.0, 1.0)){update_matrix(); }
	node3(vert3 posin) : pos{ posin }, dir(vec3()), r(glzQuaternion()), rs(glzQuaternion()), scale(vec3(1.0, 1.0, 1.0)) {update_matrix(); }

	node3(vert3 posin, glzQuaternion rin) : pos{ posin }, dir(vec3()), r(rin), rs(glzQuaternion()), scale(vec3(1.0, 1.0, 1.0)) {update_matrix(); }

	node3(vert3 posin, float angle, float scalein)
	{
		pos = posin;
		scale= vec3(scalein, scalein, scalein);
		r.rotate(angle, 0.0, 0.0, 1.0);
		update_matrix();
	}

	void update_matrix()
	{
		m.LoadIdentity();
		m.translate(pos);
		m.scale(scale);
		m.loadQuanternion(r);
	}

	void tick(float seconds)
		{ 
			pos += dir*seconds;

			glzQuaternion rt;
			rt *= rs;
			rt *= seconds;

			r *= rt;
			update_matrix();
	}


	/*
	turn to
	move to
	align to

	*/

};


class glzCamera2D{

private:

	vert3 pos;
	vert3 moveto_pos;

	float zoomto;
	float angle;
	float angleto;
	float move_speed;
	float zoom_speed;
	float angle_speed;



	void resetCamera()
	{
		m.LoadIdentity();
		m.ortho(-width*0.5f, width*0.5f, -height*0.5f, height*0.5f, -100.0f, 100.0f);
		//m.ortho(-0.5, 0.5, -0.5, 0.5, -100, 100);
		m.scale(zoom, zoom, 1);
		m.translate(pos);
		m.rotate(angle, 0.0, 0.0, 1.0);
	}

public:

	float zoom;
	float width, height;
	float aspect;
	glzMatrix m;

	glzCamera2D()
	{
		zoom = 1.0;
		zoomto = 1.0;
		angle = 0.0;
		angleto = 0.0;

		move_speed = 1.0;
		zoom_speed = 5.0;
		angle_speed = 10.0;

		width = 100.0f;
		height = 100.0f;
		resetCamera();
	}

	void setsize(float w, float h)
	{
		width = w;
		height = h;
		aspect = w / h;
		resetCamera();
	}

	void moveTo(vert3 p) { moveto_pos = p; }
	void moveToRel(vert3 p) { moveto_pos += p; }
	void zoomTo(float z) { zoomto = z; }
	void angleTo(float a) { angleto = a; }


	void moveSnap(vert3 p) { pos = p; moveto_pos = p; resetCamera(); }
	void moveSnapRel(vert3 p) { pos = moveto_pos; moveto_pos += p; resetCamera(); }
	void ZoomSnap(float z) { zoom = z; zoomto = z; resetCamera(); }
	void angleSnap(float a) { angle = a; angleto = a; resetCamera(); }

	void moveSpeed(float m) { move_speed = m; }
	void zoomSpeed(float z) { zoom_speed = z; }
	void angleSpeed(float a) { angle_speed = a; }

	void update(float seconds)
	{


		vec3 v1 = pos.vectorTo(moveto_pos);
		v1.normalize(move_speed*seconds);

		if (pos.distance(moveto_pos) < move_speed*seconds) pos = moveto_pos;
		else pos += v1;


		if (zoom < zoomto)
		{
			if ((zoomto - zoom) < (zoom_speed*(seconds*2.0)))
				zoom = zoomto;
			else zoom += zoom_speed*seconds*zoom;
		}
		else
		{
			if ((zoom - zoomto) < (zoom_speed*(seconds*2.0)))
				zoom = zoomto;
			else zoom -= zoom_speed*seconds*zoom;
		}

		bool anglecentered = false;

		if (abs(angle - angleto) < angle_speed*seconds) { angle = angleto; anglecentered = true; }
		if (abs((angle + 360) - angleto) < angle_speed*seconds) { angle = angleto; anglecentered = true; }

		if (!anglecentered)
		{
			if (angle < angleto) {
				if (abs(angle - angleto)<180.0)
					angle += angle_speed*seconds;
				else angle -= angle_speed*seconds;
			}
			else
			{
				if (abs(angle - angleto)<180.0)
					angle -= angle_speed*seconds;
				else angle += angle_speed*seconds;
			}
		}

		while (angle < 0.0) angle += 360.0;
		while (angle >= 360.0) angle -= 360.0;


		resetCamera();

	}






};




class glzRidgidSimple{ //basic physics class

private:


public:
	node3 p;
	float weight;


	glzRidgidSimple() : p(node3()), weight(1.0) {}
	glzRidgidSimple(node3 pin, float weightin) : p{ pin }, weight{ weightin } {}
/*	pos3(vert3 posin) : pos{ posin }, dir(vec3()), r(glzQuaternion()), rs(glzQuaternion()), scale(vec3(1.0, 1.0, 1.0)) {}

	void tick(double seconds) { pos += dir*seconds;	r *= rs*seconds; m.LoadIdentity();	m.translate(pos); m.scale(scale); m.loadQuanternion(r); }


	
	turn to
	move to
	align to

	*/

};




