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


using namespace std;

#include "..\utilities\type.h"
#include "particle.h"
#include "..\utilities\glz.h"
#include "..\3d\geo.h"




glzSimpleParticleSystem::glzSimpleParticleSystem()
{
	scale = 100.0f;
	running = false;
	pc = 0;
}

void glzSimpleParticleSystem::set_environment(float scale_in)
{
	scale = scale_in;
	clampx = false;
	clampy = false;
	clampz = false;
}

void glzSimpleParticleSystem::set_clamp(bool clampx_in, bool clampy_in, bool clampz_in)
{
	clampx = clampx_in;
	clampy = clampy_in;
	clampz = clampz_in;
}

// spawners

void glzSimpleParticleSystem::spawn_burst(unsigned int num, vert3 pos_in, float v_in, float mag, float maxage_in, float agediff, float drag_in, float dragdiff, float gravity, float weightin)
{

	unsigned int i = 0;

	float vm = 0, vm2 = 0;

	vert3 pos;
	vec3 dir;
	tex2 tex(0.0, v_in);

	running = true;

	while (i < num) {
		if (pc >= 20000) return;


		dir = glzRandfs_vec3();
		pos = pos_in + glzRandfs_vec3() * 5;

		if (clampx) { pos.x = pos_in.x;  dir.x = 0.0f; }
		if (clampy) { pos.y = pos_in.y;  dir.y = 0.0f; }
		if (clampz) { pos.z = pos_in.z;  dir.z = 0.0f; }


		dir.normalize(glzRandfs()*mag*scale);
	

	
		p.push_back({ pos, dir, tex, 0.0f - glzRandf()*agediff, maxage_in + glzRandf()*agediff, drag_in + glzRandfs()*dragdiff, 1.0/*((glzRandf() / 2) + 0.5f)*gravity*/, weightin, false });
		i++;
		pc++;
	}

	
	return;
}


void glzSimpleParticleSystem::spawn_line_burst(unsigned int num, vert3 pos_in, float width, float v_in, float mag, float maxage_in, float agediff, float drag_in, float dragdiff, float gravity, float weightin)
{

	unsigned int i = 0;

	float vm = 0, vm2 = 0;

	vert3 pos;
	vec3 dir;
	tex2 tex(0.0, v_in);

	running = true;

	while (i < num) {
		if (pc >= 20000) return;


		//dir = glzRandfs_vec3();
		pos = pos_in + glzRandfs_vec3() * 5;
		pos.x += glzRandfs()*width / 2;

		if (clampx) { pos.x = pos_in.x;  dir.x = 0.0f; }
		if (clampy) { pos.y = pos_in.y;  dir.y = 0.0f; }
		if (clampz) { pos.z = pos_in.z;  dir.z = 0.0f; }


		dir.normalize(glzRandfs()*mag*scale);



		p.push_back({ pos, dir, tex, 0.0f - glzRandf()*agediff, maxage_in + glzRandf()*agediff, drag_in + glzRandfs()*dragdiff, 1.0/*((glzRandf() / 2) + 0.5f)*gravity*/, weightin, false });
		i++;
		pc++;
	}


	return;
}
// aditional effects


void glzSimpleParticleSystem::collide_plane(vec3 normal, vert3 pl)
{
	if (!running) return;


	for (auto& v : p)
		if (v.active)
		
			if ((v.dir.dot(normal) < 0.0f) && (normal.dot(v.pos.vectorTo(pl)) > 0.0f)) // ie. the motion vector and the plan is pointing towards each other
			{
			//	v.dir*=0.3;
				v.dir.reflect(normal.inverse());
				v.dir *= 0.6f;
				if (v.dir.magnitude() < scale)
					v.dir *= v.dir.magnitude() / scale;
			}
	
		
}

void glzSimpleParticleSystem::collide_plane_y(float y, bool up)
{
	if (!running) return;


	for (auto& v : p)
		if (v.active)

			if (up)
				if ((v.pos.y<y)&&(v.dir.y<0.0f)) 
				{
					v.dir.y*=-1.0f;
				}

			else
				if ((v.pos.y>y) && (v.dir.y>0.0f))
				{
					v.dir.y *= -0.9f;
				}

}


void glzSimpleParticleSystem::singularity(vert3 pa, float range, float strength)
// possitive values makes it an attractor, negative makes a repulsor
{
	for (auto& v : p)
		if (v.active)
		{
			
			vec3 b;
			//b = pa.vectorTo(v.pos);
			b.x = pa.x - v.pos.x;
			b.y = pa.y - v.pos.y;
			b.z = pa.z - v.pos.z;
			float dist = b.magnitude();
			if (dist < 10.0f) dist = 10.0f;
			b.normalize(strength);


			b /= pow(dist/range, 2);
						
			//	b*= (strength) / (dist);
				v.dir += b*time*scale;
			
		}
			
}

void glzSimpleParticleSystem::noise(float magnitude)
{
	// noise
	for (auto& v : p)
	if (v.active) v.dir += (magnitude*glzRandfs_vec3()*time*scale) / v.weight;

}


void glzSimpleParticleSystem::drag(float magnitude)
{

	// cause drag
	for (auto& v : p)
		if (v.active)
		{
			float speed = v.dir.magnitude();
			v.dir.normalize(speed - (speed*v.drag*magnitude*time));
			
		}

}

void glzSimpleParticleSystem::terminal_velocity(float magnitude)
{
	// maximum speed
	for (auto& v : p)
		if (v.active)

		{ 
			float speed = v.dir.magnitude();
			if (speed > magnitude) speed = magnitude;
			v.dir.normalize(speed);
		}
}






void glzSimpleParticleSystem::gravity(vec3 gv)
{
	for (auto& v : p)
		if (v.active) v.dir += gv*time*scale*v.gravity;
}





// final steps
bool glzSimpleParticleSystem::tick()
{
	if (!running) return false;

	if (p.empty()) { p.clear(); running = false; pc = 0; return false; }

	// advance age

	for (auto& v : p) // this is more like it
		{
		v.age += time;
		if (v.age >= v.maxage)	v.active = false;
		else if (v.age < 0.0f) v.active = false;
		else  v.active = true;
		}

	

	
	// set u
	for (auto& v : p) 
		if (v.active)
		{
			v.tx.u = v.age / v.maxage;
			if (v.tx.u > 0.99f) v.tx.u = 0.99f;
		}

	// kill



	// only uggly part realy, but it cant be helped as i need access to the erase function which lies in the container and not the contained object
	int i2 = 0;

	auto i = p.begin();

	 i2 = 0; // o yes this is way uggly, but i think it woks
	 while (i < p.end()) {

		 if ((!p[i2].active) && (p[i2].age >0.0f))
		 {
			i = p.erase(i);
			pc--;
		 }
		else {
		++i;
		i2++;
		}
	
	}


	 //clamp coordinates
	 for (auto& v : p)
		 if (v.active)
		 {
			 if (clampx) v.dir.x = 0.0;
			 if (clampy) v.dir.y = 0.0;
			 if (clampz) v.dir.z = 0.0;
		 }


	//move
	 for (auto& v : p) 
		 if (v.active) v.pos += v.dir*time;



	return true;
}

void glzSimpleParticleSystem::render_out(void)
{

	if (!running) return;

	vector<point3> pdata;
	point3 pt;
	pt.n.x = 0.0;
	pt.n.y = 0.0;
	pt.n.z = 1.0;


	for (glzSimpleParticle v : p)
	{
		if ((v.active) && (v.age > 0.0))
		{
			pt.v.x = v.pos.x;
			pt.v.y = v.pos.y;
			pt.v.z = v.pos.z;

			pt.t.u = v.tx.u;
			pt.t.v = v.tx.v;

			pdata.push_back(pt);
		}
			
	}

	glzDirectPointVectorRender(pdata);

}


