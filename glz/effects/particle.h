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

// particle systems
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#pragma once

#include <vector>
#include "..\utilities\vectormath.h"


using namespace std;

typedef struct{
	vert3 pos;
	vec3 dir;  // motion vector in units per second
	tex2 tx;
	float age;  // how old this particle is, affects the u texture coordinate
	float maxage; //when to die
	float drag; // how much it's effected by drag
	float gravity;
	float weight;
	bool active;
}glzSimpleParticle;

class glzSimpleParticleSystem{
private: 
	
	float time;
	float scale;
	bool running;
	unsigned int pc;
	bool clampx, clampy, clampz;
	vector<glzSimpleParticle> p;

public:
	glzSimpleParticleSystem();
	void set_time(float t) { time = t; }
	void set_environment(float scale_in);
	void set_clamp(bool clampx_in, bool clampy_in, bool clampz_in);

	// spawners
	void spawn_burst(unsigned int num, vert3 pos_in, float v_in, float mag, float maxage_in, float agediff, float drag_in, float dragdiff, float gravity, float weightin);

	void spawn_line_burst(unsigned int num, vert3 pos_in, float width, float v_in, float mag, float maxage_in, float agediff, float drag_in, float dragdiff, float gravity, float weightin);

	// aditional effects

	void collide_plane(vec3 normal, vert3 pl);
	void collide_plane_y(float y, bool up);

	void singularity(vert3 p, float range, float strength);
	
	void noise(float magnitude);
	void drag(float magnitude);
	void terminal_velocity(float magnitude);
	void gravity(vec3 gv);



	// final steps

	bool tick();
	void render_out();

	
};



typedef struct{
	node3 p;
	tex2 t,tscale;
	glzPrimitive shape;  // how old this particle is, affects the u texture coordinate
	float maxage; //when to die
	float drag; // how much it's effected by drag
	float gravity;
	float noise;
	bool active;
	bool isRidgid; // is this particle affected by physics
	bool selfColide; // does this particle colide with other particles
	bool selfGravity;
	float attract, repulse; // repulstion is a power of two from attraction

}glzMediumParticle;