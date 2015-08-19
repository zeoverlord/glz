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

// common glz typedefs
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#include <vector>
#include <algorithm>


using namespace std;


#ifndef __glztype__
#define __glztype__


enum class glzVAOType {NONE, AUTO, VERTEX, VERTEX_TEXTURE, VERTEX_NORMAL, VERTEX_TEXTURE_NORMAL };
enum class glzOrigin { BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT, CENTERED };
enum class glzAxis { X, Y, Z };
enum class glzDistribution { UNIFORM, NORMAL, GAMMA, EXPONENTIAL };

enum class glzTexFilter { NONE, NEAREST, LINEAR, BILINEAR, TRILINEAR, ANSIO_2, ANSIO_4, ANSIO_8, ANSIO_16, ANSIO_MAX };
enum class glzTexCompression { UNCOMPRESSED, COMPRESSED };

enum class glzBoundingScan { CENTER_X, CENTER_Y, LEFT, RIGHT, TOP, BOTTOM, WIDTH, HEIGHT};


enum class glzColorChannels { RED, GREEN, BLUE, ALPHA, HUE, HUE2, CROMA, CROMA2, VALUE, LIGHTNESS, INTENSITY, LUMA, SHSV, SHSL, SHSI };
enum class glzColorType { RGB_8, RGB_32F, RGBA_8, RGBA_32F, HSL_8, HSL_32F, HSV_8, HSV_32F, HSI_8, HSI_32F, HCY_8, HCY_32F };


enum class glzPrimitive {
	CUBE, SPHERE, PYRAMID, TETRAHEDRON, HEXAHEDRON, OCTAHEDRON, DODECAHEDRON, ICOSAHEDRON, ICOSIDODECAHEDRON, TORUS,		// basic shapes - sphere and torus not implemented
	GEODESIC_TETRA, GEODESIC_HEXA, GEODESIC_OCTA, GEODESIC_DODECA, GEODESIC_ICOSA,										// geodesics - not implemented
	DOME, DISC, CYLINDER, STAR_DODECAHEDRON,																			// other shapes - not implemented
	FSQ, RANDOM_POINT, SPRITE_ATLAS_ARRAY																				// random structures
};

//enum class glzatlasMapType{NONE,GRID};
enum class glzIGTType { NONE, DISPLACE_ADD };

enum class glzTTType { NONE, VERTEX_COORD_ADOPT, ATLASARRAY, ATLAS_CUBE_TBS, ATLAS_CUBE_INDFACE, ATLAS_CUBE_CROSS };

enum class glzSceneGraphType { 
	NONE,			// shouldn't really exist
	DELETABLE,		// to be deleted before next round
	DECORATION,		// contains no interaction only rendering
	STATIC,			// does not move but may hurt player and/or recive damage
	PLAYER,			// remote controlled by the user
	ENTETY,			// has movement but no AI
	STATIC_ENEMY,	// has AI but no movement
	ENEMY			// freemoving enemy with AI
};

enum class glzSceneGraphSubType {
	NONE

};

enum class glzSceneGraphSubMessage {
	NONE, MOVE, PLAYER_MOVE, DAMAGE

};

class texturecontainer{

public:
	unsigned int uid;
	string objectname;
	unsigned int width;
	unsigned int height;
	unsigned int handle;
	unsigned int type;
	bool loaded;

	texturecontainer() 
	{
		uid = 0;
		objectname = "NULL";
		width = 1;
		height = 1;
		handle = 0;
		type = 3;
		loaded = false;
	}


	texturecontainer(string objn) : objectname(objn), width(1), height(1), handle(0), type(3), loaded(false) {}
};




// todo, move to own cpp file

class glzQuaternion;

// units - well at least it's a beginning, will continue at a later date, probably will be rewritten once visual studio gets user defined literals
/*

class glzsecond;
class glzlength;
class glzspeed;


class glzsecond{ //time in secconds
	double S;

public:
	glzsecond(double sec) : S(sec) {}
	double get() { return S; }
	double glzsecond::operator= (glzsecond b) { return b.get(); }
};

class glzlength{ // length in meters
	double L;

public:
	glzlength(double meter) : L(meter) {}
	double get() { return L; }
	double glzlength::operator= (glzlength b) { return b.get(); }

	glzspeed glzlength::operator/ (glzsecond b);
};

class glzspeed{ // speed in meters per second
	double S;

public:
	glzspeed(double mps) : S(mps) {}
	double get() { return S; }
	double glzspeed::operator= (glzspeed b) { return b.get(); }

	
};

glzspeed glzlength::operator/ (glzsecond b) { glzspeed sp(L / b.get()); return sp; }

*/






#endif /* __glztype__ */
