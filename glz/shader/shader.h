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

// glz shader toolkit - Warning does not produce actual shade
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git

#pragma once

#include "..\utilities\type.h"
#include "..\utilities\vectormath.h"
#include "..\utilities\color.h"

		//type signifies the type of data to choose from, if set at GLZ_AUTO it chooses the default settings

enum class glzShadertype{ VERTEX_SHADER,FRAGMENT_SHADER, GEOMETRY_SHADER };

unsigned int glzShaderLoad(string const file_vert, string const file_geo, string const file_frag, glzVAOType type);
unsigned int glzShaderLoad(string const file_vert, string const file_frag, glzVAOType type);
unsigned int glzShaderLoadString(string const vert, string const frag, glzVAOType type);
void glzShaderLink(unsigned int program);

void glzShaderProgramPush();
void glzShaderProgramPop();

// fixed shaders
void glzShaderUsePasstrough(void);
void glzShaderUseBasic(void);
void glzShaderUseColorTint(void);
void glzShaderUseTilemap(void);
void glzShaderUseTiledSprite(void);



unsigned int glzShaderReurnPasstrough(void);
unsigned int glzShaderReurnBasic(void);
unsigned int glzShaderReurnColorTint(void);
unsigned int glzShaderReurnTilemap(void);
unsigned int glzShaderReurnTiledSprite(void);


//uniform

void glzUniform1i(unsigned int ProgramObject, const string name, int v);
void glzUniform1f(unsigned int ProgramObject, const string name, float v);
void glzUniform2f(unsigned int ProgramObject, const string name, float v1, float v2);
void glzUniform4f(unsigned int ProgramObject, const string name, float v1, float v2, float v3, float v4);
void glzUniform4f(unsigned int ProgramObject, const string name, glzColor color);

void glzUniformMatrix4fv(unsigned int ProgramObject, const string name, float v[16]);
void glzUniformMatrix4fv(unsigned int ProgramObject, const string name, double a[16]);
void glzUniformMatrix4fv(unsigned int ProgramObject, const string name, glzMatrix m);

// make sure this replaces all uses of the openGL set uniform functions
//void glzSetuniform(int type, int location, GLsizei count, GLboolean transpose, const GLfloat *value)

// for now just call one of the two first ones with "glzVAOType::AUTO" as the type then the last one, there will be much more here in future versions


