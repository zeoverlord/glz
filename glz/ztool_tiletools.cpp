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

// tile helper functions
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git


#include "ztool_tiletools.h"



void glztiles::load(string const filename, glzTileType intype)
{
	img_filename=filename;

	glzReadTgaHead(&imghdr, img_filename);
	data = new unsigned char[imghdr.imageSize];
	glzLoadTga(&imghdr, img_filename, data);
	glzMaketex(&imghdr, data, glzTexFilter::NEAREST);
	tex = imghdr.m_id;

	width = imghdr.m_width;
	height = imghdr.m_height;

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imghdr.m_width, imghdr.m_height, imghdr.m_type, GL_UNSIGNED_BYTE, data);
	type = intype;
	tex_changed = false;
	data_changed = false;
}

void glztiles::update_texture(void)
{
	if (!tex_changed) return;
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imghdr.m_width, imghdr.m_height, imghdr.m_type, GL_UNSIGNED_BYTE, data);
	tex_changed = false;
}

void glztiles::save(void)
{
	if (data_changed) glzSaveTGA(img_filename, imghdr.m_width, imghdr.m_height, 0, glzTexCompression::COMPRESSED, imghdr.m_type, data);
}

char glztiles::get_pixel(int x, int y, int layer)
{
	return data[glz2dTo1dImageRemap(x, y, layer, 4, imghdr.m_width, imghdr.m_height, true)];
}

void glztiles::put_pixel(int x, int y, int layer, char value)
{
	data[glz2dTo1dImageRemap(x, y, layer, 4, imghdr.m_width, imghdr.m_height, true)] = value;
	tex_changed = true;
	data_changed = true;

}


void glztiles::paint_pixel(int x, int y, int sx, int sy, bool animate, bool flip, int layer)
{


	int d_o = 0;
	char dx, dy;

	if (type == glzTileType::DOUBLE_LAYER)
	{ 

	// get data


	if (layer == 2) d_o = 2;

	dx = get_pixel(x, y, d_o);
	dy = get_pixel(x, y, d_o+1);

	//dx = data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, imghdr.m_width, imghdr.m_height, true)];
	//dy = data[glz2dTo1dImageRemap(x, y, 1 + d_o, 4, imghdr.m_width, imghdr.m_height, true)];

	bool ani = false, ext = false;
	if (dx > 127) { dx -= 128; ani = true; }
	if (dy > 127) { dy -= 128; ext = true; }

	if ((sx == dx) && (sy == dy) && (animate == ani) && (flip == ext)) return; //no change so do nothing

	if (sx > 127) sx = 127;
	if (sy > 127) sy = 127;

	if (animate) sx += 128;
	if (flip) sy += 128;


	put_pixel(x, y, d_o,sx);
	put_pixel(x, y, d_o + 1,sy);

//	data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, imghdr.m_width, imghdr.m_height, true)] = sx;
//	data[glz2dTo1dImageRemap(x, y, 1 + d_o, 4, imghdr.m_width, imghdr.m_height, true)] = sy;



	}

	else
	{		
		if (layer == 1) d_o = 0;
		if (layer == 2) d_o = 1;
		if (layer == 3) d_o = 2;
		if (layer == 4) d_o = 3;

		//dx = data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, imghdr.m_width, imghdr.m_height, true)];
		//data[glz2dTo1dImageRemap(x, y, 0 + d_o, 4, imghdr.m_width, imghdr.m_height, true)] = sx;
		put_pixel(x, y, d_o, sx);
	}

	return;
}


void glztiles::put_extra_bit(int x, int y, bool bitdata, int layer)
{
	if (type != glzTileType::DOUBLE_LAYER) return; // only double layer tilemaps have extra bits

	char pxdata = get_pixel(x, y, layer);

	pxdata = pxdata & 127; // strip bit data
	if (bitdata) pxdata = pxdata | 128; // add bit data if needed


	put_pixel(x, y, layer, pxdata);


}



bool glztiles::getTilecolision(float x, float y, int layer, bool flip_y)
{
		
	//test if coords are inside tile area, if not return false.
	if (x < 00.0f) return false;
	if (x >= width) return false;

	if (y<0.0f) return false;
	if (y>height) return false;

	if (layer<1) return false;
	if (layer>4) return false;




	//split coords into integral and fractional parts.
	int const xi = (int)x, yi = (int)y;
	float const xf = x - (float)xi, yf = y - (float)yi;


	//use integral part to read the current tile data.
	int const d_o = layer-1;	
	char const td = data[glz2dTo1dImageRemap(xi, yi, 0 + d_o, 4, imghdr.m_width, imghdr.m_height, flip_y)];	
	

	//Use fractional part to determine of said coordinate is inside the colision area of the tile, return true if that is the case.

	switch (td)
	{
	case 0: //fully transparent
		return false;
		break;

	case 18:  //fully opaque
		return true;
		break;

	case 38:  // 45 degree slope with lower right opaque
		if (xf+yf>1.0)
			return true;
		break;

	case 39:  // 45 degree slope with lower left opaque
		if ((1.0-xf) + yf>1.0) return true;
		break;

	case 17:
	case 52:  // right half opaque vertical 
		if (xf >0.5)
			return true;
		break;

	case 19:
	case 53:  // left half opaque vertical 
		if (1.0-xf >0.5)
			return true;
		break;


	case 54:  // 45 degree slope with upper right opaque
		if ((1.0 - xf) + yf<1.0) return true;
		break;

	case 55:  // 45 degree slope with upper left opaque
		if (xf + yf<1.0) return true;
		break;

	case 70:  // 45 degree half slope with lower right opaque
		if (xf + yf>1.5)
			return true;
		break;

	case 71:  // 45 degree slope with lower left opaque
		if ((1.0 - xf) + yf>1.5) return true;
		break;

	case 86:  // 45 degree half slope with lower right opaque
		if ((1.0 - xf) + yf<0.5)
			return true;
		break;

	case 87:  // 45 degree slope with lower left opaque
		if (xf + yf<0.5) return true;
		break;

	case 2:
	case 103:  // right half opaque horizontal 
		if (yf >0.5)
			return true;
		break;

	case 34:
	case 119:  // left half opaque horizontal 
		if (1.0 - yf >0.5)
			return true;
		break;

	default:
	
		break;
	}



	return false;

}
 
glztiles::~glztiles()
{
	delete data;
}