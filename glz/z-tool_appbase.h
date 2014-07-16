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

// global z-tool app data
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git



#include "ztool-type.h"


#ifndef __appbase__
#define global __appbase__




class glzAppinitializationData{

public:

	wchar_t WINDOW_TITLE[255];
	char LOGFILE[255];
	bool  DISPLAY_ERRORS;						// set to false if you want to suppress errors
	bool  SHOW_FRAME;							// i normally dont but usefull if you want to make a more normal app
	bool  ALLOW_MAAXIMIZE;						// adds minimize/maximize buttons again usefull for a normal app
	bool  START_WINDOWED;						// starts in windowed mode instead of fullscreen
	bool  ALLOW_FULLSCREENSWITCH;				// doesn't work that well when showing a frame, infact dont even bother and set SHOW_FRAME to false
	bool  ALLOW_RESIZE;						// allows you to resice the window by dragging the borders, but only if SHOW_FRAME is true
	bool  START_CENTERED;						// centers the window which looks good IMHO
	unsigned int WINDOW_WIDTH;
	unsigned int  WINDOW_HEIGHT;
	unsigned int  FULLSCREEN_WIDTH;
	unsigned int  FULLSCREEN_HEIGHT;
	bool  NATIVE_FULLSCREEN;					// generally speaking this should be st to true, most often the current resolution is the higest anyway, overides the above settigns
	unsigned int  WINDOW_X;
	unsigned int  WINDOW_Y;
	bool  FORCE_OPENGL_VERSION;				// if you want to force openGL to a specific openGL version, generally set this to false for release, but for development it could be good to set it to true
	bool ENABLE_VSYNC;							// makes everything run a bit smoother, but at a constant rate, makes it impossible to get the current framerate

	glzAppinitializationData();
};



class glzAppinitialization{

public:
	glzAppinitializationData data;

	glzAppinitialization(); // grab the global data on construction

	void refresh(); // grab the global data in case it changed

	void set_title(const wchar_t title[255]);
	void set_logfile(const char logfile[255]);


	~glzAppinitialization(); // should return data into the global
	

};


#endif /* __appbase__ */
