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

#include "ztool-type.h"

using namespace std;


#ifndef __glzvectormath__
#define __glzvectormath__


enum class glzLinkStates { VAR, LINK, BOTH };


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
	double x, y;
	vec2() : x(0.0), y(0.0) {}
	vec2(double xin, double yin) : x{ xin }, y{ yin }{}
	vec2 vec2::operator+ (vec2 b) { return vec2(x + b.x, y + b.y); }
	vec2 vec2::operator+ (double b) { return vec2(x + b, y + b); }
	vec2 vec2::operator+= (vec2 b) { x += b.x; y += b.y; return *this; }
	vec2 vec2::operator+= (double b) { x += b; y += b; return *this; }

	vec2 vec2::operator- (vec2 b) { return vec2(x - b.x, y - b.y); }
	vec2 vec2::operator- (double b) { return vec2(x - b, y - b); }
	vec2 vec2::operator-= (vec2 b) { x -= b.x; y -= b.y; return *this; }
	vec2 vec2::operator-= (double b) { x -= b; y -= b; return *this; }

	vec2 vec2::operator* (vec2 b) { return vec2(x * b.x, y * b.y); }
	vec2 vec2::operator* (double b) { return vec2(x * b, y * b); }
	vec2 vec2::operator*= (vec2 b) { x *= b.x; y *= b.y; return *this; }
	vec2 vec2::operator*= (double b) { x *= b; y *= b; return *this; }

	vec2 vec2::operator/ (vec2 b) { return vec2(x / b.x, y / b.y); }
	vec2 vec2::operator/ (double b) { return vec2(x / b, y / b); }
	vec2 vec2::operator/= (vec2 b) { x /= b.x; y /= b.y; return *this; }
	vec2 vec2::operator/= (double b) { x /= b; y /= b; return *this; }
	

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

	vec3 vec3::operator+ (vec3 b) { return vec3(x + b.x, y + b.y, z + b.z); }
	vec3 vec3::operator+ (double b) { return vec3(x + b, y + b, z + b); }
	vec3 vec3::operator+= (vec3 b) { x += b.x; y += b.y; z += b.z; return *this; }
	vec3 vec3::operator+= (double b) { x += b; y += b; z += b; return *this; }

	vec3 vec3::operator- (vec3 b) { return vec3(x - b.x, y - b.y, z - b.z); }
	vec3 vec3::operator- (double b) { return vec3(x - b, y - b, z - b); }
	vec3 vec3::operator-= (vec3 b) { x -= b.x; y -= b.y; z -= b.z; return *this; }
	vec3 vec3::operator-= (double b) { x -= b; y -= b; z -= b; return *this; }

	//vec3 vec3::operator* (vert3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vec3 vec3::operator* (vec3 b) { return vec3(x * b.x, y * b.y, z * b.z); }
	vec3 vec3::operator* (double b) { return vec3(x * b, y * b, z * b); }
	//vec3 vec3::operator*= (vert3 b) { a.x *= b.x; a.y *= b.y; a.z *= b.z; return *this; }
	vec3 vec3::operator*= (vec3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vec3 vec3::operator*= (double b) { x *= b; y *= b; z *= b; return *this; }

	vec3 vec3::operator/ (vec3 b) { return vec3(x / b.x, y / b.y, z / b.z); }
	vec3 vec3::operator/ (double b) { return vec3(x / b, y / b, z / b); }
	vec3 vec3::operator/= (vec3 b) { x /= b.x; y /= b.y; z /= b.z; return *this; }
	vec3 vec3::operator/= (double b) { x /= b; y /= b; z /= b; return *this; }

	double magnitude(void) { return sqrt((abs(x) * abs(x)) + (abs(y) * abs(y)) + (abs(z)* abs(z))); }
	double dot(vec3 a) { return x*a.x + y*a.y + z*a.z; }
	vec3 inverse() { return vec3(-1 * x, -1 * y, -1 * z); }
	void reflect(vec3 b);
	void absolute(void) { x = abs(x); y = abs(y); z = abs(z); }
	void normalize(double l) { if (!this->magnitude()) return; double m = l / this->magnitude(); x *= m; y *= m; z *= m; }
	void crossproduct(vec3 a, vec3 b) { x = b.y * a.z - a.y * b.z; y = b.z * a.x - a.z * b.x; z = b.x * a.y - a.x * b.y; }
	void project(glzMatrix m);


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
	vert3 vert3::operator* (double b) { return vert3(x * b, y * b, z * b); }
	vert3 vert3::operator*= (vert3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vert3 vert3::operator*= (vec3 b) { x *= b.x; y *= b.y; z *= b.z; return *this; }
	vert3 vert3::operator*= (double b) { x *= b; y *= b; z *= b; return *this; }

	vert3 vert3::operator/ (vert3 b) { return vert3(x / b.x, y / b.y, z / b.z); }
	vert3 vert3::operator/ (vec3 b) { return vert3(x / b.x, y / b.y, z / b.z); }
	vert3 vert3::operator/ (double b) { return vert3(x / b, y / b, z / b); }
	vert3 vert3::operator/= (vert3 b) { x /= b.x; y /= b.y; z /= b.z; return *this; }
	vert3 vert3::operator/= (vec3 b) { x /= b.x; y /= b.y; z /= b.z; return *this; }
	vert3 vert3::operator/= (double b) { x /= b; y/= b; z /= b; return *this; }


	double distance(vert3 a);
	void normalizeOrigin(double l) { if (!this->magnitude()) return; double m = l / this->magnitude(); x *= m; y *= m; z *= m; }
	vec3 vectorTo(vert3 b);
	vec3 vectorPointsTo(vert3 b);	
	void project(glzMatrix m);

};




class tex2{ //texture coordinate class

public:
	double u, v;
	tex2() : u(0.0), v(0.0) {}
	tex2(double uin, double vin) : u{ uin }, v{ vin }{}
	tex2 tex2::operator+ (tex2 b) { return tex2(u + b.u, v + b.v); }
	tex2 tex2::operator+ (double b) { return tex2(u + b, v + b); }
	tex2 tex2::operator+= (tex2 b) { u += b.u; v += b.v; return *this; }
	tex2 tex2::operator+= (double b) { u += b; v += b; return *this; }

	tex2 tex2::operator- (tex2 b) { return tex2(u - b.u, v - b.v); }
	tex2 tex2::operator- (double b) { return tex2(u - b, v - b); }
	tex2 tex2::operator-= (tex2 b) { u -= b.u; v -= b.v; return *this; }
	tex2 tex2::operator-= (double b) { u -= b; v -= b; return *this; }

	tex2 tex2::operator* (tex2 b) { return tex2(u * b.u, v * b.v); }
	tex2 tex2::operator* (double b) { return tex2(u * b, v * b); }
	tex2 tex2::operator*= (tex2 b) { u *= b.u; v *= b.v; return *this; }
	tex2 tex2::operator*= (double b) { u *= b; v *= b; return *this; }

	tex2 tex2::operator/ (tex2 b) { return tex2(u / b.u, v / b.v); }
	tex2 tex2::operator/ (double b) { return tex2(u / b, v / b); }
	tex2 tex2::operator/= (tex2 b) { u /= b.u; v /= b.v; return *this; }
	tex2 tex2::operator/= (double b) { u /= b; v /= b; return *this; }


	double magnitude(void) { return sqrt((abs(u*u)) + (abs(v*v))); }
	double distance(tex2 a) { return sqrt((abs(u - a.u) * abs(u - a.u)) + (abs(v - a.v) * abs(v - a.v))); }
	void normalize(double l) { if (!this->magnitude()) return; double m = l / this->magnitude(); u *= m; v *= m; }
};


class glzMatrix{ //matrix class

private:
	void multThis(glzMatrix b) { *this *= b; }

	//glzMatrix() : glzMatrix((double[16]){ 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 }) {}
public:
	double m[16];
	//double inverted_m[16]; 
	//todo:: for each operation generate the oposite inverted matrix and heep it at hand if nneded, and it will be nneded if i don't do this

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
	void translate(double x, double y, double z);// {m[12] += x; m[13] += y;  m[14] += z; }
	void translate(vert3 p) { this->translate(p.x, p.y, p.z); }
	void scale(double x, double y, double z);
	void scale(vec3 s) { this->scale(s.x, s.y, s.z); }
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

	glzQuaternion glzQuaternion::operator * (double a)	{ x *= a; y *= a; z *= a; this->normalize(); return *this; }
	glzQuaternion glzQuaternion::operator *= (double a)	{ x *= a; y *= a; z *= a; this->normalize(); return *this; }

	glzQuaternion glzQuaternion::operator * (glzQuaternion a)	{ this->multQuaternion(a); return *this; }
	glzQuaternion glzQuaternion::operator *= (glzQuaternion a)	{ this->multQuaternion(a); return *this; }

	void identity(void) { w = 1.0; x=0.0; y=0.0; z=0.0; }
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
	double d;

	plane3() : n(vec3()), d(0.0) {}
	plane3(vec3 nin, double din) : n{ nin }, d{ din }{}
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


	node3() : pos(vert3()), dir(vec3()), r(glzQuaternion()), rs(glzQuaternion()), scale(vec3(1.0, 1.0, 1.0)) {}
	node3(vert3 posin, vec3 dirin, glzQuaternion rin, glzQuaternion rsin) : pos{ posin }, dir{ dirin }, r{ rin }, rs{ rsin }, scale(vec3(1.0, 1.0, 1.0)){}
	node3(vert3 posin) : pos{ posin }, dir(vec3()), r(glzQuaternion()), rs(glzQuaternion()), scale(vec3(1.0, 1.0, 1.0)) {}

	void tick(double seconds) 	{ pos += dir*seconds;	r *= rs*seconds;	m.LoadIdentity();	m.translate(pos);	m.scale(scale);	m.loadQuanternion(r); }


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
	double zoom;
	double zoomto;
	double angle;
	double angleto;
	double move_speed;
	double zoom_speed;
	double angle_speed;
	int width, height;

	void resetCamera()
	{
		m.LoadIdentity();
		m.ortho(-width*0.5, width*0.5, -height*0.5, height*0.5, -100, 100);
		m.scale(zoom, zoom, 1);
		m.translate(pos);
		m.rotate(angle, 0.0, 0.0, 1.0);
	}

public:
	glzMatrix m;

	glzCamera2D()
	{
		zoom = 1.0;
		zoomto = 1.0;
		angle = 0.0;
		angleto = 0.0;

		move_speed = 1.0;
		zoom_speed = 1.0;
		angle_speed = 1.0;

		width = 100;
		height = 100;	
		resetCamera();
	}

	void setsize(double w, double h)
	{
		width = w;
		height = h;
		resetCamera();
	}

	void moveTo(vert3 p) { moveto_pos = p; }
	void zoomTo(double z) { zoomto = z; }
	void angleTo(double a) { angleto = a; }

	void moveSnap(vert3 p) { pos = p; moveto_pos = p; resetCamera(); }
	void ZoomSnap(double z) { zoom = z; zoomto = z; resetCamera(); }
	void angleSnap(double a) { angle = a; angleto = a; resetCamera(); }

	void moveSpeed(double m) { move_speed = m; }
	void zoomSpeed(double z) { zoom_speed = z; }
	void angleSpeed(double a) { angle_speed = a; }

	void update(float seconds)
	{


		vec3 v1 = pos.vectorTo(moveto_pos);
		v1.normalize(move_speed*seconds);

		if (pos.distance(moveto_pos) < move_speed*seconds) pos = moveto_pos;
		else pos += v1;


		if (zoom < zoomto)
		{
			if ((zoomto - zoom) < (zoom_speed*seconds)) 
				zoom = zoomto;
			else zoom += zoom_speed*seconds;
		}
		else 
		{
			if ((zoom - zoomto ) < (zoom_speed*seconds)) 
				zoom = zoomto;
			else zoom -= zoom_speed*seconds;
		}

		if (abs(angle - angleto) < angle_speed*seconds) angle = angleto;
		if (abs((angle + 360) - angleto) < angle_speed*seconds) angle = angleto;

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

		while (angle < 0.0) angle += 360.0;
		while (angle >= 360.0) angle -= 360.0;			

		resetCamera();

	}
		

		



};




class glzRidgidSimple{ //basic physics class

private:


public:
	node3 p;
	double weight;


	glzRidgidSimple() : p(node3()), weight(1.0) {}
	glzRidgidSimple(node3 pin, double weightin) : p{ pin }, weight{ weightin } {}
/*	pos3(vert3 posin) : pos{ posin }, dir(vec3()), r(glzQuaternion()), rs(glzQuaternion()), scale(vec3(1.0, 1.0, 1.0)) {}

	void tick(double seconds) { pos += dir*seconds;	r *= rs*seconds; m.LoadIdentity();	m.translate(pos); m.scale(scale); m.loadQuanternion(r); }


	
	turn to
	move to
	align to

	*/

};





class glzSprite{ //atlassprite class

private:


public:

	// an atlas is defined by 2 tex coords, depth is used in case a texture array is used, coord are in the following config
	// c d
	// a b
	// with a being 0,0 and d is 1,1

	tex2 a, b, c, d;
	double depth;

	glzSprite() : a(tex2(0.0, 0.0)), b(tex2(1.0, 0.0)), c(tex2(0.0, 1.0)), d(tex2(1.0, 1.0)), depth(0.0){} // default numbers
	glzSprite(tex2 ain, tex2 bin, tex2 cin, tex2 din, double depthin) : a{ ain }, b{ bin }, c{ cin }, d{ din }, depth{ depthin }{} // direct initialization
	glzSprite(tex2 pos, tex2 dim, double depthin) : a{ tex2(pos.u, pos.v) }, b{ tex2(pos.u + dim.u, pos.v) }, c{ tex2(pos.u, pos.v + dim.v) }, d{ tex2(pos.u + dim.u, pos.v + dim.v) }, depth{ depthin }{} // simpler initialization

	glzSprite(unsigned int xdim, unsigned int ydim, unsigned int atlas, double depthin); // grid atlas initialization

	void make_polygons(vector<poly3> *pdata, double x, double y, double width, double height, int group, int atlas);
	void make_polygons(vector<poly3> *pdata, double x, double y, double width, double height, int group, int atlas, glzMatrix m);
};


class glzAtlasMap{ //atlassprite class

private:


public:


	vector<glzSprite> map;

	//glzAtlassprite() {} // default numbers
	glzAtlasMap(glzSprite a) { map.push_back(a); } // direct initialization
	glzAtlasMap(int w, int h); // direct initialization with preset


	void addsprite(glzSprite a) { map.push_back(a); }

};



#endif /* __glzvectormath__ */
