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

// global z-tool app data
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git


#include "ztool_appbase.h"

glzAppinitializationData::glzAppinitializationData()
{
	
	swprintf(WINDOW_TITLE, L"ZeoBase GL Framework");
	sprintf(LOGFILE, "z-log");
	// WINDOW_TITLE( L"ZeoBase GL Framework"); // what name you want windows to call it, dont touch the weird L in front of it since it's unicode
	DISPLAY_ERRORS = false;						// set to false if you want to suppress errors
	SHOW_FRAME = true;							// i normally dont but usefull if you want to make a more normal app
	ALLOW_MAAXIMIZE = true;						// adds minimize/maximize buttons again usefull for a normal app
	START_WINDOWED = true;						// starts in windowed mode instead of fullscreen
	ALLOW_FULLSCREENSWITCH = false;				// doesn't work that well when showing a frame, infact dont even bother and set SHOW_FRAME to false
	ALLOW_RESIZE = false;						// allows you to resice the window by dragging the borders, but only if SHOW_FRAME is true
	START_CENTERED = true;						// centers the window which looks good IMHO
	WINDOW_WIDTH = 800;
	WINDOW_HEIGHT = 500;
	FULLSCREEN_WIDTH = 1920;
	FULLSCREEN_HEIGHT = 1080;
	NATIVE_FULLSCREEN = true;					// generally speaking this should be st to true, most often the current resolution is the higest anyway, overides the above settigns
	WINDOW_X = 0;
	WINDOW_Y = 0;
	FORCE_OPENGL_VERSION = true;				// if you want to force openGL to a specific openGL version, generally set this to false for release, but for development it could be good to set it to true
	ENABLE_VSYNC = true;						// makes everything run a bit smoother, but at a constant rate, makes it impossible to get the current framerate
	
}

static glzAppinitializationData appData;

glzAppinitialization::glzAppinitialization() { data = appData; } // grab the global data on construction

void glzAppinitialization::pull() { data = appData; } // grab the global data in case it changed
void glzAppinitialization::push() { appData = data; } // grab the global data in case it changed

void glzAppinitialization::set_title(const wchar_t title[255])	{ swprintf(appData.WINDOW_TITLE, title); swprintf(data.WINDOW_TITLE, title); }
void glzAppinitialization::set_logfile(const char logfile[255])	{ sprintf(appData.LOGFILE, logfile); sprintf(data.LOGFILE, logfile); }



glzAppinitialization::~glzAppinitialization() { appData = data; } // should return data into the global


