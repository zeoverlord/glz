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

#include <fstream>
#include <string.h>
#include "ztool-shader.h"
#include "ztool-shader-programs.h"
#include <stdio.h>
#include <windows.h>													// Header File For The Windows Library
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library
#include <gl/glext.h>

using namespace std;


static PFNGLBINDATTRIBLOCATIONPROC				glBindAttribLocation;

static PFNGLDELETESHADERPROC					glDeleteShader;
static PFNGLGETSHADERIVPROC						glGetShaderiv;
static PFNGLGETPROGRAMIVPROC					glGetProgramiv;
static PFNGLPROGRAMPARAMETERIARBPROC			glProgramParameteri;

static PFNGLDETACHSHADERPROC                    glDetachShader;
static PFNGLCREATESHADERPROC			        glCreateShader;
static PFNGLSHADERSOURCEPROC                    glShaderSource;
static PFNGLCOMPILESHADERPROC                   glCompileShader;
static PFNGLCREATEPROGRAMPROC					glCreateProgram;
static PFNGLATTACHSHADERPROC					glAttachShader;
static PFNGLLINKPROGRAMPROC                     glLinkProgram;
static PFNGLUSEPROGRAMPROC						glUseProgram;
static PFNGLVALIDATEPROGRAMPROC                 glValidateProgram;


static PFNGLUNIFORM1FARBPROC                       glUniform1f;
static PFNGLUNIFORM2FARBPROC                       glUniform2f;
static PFNGLUNIFORM3FARBPROC                       glUniform3f;
static PFNGLUNIFORM4FARBPROC                       glUniform4f;
static PFNGLUNIFORM1IARBPROC                       glUniform1i;
static PFNGLUNIFORM2IARBPROC                       glUniform2i;
static PFNGLUNIFORM3IARBPROC                       glUniform3i;
static PFNGLUNIFORM4IARBPROC                       glUniform4i;
static PFNGLUNIFORM1FVARBPROC                      glUniform1fv;
static PFNGLUNIFORM2FVARBPROC                      glUniform2fv;
static PFNGLUNIFORM3FVARBPROC                      glUniform3fv;
static PFNGLUNIFORM4FVARBPROC                      glUniform4fv;
static PFNGLUNIFORM1IVARBPROC                      glUniform1iv;
static PFNGLUNIFORM2IVARBPROC                      glUniform2iv;
static PFNGLUNIFORM3IVARBPROC                      glUniform3iv;
static PFNGLUNIFORM4IVARBPROC                      glUniform4iv;
static PFNGLUNIFORMMATRIX2FVPROC                glUniformMatrix2fv;
static PFNGLUNIFORMMATRIX3FVPROC                glUniformMatrix3fv;
static PFNGLUNIFORMMATRIX4FVPROC                glUniformMatrix4fv;

static PFNGLGETUNIFORMFVPROC				    glGetUniformfv;
static PFNGLGETUNIFORMIVPROC					glGetUniformiv;
static PFNGLGETSHADERINFOLOGPROC                glGetShaderInfoLog;
static PFNGLGETATTACHEDSHADERSPROC              glGetAttachedShaders;
static PFNGLGETUNIFORMLOCATIONPROC              glGetUniformLocation;
static PFNGLGETACTIVEUNIFORMPROC                glGetActiveUniform;
static PFNGLGETSHADERSOURCEPROC                 glGetShaderSource;

static bool isinited_shd;

static bool passtrough_program_inited;
static unsigned int passtrough_program;

static bool basic_program_inited;
static unsigned int basic_program;

static bool colortint_program_inited;
static unsigned int colortint_program;

static bool tilemap_program_inited;
static unsigned int tilemap_program;

static bool tiledSprite_program_inited;
static unsigned int tiledSprite_program;


static vector<GLint> tempprogram;



// now i made all of these static, so you shouldn't have to initialize them again
void ini_shd(void)
{
	isinited_shd=true;
	glBindAttribLocation= (PFNGLBINDATTRIBLOCATIONPROC) wglGetProcAddress("glBindAttribLocation");
	
	glDeleteShader				= (PFNGLDELETESHADERPROC) wglGetProcAddress("glDeleteShader");
	glDetachShader				= (PFNGLDETACHSHADERPROC) wglGetProcAddress("glDetachShader");
	glCreateShader				= (PFNGLCREATESHADERPROC) wglGetProcAddress("glCreateShader");
	glShaderSource				= (PFNGLSHADERSOURCEPROC) wglGetProcAddress("glShaderSource");
	glCompileShader				= (PFNGLCOMPILESHADERPROC) wglGetProcAddress("glCompileShader");
	glCreateProgram				= (PFNGLCREATEPROGRAMPROC) wglGetProcAddress("glCreateProgram");
	glAttachShader				= (PFNGLATTACHSHADERPROC) wglGetProcAddress("glAttachShader");
	glLinkProgram				= (PFNGLLINKPROGRAMPROC) wglGetProcAddress("glLinkProgram");
	glUseProgram				= (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
	glValidateProgram			= (PFNGLVALIDATEPROGRAMPROC) wglGetProcAddress("glValidateProgram");
	glGetShaderiv				= (PFNGLGETSHADERIVPROC) wglGetProcAddress("glGetShaderiv");
	glGetProgramiv				= (PFNGLGETPROGRAMIVPROC) wglGetProcAddress("glGetProgramiv");
	glProgramParameteri				= (PFNGLPROGRAMPARAMETERIARBPROC) wglGetProcAddress("glProgramParameteri");
	
	glGetShaderInfoLog			= (PFNGLGETSHADERINFOLOGPROC) wglGetProcAddress("glGetShaderInfoLog");
	glGetAttachedShaders		= (PFNGLGETATTACHEDSHADERSPROC) wglGetProcAddress("glGetAttachedShaders");
	glGetUniformLocation		= (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress("glGetUniformLocation");
	glGetActiveUniform			= (PFNGLGETACTIVEUNIFORMPROC) wglGetProcAddress("glGetActiveUniform");
	glGetUniformfv				= (PFNGLGETUNIFORMFVPROC) wglGetProcAddress("glGetUniformfv");
	glGetUniformiv				= (PFNGLGETUNIFORMIVPROC) wglGetProcAddress("glGetUniformiv");
	glGetShaderSource			= (PFNGLGETSHADERSOURCEPROC) wglGetProcAddress("glGetShaderSource");
                                                                     
	glUniform1f= (PFNGLUNIFORM1FARBPROC) wglGetProcAddress("glUniform1fARB");
	glUniform2f= (PFNGLUNIFORM2FARBPROC) wglGetProcAddress("glUniform2fARB");
	glUniform3f= (PFNGLUNIFORM3FARBPROC) wglGetProcAddress("glUniform3fARB");
	glUniform4f= (PFNGLUNIFORM4FARBPROC) wglGetProcAddress("glUniform4fARB");
	glUniform1i= (PFNGLUNIFORM1IARBPROC) wglGetProcAddress("glUniform1iARB");
	glUniform2i= (PFNGLUNIFORM2IARBPROC) wglGetProcAddress("glUniform2iARB");
	glUniform3i= (PFNGLUNIFORM3IARBPROC) wglGetProcAddress("glUniform3iARB");
	glUniform4i= (PFNGLUNIFORM4IARBPROC) wglGetProcAddress("glUniform4iARB");
	glUniform1fv= (PFNGLUNIFORM1FVARBPROC) wglGetProcAddress("glUniform1fvARB");
	glUniform2fv= (PFNGLUNIFORM2FVARBPROC) wglGetProcAddress("glUniform2fvARB");
	glUniform3fv= (PFNGLUNIFORM3FVARBPROC) wglGetProcAddress("glUniform3fvARB");
	glUniform4fv= (PFNGLUNIFORM4FVARBPROC) wglGetProcAddress("glUniform4fvARB");
	glUniform1iv= (PFNGLUNIFORM1IVARBPROC) wglGetProcAddress("glUniform1ivARB");
	glUniform2iv= (PFNGLUNIFORM2IVARBPROC) wglGetProcAddress("glUniform2ivARB");
	glUniform3iv= (PFNGLUNIFORM3IVARBPROC) wglGetProcAddress("glUniform3ivARB");
	glUniform4iv= (PFNGLUNIFORM4IVARBPROC) wglGetProcAddress("glUniform4ivARB");
	glUniformMatrix2fv= (PFNGLUNIFORMMATRIX2FVPROC) wglGetProcAddress("glUniformMatrix2fv");
	glUniformMatrix3fv= (PFNGLUNIFORMMATRIX3FVPROC) wglGetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv= (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");

	if (!passtrough_program_inited) {
		passtrough_program = glzShaderLoadString(passtrough_vertex, passtrough_fragment, glzVAOType::AUTO);
		glzShaderLink(passtrough_program);
		passtrough_program_inited = true;
	}

	if (!basic_program_inited) {
		basic_program = glzShaderLoadString(basic_vertex, basic_fragment, glzVAOType::AUTO);
		glzShaderLink(basic_program);
		basic_program_inited = true;

	}

	if (!colortint_program_inited) {
		colortint_program = glzShaderLoadString(colortint_vertex, colortint_fragment, glzVAOType::AUTO);
		glzShaderLink(colortint_program);
		basic_program_inited = true;

	}
	

	if (!tilemap_program_inited) {
		tilemap_program = glzShaderLoadString(tilemap_vertex, tilemap_fragment, glzVAOType::AUTO);
		glzShaderLink(tilemap_program);
		tilemap_program_inited = true;

	}

	if (!tiledSprite_program_inited) {
		tiledSprite_program = glzShaderLoadString(tiledSprite_vertex, tiledSprite_fragment, glzVAOType::AUTO);
		glzShaderLink(tiledSprite_program);
		tiledSprite_program_inited = true;

	}
	

	
}

void glzShaderProgramPush()
{
	GLint tp;
	glGetIntegerv(GL_CURRENT_PROGRAM, &tp);
	tempprogram.push_back(tp);

}

void glzShaderProgramPop()
{

	//glUseProgram(tempprogram);

	if (!tempprogram.empty())
	{
		glUseProgram(tempprogram.back());	
		tempprogram.pop_back();
	}


}




unsigned long getFileLength(ifstream& file)
{
    if(!file.good()) return 0;
    
//    unsigned long pos=file.tellg();
    file.seekg(0,ios::end);
	unsigned long const len = file.tellg();
    file.seekg(ios::beg);
    
    return len;
}


void loadShaderString(unsigned int program, string const shadercode, glzShadertype type)  //loads a vertex shader in glsl format
{

	unsigned int  ShaderObject;

	// start compilling the source
	if (type == glzShadertype::VERTEX_SHADER)	ShaderObject = glCreateShader(GL_VERTEX_SHADER);
	else if (type == glzShadertype::FRAGMENT_SHADER)	ShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	else if (type == glzShadertype::GEOMETRY_SHADER)	ShaderObject = glCreateShader(GL_GEOMETRY_SHADER);
	else return;
	glShaderSource(ShaderObject, 1, (const char **)&shadercode, NULL);

	int compiled = 0;
	char str[4096];
	// Compile the vertex and fragment shader, and print out any arrors

	glCompileShader(ShaderObject);
	glGetShaderiv(ShaderObject, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {

		glGetShaderInfoLog(ShaderObject, sizeof(str), NULL, str);
		if (type == glzShadertype::VERTEX_SHADER)	MessageBoxA(NULL, str, "vertex Shader Compile Error", MB_OK | MB_ICONEXCLAMATION);
		else if (type == glzShadertype::FRAGMENT_SHADER)	MessageBoxA(NULL, str, "fragment Shader Compile Error", MB_OK | MB_ICONEXCLAMATION);
		else if (type == glzShadertype::GEOMETRY_SHADER)	MessageBoxA(NULL, str, "geometry Shader Compile Error", MB_OK | MB_ICONEXCLAMATION);
		else MessageBoxA(NULL, str, "unknown Shader Compile Error", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	glAttachShader(program, ShaderObject);

	glDeleteShader(ShaderObject);

	return;
}


// this function will load a file, compile it and attach it to a program object
void loadShaderFile(unsigned int program, string const filename, glzShadertype type)  //loads a vertex shader in glsl format
{

	unsigned int  ShaderObject;
	char*     ShaderSource;
	ifstream file;
	file.open(filename, ios::in);
	if (!file) { return; }

	unsigned long const len = getFileLength(file);

	if (len == 0) { return; }   // "Empty File" 


	ShaderSource = new char[len + 1];
	if (ShaderSource == 0) { return; }   // can't reserve memory

	ShaderSource[len] = 0;  // len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value...

	unsigned int i = 0;
	while (file.good())
	{
		ShaderSource[i++] = file.get();       // get character from file
		if (i>len) i = len;   // coding guidelines...
	}
	i--;
	ShaderSource[i] = 0;  // 0 terminate it.

	file.close();

	// i just have to say this about file loading, it is bloddy anoying, this should be like a single line
	// and maybe it will whenever i get arround to doing the packed file loading i want to do.


	// start compilling the source
	if (type==glzShadertype::VERTEX_SHADER)	ShaderObject = glCreateShader(GL_VERTEX_SHADER);
	else if (type == glzShadertype::FRAGMENT_SHADER)	ShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	else if (type == glzShadertype::GEOMETRY_SHADER)	ShaderObject = glCreateShader(GL_GEOMETRY_SHADER);
	else return;
	glShaderSource(ShaderObject, 1, (const char **)&ShaderSource, NULL);

	delete[] ShaderSource;

	int compiled = 0;
	char str[4096];
	// Compile the vertex and fragment shader, and print out any arrors

	glCompileShader(ShaderObject);
	glGetShaderiv(ShaderObject, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {

		glGetShaderInfoLog(ShaderObject, sizeof(str), NULL, str);
		if (type == glzShadertype::VERTEX_SHADER)	MessageBoxA(NULL, str, "vertex Shader Compile Error", MB_OK | MB_ICONEXCLAMATION);
		else if (type == glzShadertype::FRAGMENT_SHADER)	MessageBoxA(NULL, str, "fragment Shader Compile Error", MB_OK | MB_ICONEXCLAMATION);
		else if (type == glzShadertype::GEOMETRY_SHADER)	MessageBoxA(NULL, str, "geometry Shader Compile Error", MB_OK | MB_ICONEXCLAMATION);
		else MessageBoxA(NULL, str, "unknown Shader Compile Error", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	glAttachShader(program, ShaderObject);

	glDeleteShader(ShaderObject);

	return;
}




unsigned int glzShaderLoadString(string const vert, string const frag, glzVAOType type)
{
	if (!isinited_shd) ini_shd();

	unsigned int  program;

	program = glCreateProgram();


	loadShaderString(program, vert, glzShadertype::VERTEX_SHADER);
	loadShaderString(program, frag, glzShadertype::FRAGMENT_SHADER);



	if (type == glzVAOType::AUTO)
	{
		glBindAttribLocation(program, 0, "Position");
		glBindAttribLocation(program, 1, "TexCoord");
		glBindAttribLocation(program, 2, "Normal");
	}

	return program;
}

// this specific function will only work if you have openGL 3.2 installed because of the geometry shader which should be any dx 10 class hardware, basically gf8xxx and above
unsigned int glzShaderLoad(string const file_vert, string const file_geo, string const file_frag, glzVAOType type)
{
	if(!isinited_shd) ini_shd();

	unsigned int  program;

	program	   = glCreateProgram();
		

	loadShaderFile(program, file_vert, glzShadertype::VERTEX_SHADER);
	loadShaderFile(program, file_geo, glzShadertype::GEOMETRY_SHADER);
	loadShaderFile(program, file_frag, glzShadertype::FRAGMENT_SHADER);


	
	if (type == glzVAOType::AUTO)
	{	
	glBindAttribLocation(program,0, "Position");
	glBindAttribLocation(program,1, "TexCoord");
	glBindAttribLocation(program,2, "Normal");
	}

    return program;
}

// this function loads only the certex and fragment programs and like the above it sets the vertex attribute locations
// in the rare case you want to set these yourself then use any number you like above lets say 5, its really 1 but i might add support for like just vertex + texture and such later on
// one thing to note is that the program is not linked, that function is lower down
unsigned int glzShaderLoad(string const file_vert, string const file_frag, glzVAOType type)
{
	if(!isinited_shd) ini_shd();

	unsigned int  program;

	program	   = glCreateProgram();
		

	loadShaderFile(program, file_vert,glzShadertype::VERTEX_SHADER);
	loadShaderFile(program, file_frag, glzShadertype::FRAGMENT_SHADER);

	
	if (type == glzVAOType::AUTO)
	{	
	glBindAttribLocation(program,0, "Position");
	glBindAttribLocation(program,1, "TexCoord");
	glBindAttribLocation(program,2, "Normal");
	}

    return program;

}


// this program links the program, this is so that you can set up some custom settings, like those for the geometry shader, i will refer you to my geometry shader tutorials for that
void glzShaderLink(unsigned int program)
{
	if(!isinited_shd) ini_shd();

	int linked   = 0;

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS,&linked);

	if (!linked) {
		MessageBoxA (HWND_DESKTOP, "can't link shaders", "Error", MB_OK | MB_ICONEXCLAMATION);
	    return;
	}


return;
}



void glzShaderUsePasstrough(void)
{	glUseProgram(passtrough_program);
	return;
}

void glzShaderUseBasic(void)
{
	glUseProgram(basic_program);
	return;
}

void glzShaderUseColorTint(void)
{
	glUseProgram(colortint_program);
	return;
}


void glzShaderUseTilemap(void)
{
	glUseProgram(tilemap_program);
	return;
}

void glzShaderUseTiledSprite(void)
{
	glUseProgram(tiledSprite_program);
	return;
}




unsigned int glzShaderReurnPasstrough(void)
{
	return passtrough_program;
}

unsigned int glzShaderReurnBasic(void)
{
	return basic_program;
}

unsigned int glzShaderReurnColorTint(void)
{
	return colortint_program;
}

unsigned int glzShaderReurnTilemap(void)
{
	return tilemap_program;
}

unsigned int glzShaderReurnTiledSprite(void)
{
	return tiledSprite_program;
}




// uniforms

void glzUniform1i(unsigned int ProgramObject, const string name, int v)
{
	unsigned int loc = glGetUniformLocation(ProgramObject, (GLchar*)&name);
	//glUseProgram(ProgramObject);
	glUniform1i(loc, v);
}

void glzUniform1f(unsigned int ProgramObject, const string name, float v)
{
	unsigned int loc = glGetUniformLocation(ProgramObject, (GLchar*)&name);
	//glUseProgram(ProgramObject);
	glUniform1f(loc, v);
}

void glzUniform2f(unsigned int ProgramObject, const string name, float v1, float v2)
{
	unsigned int loc = glGetUniformLocation(ProgramObject, (GLchar*)&name);
	//glUseProgram(ProgramObject);
	glUniform2f(loc, v1, v2);
}

void glzUniform4f(unsigned int ProgramObject, const string name, float v1, float v2, float v3, float v4)
{
	unsigned int loc = glGetUniformLocation(ProgramObject, (GLchar*)&name);
	//glUseProgram(ProgramObject);
	glUniform4f(loc, v1, v2, v3, v4);
}

void glzUniformMatrix4fv(unsigned int ProgramObject, const string name, float v[16])
{
	unsigned int loc = glGetUniformLocation(ProgramObject, (GLchar*)&name);
	//glUseProgram(ProgramObject);
	glUniformMatrix4fv(loc, 1, GL_FALSE, v);
}


void glzUniformMatrix4fv(unsigned int ProgramObject, const string name, double a[16])
{
	float b[16];

	int v[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	for (auto i : v)
		b[i] = (float)a[i];

	unsigned int loc = glGetUniformLocation(ProgramObject, (GLchar*)&name);
	//glUseProgram(ProgramObject);
	glUniformMatrix4fv(loc, 1, GL_FALSE, b);
}


void glzUniformMatrix4fv(unsigned int ProgramObject, const string name, glzMatrix m)
{

	float mtemp[16];
	m.transferMatrix(&mtemp[0]);

	unsigned int loc = glGetUniformLocation(ProgramObject, (GLchar*)&name);
//	glUseProgram(ProgramObject);
	glUniformMatrix4fv(loc, 1, GL_FALSE, mtemp);
}







// now this is all pretty basic stuff but here is a list of things i like to add later on

// shader script, which is basically a single file you call with all the shaders in it and how to load it.
// tesselation shader support
// precompiled shaders
// in code loading, which is more or less using an array to load a shader
// default shaders, much like those included in the toolkit but allready defined in the code using the above function to load them.