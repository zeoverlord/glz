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

#include <math.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "ztool-resourcemanager.h"
#include <stdio.h>
#include <vector>

using namespace std;

static resinit ri;




static vector<txLoadContainer> textureloadstore;
static vector<texturecontainer> texturestore;



void glzResourcemanager::createTexture(string name, string filename, glzTexFilter filter)
{
	createTexture(name, filename, filter, 5);
}


	
void glzResourcemanager::createTexture(string name, string filename, glzTexFilter filter, int priority)
{

	texturecontainer tx;
	txLoadContainer tl;

	bool foundname=false;

	for (auto& a : texturestore)
		if (a.objectname == name) {	foundname = true;}

	if (foundname==false)
	{ 
		tx.objectname = name;
		tx.handle = ri.getDefaultexture();
		//tx.handle = glzLoadTexture(filename, filter);	
		tx.uid = ri.getNewId();
		texturestore.push_back(tx);
	}

	tl.name = name;
	tl.filename = filename;
	tl.filter = filter;
	tl.priority = priority;

	
	textureloadstore.push_back(tl);
	return;
}

	
texturecontainer* glzResourcemanager::gettexture(string name)
{
	for (auto& a : texturestore)
		if (a.objectname == name) {
			//tcon =;
			return &a;}

	texturecontainer tx;
	tx.objectname = name;
	tx.handle = ri.getDefaultexture();
	tx.uid = ri.getNewId();
	texturestore.push_back(tx);

	for (auto& a : texturestore)
		if (a.objectname == name) {
			//tcon =;
			return &a;
		}

	return nullptr;

}

unsigned int glzResourcemanager::gettextureHandle(string name)
{
	for (auto a : texturestore)
		if (a.objectname == name) 
		{
			return a.handle;
		}


	texturecontainer tx;
	tx.objectname = name;
	tx.handle = ri.getDefaultexture();
	tx.uid = ri.getNewId();
	texturestore.push_back(tx);

	return tx.handle;

}


void glzResourcemanager::manipulate(string name)
{
	for (auto& a : texturestore)
		if (a.objectname == name)
		{
			a.handle = ri.getDefaultexture();
		}
}


bool glzResourcemanager::load_one(void)
{
	

	if (textureloadstore.size() == 0) return false;

	sort(textureloadstore.begin(), textureloadstore.end(), [](const txLoadContainer& a, const txLoadContainer& b) {	return a.priority < b.priority;	});
	
	auto tl = textureloadstore.front();
	textureloadstore.erase(textureloadstore.begin());


	for (auto& a : texturestore)
		if (a.objectname == tl.name) {	
			a.handle = glzLoadTexture(tl.filename, tl.filter);
			return true;
		}

	return true;
}


void glzResourcemanager::load_all(void)
{

	sort(textureloadstore.begin(), textureloadstore.end(), [](const txLoadContainer& a, const txLoadContainer& b) {	return a.priority < b.priority;	});

	while (load_one()){}

	return;
}