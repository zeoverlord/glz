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
#include "ztool-tex.h"
#include <stdio.h>
#include <windows.h>													// Header File For The Windows Library
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library
#include <gl/glext.h>
using namespace std;

static bool isinited_tex;

unsigned int filter_max;
unsigned int filter_min;
unsigned int filter_wrap;
unsigned int filter_mip;
int filter_ansio;

static PFNGLGENERATEMIPMAPPROC glGenerateMipmap;


void ini_tex(void)
{
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");
	
	isinited_tex=true;
}

// no need to be concerned about this one except if you want to add more filtering modes, then just add a new define and do the filtering settings here
// later i might end up changing this to be a bit more thourough with both wrapping and filtering, but i will probably not need to break any code you write that uses this
void chooseFilter(glzTexFilter filter)
{
	filter_mip=0;
	filter_ansio=0;

	switch(filter)
	{
	case glzTexFilter::NONE:
		filter_max=GL_NEAREST;
		filter_min=GL_NEAREST;
		filter_wrap=GL_REPEAT;
		break;
		
	case glzTexFilter::NEAREST:
		filter_max=GL_NEAREST;
		filter_min=GL_NEAREST;
		filter_wrap=GL_REPEAT;
		break;
		
	case glzTexFilter::LINEAR:
		filter_max=GL_LINEAR;
		filter_min=GL_LINEAR;
		filter_wrap=GL_REPEAT;
		break;
		
	case glzTexFilter::BILINEAR:
		filter_max=GL_LINEAR;
		filter_min=GL_LINEAR_MIPMAP_NEAREST;
		filter_wrap=GL_REPEAT;
		filter_mip=1;
		break;
		
	case glzTexFilter::TRILINEAR:
		filter_max=GL_LINEAR;
		filter_min=GL_LINEAR_MIPMAP_LINEAR;
		filter_wrap=GL_REPEAT;
		filter_mip=1;
		break;

	case glzTexFilter::ANSIO_2:
		filter_max=GL_LINEAR;
		filter_min=GL_LINEAR_MIPMAP_LINEAR;
		filter_wrap=GL_REPEAT;
		filter_ansio=2;
		filter_mip=1;
		break;
			
	case glzTexFilter::ANSIO_4:
		filter_max=GL_LINEAR;
		filter_min=GL_LINEAR_MIPMAP_LINEAR;
		filter_wrap=GL_REPEAT;
		filter_ansio=4;
		filter_mip=1;
		break;
			
	case glzTexFilter::ANSIO_8:
		filter_max=GL_LINEAR;
		filter_min=GL_LINEAR_MIPMAP_LINEAR;
		filter_wrap=GL_REPEAT;
		filter_ansio=8;
		filter_mip=1;
		break;
			
	case glzTexFilter::ANSIO_16:
		filter_max=GL_LINEAR;
		filter_min=GL_LINEAR_MIPMAP_LINEAR;
		filter_wrap=GL_REPEAT;
		filter_ansio=16;
		filter_mip=1;
		break;
			
	case glzTexFilter::ANSIO_MAX:
		filter_max=GL_LINEAR;
		filter_min=GL_LINEAR_MIPMAP_LINEAR;
		filter_wrap=GL_REPEAT;
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&filter_ansio);
		filter_mip=1;
		break;

	}
}




unsigned int glzGetDefaultTextturehandle(void)
{
	if (!isinited_tex) ini_tex();


	GLubyte checkImage[32][32][4];
	unsigned int id;

	int i, j, c;

	for (i = 0; i < 32; i++) {
		for (j = 0; j < 32; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}


	chooseFilter(glzTexFilter::LINEAR);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, filter_wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, filter_wrap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 2,2, GL_RGBA, GL_UNSIGNED_BYTE, data);
	return id;
}


// this function will only read the head of a tga file and report back things like size dimetions and color channels
// it is mainly used to get the size of the data buffer you need to load this image, it is in img.imageSize for ease of use later on
void glzReadTgaHead(img_head *img, string filename)
	{


	//initialize or reset image info
	img->m_width=0;
	img->m_height=0;
	img->m_bpp=0;
	img->m_id=0;
	img->m_type=0;
	img->imageSize=0;
	img->origin = glzOrigin::BOTTOM_LEFT;



	ifstream File(filename, ios::in | ios::binary);
	unsigned char header[20];
	
	if (!File)
	{
		return;
	}
	
	//read all 18 bytes of the header
	File.read (reinterpret_cast<char *>(header), sizeof (char)*18);
	
	//should be image type 2 (color) or type 10 (rle compressed color)
	if (header[2] != 2 && header[2] != 10)
	{
		File.close();
		return;
	}

	//if there is an image ID section then skip over it
	if (header[0])
	{
		File.seekg(header[0],ios::cur);
	}

	// get the size and bitdepth from the header
	img->m_width = header[13] * 256 + header[12];
	img->m_height = header[15] * 256 + header[14];
	img->m_bpp = header[16] / 8;

	if (header[17] & 16) img->origin = glzOrigin::BOTTOM_RIGHT;
	if (header[17] & 32) img->origin = glzOrigin::TOP_LEFT;
	if ((header[17] & 16) && (header[17] & 32)) img->origin = glzOrigin::TOP_RIGHT;

	if (img->m_bpp != 3 && img->m_bpp != 4) 
	{
		File.close();
		return;
	}
		
	img->imageSize = img->m_width * img->m_height * img->m_bpp;

	File.close();
	return;


	}


// this function on the oter hand will load the entire tga into the data buffer

void glzLoadTga(img_head *img, string filename, unsigned char *data)
{		

	ifstream File(filename, ios::in | ios::binary);
	unsigned char header[20];
	
	if (!File)
	{
		return;
	}
	
	//read all 18 bytes of the hrader
	File.read (reinterpret_cast<char *>(header), sizeof (char)*18);
	
	//should be image type 2 (color) or type 10 (rle compressed color)
	if (header[2] != 2 && header[2] != 10)
	{
		File.close();
		return;
	}

	//if there is an image ID section then skip over it
	if (header[0])
	{
		File.seekg(header[0],ios::cur);
	}
	
	//read the uncompressed image data if type 2
	if (header[2] == 2) 
	{
		File.read(reinterpret_cast<char *>(data), sizeof (char)*img->imageSize);
	}

	long ctpixel=0,ctloop=0;

	//read the compressed image data if type 10
	if (header[2] == 10)  
	{
		// stores the rle header and the temp color data
		unsigned char rle;
		unsigned char color[4];
		
		while (ctpixel<img->imageSize)
		{
			// reads the the RLE header
			File.read(reinterpret_cast<char *>(&rle), 1);

			// if the rle header is below 128 it means that what folows is just raw data with rle+1 pixels
			if (rle<128)
			{
				File.read(reinterpret_cast<char *>(&data[ctpixel]), img->m_bpp*(rle+1));
				ctpixel+=img->m_bpp*(rle+1);
			}

			// if the rle header is equal or above 128 it means that we have a string of rle-127 pixels 
			// that use the folowing pixels color
			else 
			{
				// read what color we should use
				File.read(reinterpret_cast<char *>(&color[0]), img->m_bpp);

				// insert the color stored in tmp into the folowing rle-127 pixels
				ctloop=0;
				while(ctloop<(rle-127))
				{						
					data[ctpixel]=color[0];
					data[ctpixel+1]=color[1];
					data[ctpixel+2]=color[2];
					if (img->m_bpp==4)
					{
						data[ctpixel+3]=color[3];
					}

					ctpixel+=img->m_bpp;
					ctloop++;
				}
			}
		}
	}
		
	ctpixel=0;

	//Because TGA file store their colors in BGRA format we need to swap the red and blue color components
	while (ctpixel<img->imageSize)
	{
		data[ctpixel] ^= data[ctpixel+2] ^=	data[ctpixel] ^= data[ctpixel+2];
		ctpixel+= img->m_bpp;
	}
	//close file
	File.close();
	//selecting the image type
	img->m_type=GL_RGB;
	if(img->m_bpp==4) img->m_type=GL_RGBA;
	//Generating textures for OpenGL

	return;
}






// unless you either made your own texture or changed the texture data somehow you should really let glzLoadTexture do the job
void glzMaketex(img_head *img, unsigned char data[], glzTexFilter filter)
{
if(!isinited_tex) ini_tex();


	unsigned int texID;
	chooseFilter(filter);

	glGenTextures(1,&texID);
	glBindTexture(GL_TEXTURE_2D,texID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,filter_max);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,filter_min);		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,filter_wrap);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,filter_wrap);

	glTexImage2D(GL_TEXTURE_2D, 0, img->m_type, img->m_width, img->m_height, 0, img->m_type, GL_UNSIGNED_BYTE, data);

	if (filter_mip) glGenerateMipmap(GL_TEXTURE_2D);

	img->m_id=texID;

	return;

}







// this is pretty self explanatory, supply filename and filter type (GLZ_FILTER_ANSIO_MAX is reccomended) and it spits out a texture handle
unsigned int glzLoadTexture(string filename, glzTexFilter filter)
{
	if(!isinited_tex) ini_tex();

	img_head img;
	unsigned char *data;

	glzReadTgaHead(&img, filename);

	data = new unsigned char[img.imageSize];
	glzLoadTga(&img, filename, data);
	glzMaketex(&img, data, filter);

	delete[] data;
	data=NULL;


return img.m_id;

}


// most oftenly the use will be "glzScreenShot("myfile.tga",width,height,GLZ_COMPRESSED)" but i added a few fancy things if you like to fiddle with it
// glzScreenShotADV adds x and y offsets and lossy compression, the lossy compression basically just lowers the sensitivity on the rle segments with +- the lossy argument
// it's especially useful if you have lots of low amplidude high frequency noice and few gradients, but it can look uggly if set to high or if it is animated, below 5 should be ok in most cases

void glzScreenShot(string filename, int x, int y, glzTexCompression type)
{
	glzScreenShotADV(filename,0,0,x,y,type);
}

void glzScreenShotADV(string filename, int xoffset, int yoffset, int x, int y, glzTexCompression type)
{
if(!isinited_tex) ini_tex();


// get the image data
long const imageSize = x * y * 3;									
unsigned char *data = new unsigned char[imageSize];
unsigned char cdata[4];  //new space for an yline

glReadPixels(xoffset,yoffset,x,y, GL_BGR,GL_UNSIGNED_BYTE,data);


// split x and y sizes into bytes
int const xa = x % 256;
int const xb = (x - xa) / 256;

int const ya = y % 256;
int const yb = (y - ya) / 256;

unsigned char const uncompressed_header[18] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, (char)xa, (char)xb, (char)ya, (char)yb, 24, 0 };
unsigned char const compressed_header[18] = { 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, (char)xa, (char)xb, (char)ya, (char)yb, 24, 0 };

if (type == glzTexCompression::UNCOMPRESSED)
{
	
	// write header and uncompressed data to file
fstream File(filename, ios::out | ios::binary);
File.write(reinterpret_cast<char const *>(uncompressed_header), sizeof (char)* 18);
File.write (reinterpret_cast<char const *>(data), sizeof (char)*imageSize);
File.close();

}
else if (type == glzTexCompression::COMPRESSED)
{
	// write header to file
fstream File(filename, ios::out | ios::binary);
File.write(reinterpret_cast<char const *>(compressed_header), sizeof (char)* 18);


int yline=0,xp=0;
int dc=0,i=0,rlel=0,rlec=0,dpos=0,cdpos=0,same=0;
char curPixel[3]={0,0,0},rlelength=0;
// run trough all lines
while (yline<y)
{
	
	// scan for  rle segments, at least 3 pixels in a row
	i=0;	
	rlelength=0;
	while (i<x)
	{
		rlelength=0;

		//is the segment compressed
		
		rlec=0;
		if(i<=x)  // it's not if it's at the end of the line
		{
		rlec=1;
		if(data[dpos+(i*3)+0]!=data[dpos+(i*3)+3]) rlec=0;
		if(data[dpos+(i*3)+1]!=data[dpos+(i*3)+4]) rlec=0;
		if(data[dpos+(i*3)+2]!=data[dpos+(i*3)+5]) rlec=0;
		}

		// of rlec is still 1 then yes


		if(rlec) // compressed segment
		{
			xp++;
			//scan till the data changes or rlelength=127 or i=x
			rlelength=0;
			same=1;
			while (same)
			{
			rlelength++;
						
			// non destructive encoding
		    if(data[dpos+(i*3)+0+3]!=data[dpos+((i+rlelength+1)*3)+0]) same=0;
			if(data[dpos+(i*3)+1+3]!=data[dpos+((i+rlelength+1)*3)+1]) same=0;
			if(data[dpos+(i*3)+2+3]!=data[dpos+((i+rlelength+1)*3)+2]) same=0;
						

			if(i+rlelength>=x) same=0;
			if(rlelength>=127) same=0;
			if(i==x)  same=0;
			}

			// write the rle segment to the data storage
			cdata[0]=rlelength+127;
			File.write (reinterpret_cast<char *>(cdata), sizeof(char));
			File.write (reinterpret_cast<char *>(data)+dpos+(i*3), sizeof(char)*3);

			i+=rlelength;
			

		}

		else // uncompressed segment
		{
			//get the length of this segment by scaning for the next compressed one
			rlelength=0;
			same=1;
			while (same)
			{
			rlelength++;

			if(i==x)  same=0;
			if(i+rlelength>=x)  same=0; // it's not if it's at the end of the line
			if(data[dpos+((i+rlelength-1)*3)+0]==data[dpos+((i+rlelength-1)*3)+3]) same=0;
			if(data[dpos+((i+rlelength-1)*3)+1]==data[dpos+((i+rlelength-1)*3)+4]) same=0;
			if(data[dpos+((i+rlelength-1)*3)+2]==data[dpos+((i+rlelength-1)*3)+5]) same=0;

			// if rlec is still 1 then the next pixel is a new segment
			if(rlelength>=127) same=0;

			}
			

			//now write the data
		
			cdata[0]=rlelength-1;
			File.write (reinterpret_cast<char *>(cdata), sizeof(char));	
			File.write (reinterpret_cast<char *>(data)+dpos+(i*3), sizeof(char)*3*rlelength);
			i+=rlelength;

		}






	}

yline++;
dpos=yline*(x*3);
}

// close the file
File.close();


 }
delete[] data;
data=NULL;
}



void glzSaveTGA(string filename, int x, int y, glzTexCompression type, unsigned int tex_type, unsigned char *in_data)
{
	if (!isinited_tex) ini_tex();


	bool has_alpha = false;
	if (tex_type == GL_RGBA) has_alpha = true;
	if (tex_type == GL_RGB) has_alpha = false;

	// get the image data
	long imageSize = x * y * 3;
	if (has_alpha) imageSize = x * y * 4;
	unsigned char *data = new unsigned char[imageSize];
	unsigned char cdata[4];  //new space for an yline

	int i = 0;
	
	unsigned char dt = 0;


	//Because TGA file store their colors in BGRA format we need to swap the red and blue color components
	while (i<imageSize)
	{
		data[i + 0] = in_data[i + 2];
		data[i + 1] = in_data[i + 1];
		data[i + 2] = in_data[i + 0];
		if (has_alpha) data[i + 3] = in_data[i + 3];

		if (has_alpha) i += 4;
		else i += 3;
	}


	// split x and y sizes into bytes
	int xa = x % 256;
	int xb = (x - xa) / 256;

	int ya = y % 256;
	int yb = (y - ya) / 256;

	int bpp = 24;
	if (has_alpha) bpp = 32;

	 char const uncompressed_header[18] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, (char)xa, (char)xb, (char)ya, (char)yb, bpp, 0 };
	 char const compressed_header[18] = { 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, (char)xa, (char)xb, (char)ya, (char)yb, bpp, 0 };

	

	if (type == glzTexCompression::UNCOMPRESSED)
	{
		// write header and uncompressed data to file
		fstream File(filename, ios::out | ios::binary);
		File.write(reinterpret_cast<char const *>(uncompressed_header), sizeof (char)* 18);
		File.write(reinterpret_cast<char *>(data), sizeof (char)*imageSize);
		File.close();

	}
	else if (type == glzTexCompression::COMPRESSED)
	{
		// write header to file
		fstream File(filename, ios::out | ios::binary);
		File.write(compressed_header, sizeof (char)* 18);
		

		int yline = 0, xp = 0;
		int dc = 0, i = 0, rlel = 0, rlec = 0, dpos = 0, cdpos = 0, same = 0;
		char curPixel[3] = { 0, 0, 0 }, rlelength = 0;
		// run trough all lines
		while (yline<y)
		{

			// scan for  rle segments, at least 3 pixels in a row
			i = 0;
			rlelength = 0;
			while (i<x)
			{
				rlelength = 0;

				//is the segment compressed

				rlec = 0;
				if (i <= x)  // it's not if it's at the end of the line
				{

					// if the next pixel is the same use the rle compressed segment, otherwise use an uncompressed segment
					rlec = 1;
					if (!has_alpha)
					{						
						if (data[dpos + (i * 3) + 0] != data[dpos + (i * 3) + 3]) rlec = 0;
						if (data[dpos + (i * 3) + 1] != data[dpos + (i * 3) + 4]) rlec = 0;
						if (data[dpos + (i * 3) + 2] != data[dpos + (i * 3) + 5]) rlec = 0;						
					}
					else
					{				
						if (data[dpos + (i * 4) + 0] != data[dpos + (i * 4) + 4]) rlec = 0;
						if (data[dpos + (i * 4) + 1] != data[dpos + (i * 4) + 5]) rlec = 0;
						if (data[dpos + (i * 4) + 2] != data[dpos + (i * 4) + 6]) rlec = 0;
						if (data[dpos + (i * 4) + 3] != data[dpos + (i * 4) + 7]) rlec = 0;
					}

				}

				// of rlec is still 1 then yes


				if (rlec) // compressed segment
				{
					xp++;
					//scan till the data changes or rlelength=127 or i=x
					rlelength = 0;
					same = 1;
					while (same)
					{
						rlelength++;

						// non destructive encoding
							if (!has_alpha)
							{

								if (data[dpos + (i * 3) + 0 + 3] != data[dpos + ((i + rlelength + 1) * 3) + 0]) same = 0;
								if (data[dpos + (i * 3) + 1 + 3] != data[dpos + ((i + rlelength + 1) * 3) + 1]) same = 0;
								if (data[dpos + (i * 3) + 2 + 3] != data[dpos + ((i + rlelength + 1) * 3) + 2]) same = 0;
							}
							else
							{
								if (data[dpos + (i * 4) + 0 + 4] != data[dpos + ((i + rlelength + 1) * 4) + 0]) same = 0;
								if (data[dpos + (i * 4) + 1 + 4] != data[dpos + ((i + rlelength + 1) * 4) + 1]) same = 0;
								if (data[dpos + (i * 4) + 2 + 4] != data[dpos + ((i + rlelength + 1) * 4) + 2]) same = 0;
								if (data[dpos + (i * 4) + 3 + 4] != data[dpos + ((i + rlelength + 1) * 4) + 3]) same = 0;

							}				

						if (i + rlelength >= x) same = 0;
						if (rlelength >= 127) same = 0;
						if (i == x)  same = 0;
					}

					// write the rle segment to the data storage
					cdata[0] = rlelength + 127;
					File.write(reinterpret_cast<char *>(cdata), sizeof(char));
					if (!has_alpha) File.write(reinterpret_cast<char *>(data)+dpos + (i * 3), sizeof(char)* 3);
					else File.write(reinterpret_cast<char *>(data)+dpos + (i * 4), sizeof(char)* 4);

					i += rlelength;


				}

				else // uncompressed segment
				{
					//get the length of this segment by scaning for the next compressed one
					rlelength = 0;
					same = 1;
					while (same)
					{
						rlelength++;

						if (i == x)  same = 0;
						if (i + rlelength >= x)  same = 0; // it's not if it's at the end of the line
						if (!has_alpha)
						{
							if (data[dpos + ((i + rlelength - 1) * 3) + 0] == data[dpos + ((i + rlelength - 1) * 3) + 3]) same = 0;
							if (data[dpos + ((i + rlelength - 1) * 3) + 1] == data[dpos + ((i + rlelength - 1) * 3) + 4]) same = 0;
							if (data[dpos + ((i + rlelength - 1) * 3) + 2] == data[dpos + ((i + rlelength - 1) * 3) + 5]) same = 0;
						}
						else{

							if (data[dpos + ((i + rlelength - 1) * 4) + 0] == data[dpos + ((i + rlelength - 1) * 4) + 4]) same = 0;
							if (data[dpos + ((i + rlelength - 1) * 4) + 1] == data[dpos + ((i + rlelength - 1) * 4) + 5]) same = 0;
							if (data[dpos + ((i + rlelength - 1) * 4) + 2] == data[dpos + ((i + rlelength - 1) * 4) + 6]) same = 0;
							if (data[dpos + ((i + rlelength - 1) * 4) + 3] == data[dpos + ((i + rlelength - 1) * 4) + 7]) same = 0;

						}
						// if rlec is still 1 then the next pixel is a new segment
						if (rlelength >= 127) same = 0;

					}


					//now write the data

					cdata[0] = rlelength - 1;
					File.write(reinterpret_cast<char *>(cdata), sizeof(char));
					if (!has_alpha) File.write(reinterpret_cast<char *>(data)+dpos + (i * 3), sizeof(char)* 3 * rlelength);
					else File.write(reinterpret_cast<char *>(data)+dpos + (i * 4), sizeof(char)* 4 * rlelength);
					i += rlelength;

				}






			}

			yline++;
			if (!has_alpha) dpos = yline*(x * 3);
			else  dpos = yline*(x * 4);
		}

		// close the file
		File.close();
		delete(data);


	}

}


// things i like to add in the future

// openEXR support, both loading and saving
// More explicit saving, much like how the texture loading is split in many functions i would like to do the same with screenshots
// various texture loading styles, like texture arrays and cubemaps
// tools that will help with various forms of megatexturing
// a way to kill textures