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

// glz texture toolkit - will load any image in the world as long as it's a small subset of the .tga format
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git


#include "type.h"
#include "..\image\tex.h"
#include <windows.h>													// Header File For The Windows Library
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library
#include <gl/glext.h>

#ifndef __glzresourcemanager__
#define __glzresourcemanager__


typedef struct
{
	string name;
	string filename;
	glzTexFilter filter;
	int priority; // 0 is alreaady loaded, 1 is higest priority
} txLoadContainer;


class resinit
{
public:

	unsigned int defaulttexture;
	unsigned int idcounter;
	bool loaded;

	resinit()
	{
		idcounter = 0;
		loaded = false;
		defaulttexture = 0;
	}
	~resinit()
	{
	//	glDeleteTextures(1, &defaulttexture);
	}

	unsigned int getNewId(void)	{idcounter++; return idcounter;}

	unsigned int getDefaultexture()
	{
		if (!loaded)
		{ 
			defaulttexture = glzGetDefaultTextturehandle();
			loaded = true;
		}

		return defaulttexture;

	}

};



class glzResourcemanager
{


public:

	void createTexture(string name, string filename, glzTexFilter filter);
	void createTexture(string name, string filename, glzTexFilter filter, int priority);


	texturecontainer* gettexture(string name);

	unsigned int gettextureHandle(string name);

	void manipulate(string name);

	bool load_one(void);

	void load_all(void);

};




#endif /* __glzresourcemanager__ */



