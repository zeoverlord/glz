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

// color class
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#pragma once

#include "type.h"

// todo implement http://www.color-hex.com/color-names.html colors
// at least the main 16 colors

#define COL_BLACK		glzColor(0.0f, 0.0f, 0.0f, 1.0f)
#define COL_WHITE		glzColor(1.0f, 1.0f, 1.0f, 1.0f)

#define COL_LIGHTGRAY	glzColor(0.33f, 0.33f, 0.33f, 1.0f)
#define COL_GRAY		glzColor(0.50f, 0.50f, 0.50f, 1.0f)
#define COL_DARKGRAY	glzColor(0.66f, 0.66f, 0.66f, 1.0f)

#define COL_RED			glzColor(1.0f, 0.0f, 0.0f, 1.0f)
#define COL_GREEN		glzColor(0.0f, 1.0f, 0.0f, 1.0f)
#define COL_BLUE		glzColor(0.0f, 0.0f, 1.0f, 1.0f)
#define COL_MAGENTA		glzColor(1.0f, 0.0f, 1.0f, 1.0f)
#define COL_CYAN		glzColor(0.0f, 1.0f, 1.0f, 1.0f)
#define COL_YELLOW		glzColor(1.0f, 1.0f, 0.0f, 1.0f)


using namespace std;
class glzColor {
public:
	float r, g, b, a;
	

	glzColor() : r(0.0), g(0.0), b(0.0), a(1.0) {}
	//color() : r(0.0), g(0.0), b(0.0) {}
	glzColor(float rin, float gin, float bin, float ain) : r(rin), g(gin), b(bin), a(ain) {}

	// these shouldn't really be used for blending
	glzColor glzColor::operator() (float inR, float inG, float inB, float inA) { r = inR; g = inG; b = inB; a = inA; return *this; }
	glzColor glzColor::operator() (float inR, float inG, float inB) { r = inR; g = inG; b = inB; return *this; }


	glzColor glzColor::operator+ (glzColor c) { return glzColor(r + c.r, g + c.g, b + c.b, a + c.a); }
	glzColor glzColor::operator+= (glzColor c) { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
	glzColor glzColor::operator+ (float c) { return glzColor(r + c, g + c, b + c, a); }
	glzColor glzColor::operator+= (float c) { r += c; g += c; b += c;  return *this; }

	glzColor glzColor::operator- (glzColor c) { return glzColor(r - c.r, g - c.g, b - c.b, a - c.a); }
	glzColor glzColor::operator-= (glzColor c) { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
	glzColor glzColor::operator- (float c) { return glzColor(r - c, g - c, b - c, a); }
	glzColor glzColor::operator-= (float c) { r -= c; g -= c; b -= c; return *this; }

	glzColor glzColor::operator* (glzColor c) { return glzColor(r * c.r, g * c.g, b * c.b, a * c.a); }
	glzColor glzColor::operator*= (glzColor c) { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
	glzColor glzColor::operator* (float c) { return glzColor(r * c, g * c, b * c, a * c); }
	glzColor glzColor::operator*= (float c) { r *= c; g *= c; b *= c; return *this; }

	glzColor glzColor::operator/ (glzColor c) { return glzColor(r / c.r, g / c.g, b / c.b, a / c.a); }
	glzColor glzColor::operator/= (glzColor c) { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }
	glzColor glzColor::operator/ (float c) { return glzColor(r / c, g / c, b / c, a); }
	glzColor glzColor::operator/= (float c) { r /= c; g /= c; b /= c; return *this; }


};


inline glzColor operator+ (float c, glzColor a) { return glzColor(a.r + c, a.g + c, a.b + c, a.a); }
inline glzColor operator- (float c, glzColor a) { return glzColor(a.r - c, a.g - c, a.b - c, a.a); }
inline glzColor operator* (float c, glzColor a) { return glzColor(a.r * c, a.g * c, a.b * c, a.a * c); }
inline glzColor operator/ (float c, glzColor a) { return glzColor(c / a.r, c / a.g, c / a.b, a.a); }


// Todo: pull in color conversion from glz