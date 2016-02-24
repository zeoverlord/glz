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

// glz toolkit - this file contains may usefull little functions and many more useless ones
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git


#include <math.h>
#include <fstream>
#include <string.h>
#include "glz.h"
#include <stdio.h>
#include <random>
#include <windows.h>													// Header File For The Windows Library
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library
#include <gl/glext.h>
using namespace std;


static bool isinited_glz;
static std::mt19937 mt(1729);


// currently this thing is entirely useless since this file doesn't even bother to mess with any APIs, it's almost entirely just math, but it might change someday which is why it's here
void ini_glz(void)
{
	
	isinited_glz=true;

}


// usefull stuff

float glzRandf(void)  //produces a value between 1 and 0
{
	std::uniform_real_distribution<float> dist(0.0,1.0);
	return dist(mt);

	// you wouldn't belive how often doing this the wrong way have given me strange results
}

float glzRandf(glzDistribution D)  //produces a value between 1 and 0
{
	std::uniform_real_distribution<float> dist_u(0.0, 1.0);
	std::normal_distribution<float> dist_n(0.0, 1.0);
	std::gamma_distribution<float> dist_g(1.0, 1.0);
	std::exponential_distribution<float> dist_e(1.0);

	switch (D)
	{
	case glzDistribution::UNIFORM:
		return abs(dist_u(mt));
		break;
	case glzDistribution::NORMAL:
		return dist_n(mt);
		break;
	case glzDistribution::GAMMA:
		return dist_g(mt);
		break;
	case glzDistribution::EXPONENTIAL:
		return dist_e(mt);
		break;
	default:
		return abs(dist_u(mt));
		break;
	}

}

double glzRandfs(void)  //produces a value between 1 and -1
{
	std::uniform_real_distribution<double> dist(-1.0, 1.0);
	return dist(mt);
}


double glzRandfs(glzDistribution D)  //produces a value between 1 and -1
{
	std::uniform_real_distribution<double> dist_u(-1.0, 1.0);
	std::normal_distribution<double> dist_n(0.0, 1.0);
	std::gamma_distribution<double> dist_g(1.0, 1.0);
	std::exponential_distribution<double> dist_e(1.0);
   std::uniform_int_distribution<int> dist_i(0, 1);

int sign = 1;
if (dist_i(mt)) sign = -1;

	switch (D)
	{ 
		case glzDistribution::UNIFORM:			
			return dist_u(mt);
			break;
		case glzDistribution::NORMAL:			
			return dist_n(mt)*sign;
			break;
		case glzDistribution::GAMMA:			
			return dist_g(mt)*sign;
			break;
		case glzDistribution::EXPONENTIAL:			
			return dist_e(mt)*sign;
			break;
		default:
			return dist_u(mt);
			break;
}
	
}


vec3 glzRandfs_vec3(void)  //produces a value between 1 and -1
{	
	return vec3(glzRandfs(), glzRandfs(), glzRandfs());
}

vec3 glzRandfs_vec3(glzDistribution D)  //produces a value between 1 and -1
{
	return vec3(glzRandfs(D), glzRandfs(D), glzRandfs(D));
}

vert3 glzRandfs_vert3(void)  //produces a value between 1 and -1
{
	return vert3(glzRandfs(), glzRandfs(), glzRandfs());
}

vert3 glzRandfs_vert3(glzDistribution D)  //produces a value between 1 and -1
{
	return vert3(glzRandfs(D), glzRandfs(D), glzRandfs(D));
}



/*
vec3 glzRandfs_vec3(glzDistribution D)  //produces a value between 1 and -1
{
	std::uniform_real_distribution<float> dist_u(-1.0, 1.0);
	std::normal_distribution<float> dist_n(0.0, 1.0);
	std::gamma_distribution<float> dist_g(1.0, 1.0);
	std::exponential_distribution<float> dist_e(1.0);
	std::uniform_int_distribution<int> dist_i(0, 1);

	int sign = 1;
	if (dist_i(mt)) sign = -1;

	switch (D)
	{
	case glzDistribution::UNIFORM:
		return vec3(dist_u(mt), dist_u(mt), dist_u(mt));
		break;
	case glzDistribution::NORMAL:
		return vec3(dist_n(mt)*sign, dist_n(mt)*sign, dist_n(mt)*sign);
		break;
	case glzDistribution::GAMMA:
		return vec3(dist_g(mt)*sign, dist_g(mt)*sign, dist_g(mt)*sign);
		break;
	case glzDistribution::EXPONENTIAL:
		return vec3(dist_e(mt)*sign, dist_e(mt)*sign, dist_e(mt)*sign);
		break;
	default:
		return vec3(dist_u(mt), dist_u(mt), dist_u(mt));
		break;
	}

}*/

// static noise functions, works but is slower than random because of overhead, still they are pretty good for generation of pseudorandom data that needs to be the same every time


float glzMersienneNoise(float seed)
{
	std::mt19937 mtx(seed);
	std::uniform_real_distribution<float> dist_u(-1.0, 1.0);
	return dist_u(mtx);

}
float glzMersienneNoise(float seed, float x)
{
	std::mt19937 mtx(seed + x);
	std::uniform_real_distribution<float> dist_u(-1.0, 1.0);
	return dist_u(mtx);
}

float glzMersienneNoise(float seed, float x, float y)
{
	std::mt19937 mtx(seed+x);
	std::uniform_real_distribution<float> dist_x(0.0, 374321.0);
	std::uniform_real_distribution<float> dist_u(-1.0, 1.0);
	std::mt19937 mty(y * dist_x(mtx));
	return dist_u(mty);

}

float glzMersienneNoise(float seed, float x, float y, float z)
{
	std::mt19937 mtx(seed + x);
	std::uniform_real_distribution<float> dist_x(0.0, 374321.0);
	std::uniform_real_distribution<float> dist_y(0.0, 1729.0);
	std::uniform_real_distribution<float> dist_u(-1.0, 1.0);
	std::mt19937 mty(y * dist_x(mtx));
	std::mt19937 mtz(z * dist_y(mty));
	return dist_u(mtz); 
}






// misc math


float glzModF(float f, float m)  //float modulo function
{
	if (m==0.0f) return f;

	if (f<0)  // negative numbers
		while(f<m) f+=abs(m);
	else      // possitive numbers
		while(f>m) f-=abs(m);
	return f;
}

int glzModFStep(float f, float m)  //special float modulo function that returns the number of times m fits in f
{
	return (int)(f/m);
}

int glzTimeCounter(float *t, float m)  //timing function
{
	int r=0;
	while(t[0]>m) {t[0]-=abs(m); r++;}
	return r; // retruns the number of m's that exist in f while decreasing f with r*m
}

float quantize(float f, float s)
{
	float r = 0.0f;
	if (s <= 0.0f) return f;

	if (f<0)  // negative numbers
	while (f<=r-s) r -= s;
	else      // possitive numbers
	while (f>=r+s) r += s;
	return r;
}

float glzIntegral(float f)
{
	float r = 0.0f;
	if (1.0 <= 0.0f) return f;

	if (f<0)  // negative numbers
	while (f <= r - 1.0) r -= 1.0;
	else      // possitive numbers
	while (f >= r + 1.0) r += 1.0;
	return r;
}

// some general math functions


float glzDotproduct(float a[3], float b[3])
{
	return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

double glzDotproduct(double a[3], double b[3])
{
	return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

double glzDotproduct(vec3 a, vec3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}


float glzMagnitude(float a[3])
{

	return (float)sqrt((a[0]*a[0]) + (a[1]*a[1]) + (a[2]*a[2]));
}


double glzMagnitude(double a[3])
{

	return sqrt((a[0]*a[0]) + (a[1]*a[1]) + (a[2]*a[2]));
}

void glzNormalize(float *a, float le)
{

	float l=le/(float)sqrt((a[0]*a[0]) + (a[1]*a[1]) + (a[2]*a[2]));

	a[0]*=l;
	a[1]*=l;
	a[2]*=l;
	return;
}

void glzNormalize(double *a, double le)
{

	double l=le/(double)sqrt((a[0]*a[0]) + (a[1]*a[1]) + (a[2]*a[2]));

	a[0]*=l;
	a[1]*=l;
	a[2]*=l;
	return;
}

void glzCrossproduct(float a[3], float b[3], float *r)
	{
		r[0]=b[1]*a[2]-a[1]*b[2];
		r[1]=b[2]*a[0]-a[2]*b[0];
		r[2]=b[0]*a[1]-a[0]*b[1];
		return;
	}

void glzCrossproduct(double a[3], double b[3], double *r)
	{
		r[0]=b[1]*a[2]-a[1]*b[2];
		r[1]=b[2]*a[0]-a[2]*b[0];
		r[2]=b[0]*a[1]-a[0]*b[1];
		return;
	}


vec3 glzCrossproduct(vec3 a, vec3 b)
{
	vec3 r;
	r.x = b.y * a.z - a.y * b.z;
	r.y = b.z * a.x - a.z * b.x;
	r.z = b.x * a.y - a.x * b.y;
	return r;
}


//image code


// this will take xy coordinates and convert them into 1d coordinates suitable for adressing images
// use GLZ_INVERT as the type if the image data is stored bottom first but the y coordinate is top first

unsigned int glz2dTo1dImageRemap(unsigned int x, unsigned int y, unsigned int col, unsigned int step, unsigned int width, unsigned int height, bool invert)
{
	if (invert) return (((((height-1)-y)*width)+x)*step)+col;
	else return ((((width*y)*width)+x)*step)+col;

}

// gets the inerpolated result of a 2d coordinate from a 1D array, it's more or less what openGL does
// this uses the function above so the type has the same function
float glzImageReadBilinear(float x, float y, unsigned int col, unsigned int step, int width, int height, unsigned int mirror, unsigned char *data, bool invert)
{
	int ix=(int)x,iy=(int)y,ix2=(int)x+1,iy2=(int)y+1,mx,my;
	float xfract=x-(float)ix;
	float yfract=y-(float)iy;

	float value[2];



	// mirror transform
	if (mirror & GLZ_MIRROR_X) 
		{
			mx=(ix/width)%2;
			while (ix<0) ix+=width;
			while (ix>width-1) ix-=width;
			if (mx) ix=width-ix;


			mx=(ix2/width)%2;
			while (ix2<0) ix2+=width;
			while (ix2>width) ix2-=width;
			if (mx) ix2=width-ix2;
		}

	if (mirror & GLZ_MIRROR_Y)
		{
			my=(iy/height)%2;
			while (iy<0) iy+=height;
			while (iy>height-1) iy-=height;
			if (my) iy=height-iy;

			my=(iy2/height)%2;
			while (iy2<0) iy2+=height;
			while (iy2>height) iy2-=height;
			if (my) iy2=height-iy2;
		}

	if (mirror & GLZ_REPEAT_X) 
		{
			while (ix<0) ix+=width;
			while (ix>width-1) ix-=width;

			while (ix2<0) ix2+=width;
			while (ix2>width) ix2-=width;
		}

	if (mirror & GLZ_REPEAT_Y)
		{
			while (iy<0) iy+=height;
			while (iy>height-1) iy-=height;

			while (iy2<0) iy2+=height;
			while (iy2>height) iy2-=height;
		}

	if (mirror & GLZ_CLAMP_X)		
		{
			if (ix<0) ix=0;
			if (ix>height-1) ix=height-1;

			if (ix2<1) ix2=1;
			if (ix2>height) ix2=height;
		}

	if (mirror & GLZ_CLAMP_Y)
		{
			if (iy<0) iy=0;
			if (iy>height-1) iy=height-1;

			if (iy2<1) iy2=1;
			if (iy2>height) iy2=height;
		}


	value[0]=0.0f;
	value[1]=0.0f;
	float r=0;


	value[0] = (float)data[glz2dTo1dImageRemap(ix, iy, col, step, width, height, invert)] * (1 - xfract) / 255;
	value[0] += (float)data[glz2dTo1dImageRemap(ix2, iy, col, step, width, height, invert)] * (xfract) / 255;
	
	value[1] = (float)data[glz2dTo1dImageRemap(ix, iy2, col, step, width, height, invert)] * (1 - xfract) / 255;
	value[1] += (float)data[glz2dTo1dImageRemap(ix2, iy2, col, step, width, height, invert)] * (xfract) / 255;


	r=(value[1]*yfract)+(value[0]*(1-yfract));

	return r;

}

// this will take a RGB color and convert it to pretty much any other color space
// likewise this will also take HSL HSV HSI and HCY (hue croma luma) and convert it back to rgb, that is if all things work properly
// so far any thesting has worked out as it should
// btw this is probably the only code in there i don't fully understand

float glzColorConvert(float a, float b, float c, glzColorType intype, glzColorChannels outtype)
{



	if ((intype == glzColorType::RGB_8) || (intype == glzColorType::RGB_32F))
	{
		if (intype == glzColorType::RGB_8) { a /= 255; b /= 255; c /= 255; }

		char max='a',min='a';
		float maxval=a,minval=a;
			
		if (maxval<b) {maxval=b; max='b';}
		if (maxval<c) {maxval=c; max='c';}

		if (minval>b) {minval=b; min='b';}
		if (minval>c) {minval=c; min='c';}


		float d = maxval - minval,alpha=0.5f*(2*a-b-c),beta=(sqrt(3.0f)/2)*(b-c);
		float h=0.0f,l=0.5f*(maxval+minval), i=(1.0f/3.0f)*(a+b+c), y=(0.3f*a)+(0.59f*b)+(0.11f*c);

		switch(outtype)
		{
		case glzColorChannels::RED:
			return a;
			break;
		case glzColorChannels::GREEN:
			return b;
			break;
		case glzColorChannels::BLUE:
			return c;
			break;
		case glzColorChannels::HUE:
			if(max == min) return 0.0f;

			switch(max)
			{
				case 'a': h=((b - c)/(d))/6; break;
				case 'b': h=(2 + (c - a)/(d))/6; break;
				case 'c': h=(4 + (a - b)/(d))/6; break;
			}	
			
			if(h < 0)
			h += 1.0f;

			return h;

			break;

		case glzColorChannels::HUE2:
			if(max == min) return 0.0f;
			h=(float)atan2(beta,alpha)*(float)PI_OVER_180_REVERSE;
			if(h < 0)
			h += 360.0f;
			return h/360.f;
			break;

		case glzColorChannels::CROMA:
			return d;
			break;
		case glzColorChannels::CROMA2:

			return sqrt((alpha*alpha)+(beta*beta));
			break;

		case glzColorChannels::VALUE:
			return maxval;
			break;

		case glzColorChannels::LIGHTNESS:
			return l;
			break;

		case glzColorChannels::INTENSITY:
			return i;
			break;

		case glzColorChannels::LUMA:
			return y;
			break;

		case glzColorChannels::SHSV:
			if(max == min) return 0.0f;
			return d/maxval;
			break;

		case glzColorChannels::SHSL:
			if(max == min) return 0.0f;			

			if(l>0.5)	return d/(2-(2*l));
			else		return d/(2*l);
			break;

		case glzColorChannels::SHSI:
			if(max == min) return 0.0f;
			return 1-(minval/i);
			break;

		default:
			return 0.0f;  // if all else fails
			break;

		}




		return 0.0f;  // if all else fails
	}



	if ((intype == glzColorType::HSL_8) || (intype == glzColorType::HSL_32F))
	{
		if (intype == glzColorType::HSL_8) { a /= 255; b /= 255; c /= 255; }
		
		float  h2=a*6.0f,rgb=0.0f;
		float d=0.0f,x=0.0f;		

		d = b*(1 - abs(2*c - 1));
		float minval=c-(0.5f*d);
		x = d*(1 - abs(((int)(h2)%2 + (h2) - (int)(h2)) - 1));


		//x=2*c - d;
		
		

		switch(outtype)
		{

		case glzColorChannels::RED:
		
	
			if (h2>=0) rgb=d;
			if (h2>1) rgb=x;
			if (h2>2) rgb=0;
			if (h2>3) rgb=0;
			if (h2>4) rgb=x;
			if (h2>5) rgb=c;
			if (h2>6) rgb=0;

			if (b==0) rgb=1;
			
			return rgb+minval;
			break;

		case glzColorChannels::GREEN:
	
			if (h2>=0) rgb=x;
			if (h2>1) rgb=d;
			if (h2>2) rgb=d;
			if (h2>3) rgb=x;
			if (h2>4) rgb=0;
			if (h2>5) rgb=0;
			if (h2>6) rgb=0;

			if (b==0) rgb=1;
			
			return rgb+minval;
			break;

		case glzColorChannels::BLUE:
	
			if (h2>=0) rgb=0;
			if (h2>1) rgb=0;
			if (h2>2) rgb=x;
			if (h2>3) rgb=d;
			if (h2>4) rgb=d;
			if (h2>5) rgb=x;
			if (h2>6) rgb=0;

			if (b==0) rgb=1;
			
			return rgb+minval;
			break;

		default:
			return 0.0f;  // if all else fails
			break;

		}

	}
			
		
	if ((intype == glzColorType::HSV_8) || (intype == glzColorType::HSV_32F))
	{
		if (intype == glzColorType::HSV_8) { a /= 255; b /= 255; c /= 255; }


		float  h2=a*6.0f,rgb=0.0f;
		float d=b*c,x=0.0f;		
		float minval=c-d;
		x = d*(1 - abs(((int)(h2)%2 + (h2) - (int)(h2)) - 1));


		switch(outtype)
		{

		case glzColorChannels::RED:
	
			if (h2>=0) rgb=d;
			if (h2>1) rgb=x;
			if (h2>2) rgb=0;
			if (h2>3) rgb=0;
			if (h2>4) rgb=x;
			if (h2>5) rgb=c;
			if (h2>6) rgb=0;

			if (b==0) rgb=1;

			return rgb+minval;
			break;

		case glzColorChannels::GREEN:
	
			if (h2>=0) rgb=x;
			if (h2>1) rgb=d;
			if (h2>2) rgb=d;
			if (h2>3) rgb=x;
			if (h2>4) rgb=0;
			if (h2>5) rgb=0;
			if (h2>6) rgb=0;

			if (b==0) rgb=1;
			
			return rgb+minval;
			break;

		case glzColorChannels::BLUE:
	
			if (h2>=0) rgb=0;
			if (h2>1) rgb=0;
			if (h2>2) rgb=x;
			if (h2>3) rgb=d;
			if (h2>4) rgb=d;
			if (h2>5) rgb=x;
			if (h2>6) rgb=0;

			if (b==0) rgb=1;
			

			return rgb+minval;
			break;

		default:
			return 0.0f;  // if all else fails
			break;

		}
		
	}
	
	if ((intype == glzColorType::HCY_8) || (intype == glzColorType::HCY_32F))
	{
		if (intype == glzColorType::HCY_8) { a /= 255; b /= 255; c /= 255; }


		
		float  h2=a*6.0f;
		float d=b;		
		float minval=0;
		float x = d*(1 - abs(((int)(h2)%2 + (h2) - (int)(h2)) - 1));		
		float  c_r=0.0f,c_g=0.0f,c_b=0.0f;


		if (h2>0) c_r=d;
		if (h2>1) c_r=x;
		if (h2>2) c_r=0;
		if (h2>3) c_r=0;
		if (h2>4) c_r=x;
		if (h2>5) c_r=c;
		if (h2>6) c_r=0;

		if (h2>0) c_g=x;
		if (h2>1) c_g=d;
		if (h2>2) c_g=d;
		if (h2>3) c_g=x;
		if (h2>4) c_g=0;
		if (h2>5) c_g=0;
		if (h2>6) c_g=0;

		if (h2>0) c_b=0;
		if (h2>1) c_b=0;
		if (h2>2) c_b=x;
		if (h2>3) c_b=d;
		if (h2>4) c_b=d;
		if (h2>5) c_b=x;
		if (h2>6) c_b=0;

		if (b==0) {c_r=0; c_g=0; c_b=0;}

		minval=c-(0.3f*c_r+0.59f*c_g+0.11f*c_b);


		switch(outtype)
		{

		case glzColorChannels::RED:
			return c_r+minval;
			break;

		case glzColorChannels::GREEN:
			return c_g+minval;
			break;

		case glzColorChannels::BLUE:
			return c_b+minval;
			break;

		default:
			return 0.0f;  // if all else fails
			break;

		}


	}

	if ((intype == glzColorType::HSI_8) || (intype == glzColorType::HSI_32F))
	{
		if (intype == glzColorType::HSI_8) { a /= 255; b /= 255; c /= 255; }
				
		float  h2=a*360.0f,c_r=0.0f,c_g=0.0f,c_b=0.0f;

        if(h2>=0.0f && h2<=(360.0f/3.0f))
        {
		//	backR = (int) (c + (c * b) * cos(resultHue) / cos(60-resultHue));

			c_r=(c +(c*b)*cos(h2*(float)PI_OVER_180) / cos((60.0f-h2)*(float)PI_OVER_180));
			c_g=(c +(c*b)*(1-cos(h2*(float)PI_OVER_180) / cos((60.0f-h2)*(float)PI_OVER_180)));
			c_b=(c -(c*b));
        }
        else if(h2>(360.0f/3.0f) && h2<=(2.0f*360.0f/3.0f))
        {
            h2-=(360.0f/3.0f);

            c_r=(c-(c*b));
            c_g=(c+(c*b)*cos(h2*(float)PI_OVER_180) / cos((60.0f-h2)*(float)PI_OVER_180));
            c_b=(c+(c*b)*(1-cos(h2*(float)PI_OVER_180) / cos((60.0f-h2)*(float)PI_OVER_180)));

        }
        else /* h>240 h<360 */
        {
            h2-=(2.0*360.0f/3.0f);

			c_r=(c+(c*b)*(1-cos(h2*(float)PI_OVER_180) / cos((60.0f-h2)*(float)PI_OVER_180)));
            c_g=(c-(c*b));
            c_b=(c+(c*b)*cos(h2*(float)PI_OVER_180) / cos((60.0f-h2)*(float)PI_OVER_180));
            
        }


		switch(outtype)
		{

		case glzColorChannels::RED:
			return c_r;
			break;

		case glzColorChannels::GREEN:
			return c_g;
			break;

		case glzColorChannels::BLUE:
			return c_b;
			break;

		default:
			return 0.0f;  // if all else fails
			break;

		}

	}
	
	



	return 0.0f;  // if all else fails
}






// specialty code

float glzRemapToRange(float curmin,float cur,float curmax, float rmin, float rmax)
{
	// remaps one 1d coordinate space to another
	// used for stuff like converting frame numbers to time or some very specific movement
	// example: if cur is set to curmin then rmin will return, if set to curmax then rmax will return
	// if set halfway inbetween then that a value between rmin and rmax will return
	// cur can also be outside curmin and curmax and work just as well
	float curpercent=(cur-curmin)/(curmax-curmin);
	float rmagnitude=rmax-rmin;
	return rmin+(curpercent*rmagnitude);
}

float glzLerpRange(float curmin, float cur, float curmax, float rmin, float rmax)
{
	return glzRemapToRange( curmin,  cur,  curmax,  rmin,  rmax);
}

float glzLerpRange2D(float curmin[2], float cur[2], float curmax[2], float rmin[2], float rmax[2])
{
	// remaps one 2d coordinate space to another
	// much like the glzRemapToRange but with two rows that are in turn interpolated with each other

	float a1 = glzRemapToRange(curmin[0], cur[0], curmax[0], rmin[0], rmax[0]);

	float a2 = glzRemapToRange(curmin[0], cur[0], curmax[0], rmin[1], rmax[1]);

	float a3 = glzRemapToRange(curmin[1], cur[1], curmax[1], a1, a2);

	return a3;

}

float glzLerpRange3D(float curmin[4], float cur[3], float curmax[4], float rmin[4], float rmax[4])
{
	// remaps one 3d coordinate space to another
	// much like the 2D version but with two rows that are in turn interpolated with each other

	float a1 = glzRemapToRange(curmin[0], cur[0], curmax[0], rmin[0], rmax[0]);

	float a2 = glzRemapToRange(curmin[0], cur[0], curmax[0], rmin[1], rmax[1]);

	float a3 = glzRemapToRange(curmin[0], cur[0], curmax[0], rmin[2], rmax[2]);

	float a4 = glzRemapToRange(curmin[0], cur[0], curmax[0], rmin[3], rmax[3]);

	float b1 = glzRemapToRange(curmin[1], cur[1], curmax[1], a1, a2);

	float b2 = glzRemapToRange(curmin[1], cur[1], curmax[1], a3, a4);

	float r = glzRemapToRange(curmin[2], cur[2], curmax[2], b1, b2);

	return r;

}

float glzLerp(float cur, float rmin, float rmax)
{

	return glzRemapToRange(0.0, cur, 1.0, rmin, rmax);
}

float glzLerp2D(float cur[2], float rmin[2], float rmax[2])
{
	float a[2] = { 0.0, 0.0 };
	float b[2] = { 1.0, 1.0 };

	return glzLerpRange2D(a, cur, b, rmin, rmax);
}

float glzLerp3D(float cur[3], float rmin[4], float rmax[4])
{	
	float a[4] = { 0.0, 0.0, 0.0,0.0 };
	float b[4] = { 1.0, 1.0, 1.0,1.0 };	

	return glzLerpRange3D(a, cur, b, rmin, rmax);
}


void glzRemapToRangeArray(float curmin,float curmax, float rmin, float rmax, float *curdata, unsigned int num)
{
	// remaps an 1d coordinate space to another with an entire array instead of a single coordinate
	// used for stuff like converting frame numbers to time or some very specific movement
	// example: if cur is set to curmin then rmin will return, if set to curmax then rmax will return
	// if set halfway inbetween then that a value between rmin and rmax will return
	// cur can also be outside curmin and curmax and work just as well

	float curpercent,rmagnitude;
	unsigned int i=0;
	rmagnitude=rmax-rmin;

	while (i<num)
	{
	curpercent=(curdata[i]-curmin)/(curmax-curmin);	
	curdata[i]=rmin+(curpercent*rmagnitude);
	i++;
	}


	return;
}




void glzCubicCurve(float pos, float l, float p1[3], float v1[3], float p2[3], float v2[3], float *ret)
{
	// used for creading a cubic curve between p1 and p2
	// usefull for making smooth animated paths
	// v1 dictates in which direction you enter the curce and v2 in which you leave
	// l alows you to adjust how sharp or smooth the curve is the actual value depends on a lot of factors 
	// like distance between p1 and p2 and the values of v1 and v2, default should be 1.0
	// pos is a float between 0.0 and 1.0 that points to where on the curve it is
	float p1e[3], p2e[3];

	p1e[0]=p1[0]+((v1[0]*l)*pos);
	p1e[1]=p1[1]+((v1[1]*l)*pos);
	p1e[2]=p1[2]+((v1[2]*l)*pos);


	p2e[0]=p2[0]+(((-1*v2[0])*l)*(1-pos));
	p2e[1]=p2[1]+(((-1*v2[1])*l)*(1-pos));
	p2e[2]=p2[2]+(((-1*v2[2])*l)*(1-pos));


	ret[0]=(p1e[0]*(1-pos))+(p2e[0]*pos);
	ret[1]=(p1e[1]*(1-pos))+(p2e[1]*pos);
	ret[2]=(p1e[2]*(1-pos))+(p2e[2]*pos);

	return;
}




void glzAtlasQuad(unsigned int xres, unsigned int yres, unsigned int atlas, glzOrigin origin, float *uvOut)
{
	// generates 4 float[2] coordinates that corresponds to the sub image of an images atlas

	float xwidth=float(1.0f/xres);
	float ywidth=float(1.0f/yres);



	unsigned int x=0,y=0;

	if (atlas>=xres*yres) x=xres*yres-1;
	else x=atlas;
	//if (x<0) x=0;

	while (x>=xres)
	{
		x-=xres;
		y++;
	}

	y=yres-y;

	// bottom left
	// (0,0)
	uvOut[0]=(x*xwidth);
	uvOut[1]=(y*ywidth)-ywidth;

	// (0,1)
	uvOut[2]=(x*xwidth);
	uvOut[3]=(y*ywidth);

	// (1,1)
	uvOut[4]=(x*xwidth)+xwidth;
	uvOut[5]=(y*ywidth);

	// (1,0)
	uvOut[6]=(x*xwidth)+xwidth;
	uvOut[7]=(y*ywidth)-ywidth;

	if ((origin == glzOrigin::BOTTOM_RIGHT) || (origin == glzOrigin::TOP_RIGHT))
	{
		// preform a x-flip
		uvOut[0] = 1.0f - uvOut[0];
		uvOut[2] = 1.0f - uvOut[2];
		uvOut[4] = 1.0f - uvOut[4];
		uvOut[6] = 1.0f - uvOut[6];
		
	}

	if ((origin == glzOrigin::TOP_LEFT) || (origin == glzOrigin::TOP_RIGHT))
	{
		// preform a y-flip
		uvOut[1] = 1.0f - uvOut[1];
		uvOut[3] = 1.0f - uvOut[3];
		uvOut[5] = 1.0f - uvOut[5];
		uvOut[7] = 1.0f - uvOut[7];

	}

}

glzSprite glzAtlasQuad(unsigned int xres, unsigned int yres, unsigned int atlas, glzOrigin origin)
{
	// generates 4 float[2] coordinates that corresponds to the sub image of an images atlas

	float xwidth = float(1.0f / xres);
	float ywidth = float(1.0f / yres);

	glzSprite atlasquad;

	unsigned int x = 0, y = 0;

	if (atlas >= xres*yres) x = xres*yres - 1;
	else x = atlas;
	//if (x<0) x=0;

	while (x >= xres)
	{
		x -= xres;
		y++;
	}

	y = yres - y;

	// bottom left
	// (0,0) a
	atlasquad.a.u = (x*xwidth);
	atlasquad.a.v = (y*ywidth) - ywidth;

	// (0,1) c
	atlasquad.c.u = (x*xwidth);
	atlasquad.c.v = (y*ywidth);

	// (1,1) d
	atlasquad.d.u = (x*xwidth) + xwidth;
	atlasquad.d.v = (y*ywidth);

	// (1,0) b
	atlasquad.b.u = (x*xwidth) + xwidth;
	atlasquad.b.v = (y*ywidth) - ywidth;

	if ((origin == glzOrigin::BOTTOM_RIGHT) || (origin == glzOrigin::TOP_RIGHT))
	{
		// preform a x-flip
		atlasquad.a.u = 1.0f - atlasquad.a.u;
		atlasquad.b.u = 1.0f - atlasquad.b.u;
		atlasquad.c.u = 1.0f - atlasquad.c.u;
		atlasquad.d.u = 1.0f - atlasquad.d.u;

	}

	if ((origin == glzOrigin::TOP_LEFT) || (origin == glzOrigin::TOP_RIGHT))
	{
		// preform a y-flip
		atlasquad.a.v = 1.0f - atlasquad.a.v;
		atlasquad.b.v = 1.0f - atlasquad.b.v;
		atlasquad.c.v = 1.0f - atlasquad.c.v;
		atlasquad.d.v = 1.0f - atlasquad.d.v;

	}

	return atlasquad;
}

void glzAtlasAniQuad(unsigned int xres, unsigned int yres, float time, glzOrigin origin, float *uvout)
{

	// same as above but instead of a direct reference we use a time variable instead, useful for animation but is not used in the basecode due to the static nature of vertex buffers
	// to get the same effect generate a GLZ_PRIMITIVE_SPRITE_ATLAS_ARRAY primitive and sellect the current frame to render with either a timer or glzRemapToRange

	float xwidth=float(1.0f/xres);
	float ywidth=float(1.0f/yres);

	unsigned int x=0,y=yres;

	float t=time;
	
	if (t>1) {while(t>1) t--;}
	if (t<0) {while(t<0) t++;}

	unsigned int num=int((xres*yres)*t);

	if (num>xres*yres) x=xres*yres;
	else x=num;
	if (x<0) x=0;

	while (x>=xres)
	{
		x-=xres;
		y++;
	}


	y=yres-y;

	// (0,0)
	uvout[0]=(x*xwidth);
	uvout[1]=(y*ywidth);

	// (0,1)
	uvout[2]=(x*xwidth);
	uvout[3]=(y*ywidth)-ywidth;

	// (1,1)
	uvout[4]=(x*xwidth)+xwidth;
	uvout[5]=(y*ywidth)-ywidth;

	// (1,0)
	uvout[6]=(x*xwidth)+xwidth;
	uvout[7]=(y*ywidth);


	if ((origin == glzOrigin::BOTTOM_RIGHT) || (origin == glzOrigin::TOP_RIGHT))
	{
		// preform a x-flip
		uvout[0] = 1.0f - uvout[0];
		uvout[2] = 1.0f - uvout[2];
		uvout[4] = 1.0f - uvout[4];
		uvout[6] = 1.0f - uvout[6];

	}

	if ((origin == glzOrigin::TOP_LEFT) || (origin == glzOrigin::TOP_RIGHT))
	{
		// preform a y-flip
		uvout[1] = 1.0f - uvout[1];
		uvout[3] = 1.0f - uvout[3];
		uvout[5] = 1.0f - uvout[5];
		uvout[7] = 1.0f - uvout[7];

	}

}


void glzAtlasUVarrayRemap(unsigned int atlas, unsigned int num, unsigned int aw, unsigned int ah, glzOrigin origin, float *uv)
{
	// i use this to make normally mapped objects into atlas mapped objects
	float quv[8];

	glzAtlasQuad(aw,ah,atlas,origin, quv);

	unsigned int i=0;

	while (i<num)
	{
	uv[0+i*2]=glzRemapToRange(0,uv[0+i*2],1, quv[0], quv[4]);
	uv[1+i*2]=glzRemapToRange(0,uv[1+i*2],1, quv[1], quv[3]);
	i++;
	}

	return;

}

void glzAtlasUVarrayRemap(unsigned int atlas, unsigned int aw, unsigned int ah, glzOrigin origin, vector<poly3> *p, int group)
{
	float quv[8];

	glzAtlasQuad(aw, ah, atlas, origin, quv);

	int i2 = 0;


	auto i = p->begin();
	i2 = 0;
	while (i < p->end()) {

		if (p->at(i2).group == group)
		{
			p->at(i2).a.t.u = glzRemapToRange(0, p->at(i2).a.t.u, 1, quv[0], quv[4]);
			p->at(i2).a.t.v = glzRemapToRange(0, p->at(i2).a.t.v, 1, quv[1], quv[3]);

			p->at(i2).b.t.u = glzRemapToRange(0, p->at(i2).b.t.u, 1, quv[0], quv[4]);
			p->at(i2).b.t.v = glzRemapToRange(0, p->at(i2).b.t.v, 1, quv[1], quv[3]);

			p->at(i2).c.t.u = glzRemapToRange(0, p->at(i2).c.t.u, 1, quv[0], quv[4]);
			p->at(i2).c.t.v = glzRemapToRange(0, p->at(i2).c.t.v, 1, quv[1], quv[3]);

		}

		++i;
		i2++;
	}
	return;
}

void glzAtlasUVarrayRemap(unsigned int atlas, unsigned int aw, unsigned int ah, glzOrigin origin, vector<poly3> *p, int group, int side)
{
	// i use this to make normally mapped objects into atlas mapped objects
	float quv[8];

	glzAtlasQuad(aw, ah, atlas, origin, quv);

	int i2 = 0;


	auto i = p->begin();
	i2 = 0; 
	while (i < p->end()) {

		if ((p->at(i2).group == group) && (p->at(i2).atlas == side))
		{
			p->at(i2).a.t.u = glzRemapToRange(0, p->at(i2).a.t.u, 1, quv[0], quv[4]);
			p->at(i2).a.t.v = glzRemapToRange(0, p->at(i2).a.t.v, 1, quv[1], quv[3]);

			p->at(i2).b.t.u = glzRemapToRange(0, p->at(i2).b.t.u, 1, quv[0], quv[4]);
			p->at(i2).b.t.v = glzRemapToRange(0, p->at(i2).b.t.v, 1, quv[1], quv[3]);

			p->at(i2).c.t.u = glzRemapToRange(0, p->at(i2).c.t.u, 1, quv[0], quv[4]);
			p->at(i2).c.t.v = glzRemapToRange(0, p->at(i2).c.t.v, 1, quv[1], quv[3]);

		}

	++i;
	i2++;
	}
	return;

}

void glzAtlasUVarrayRemapRotate(unsigned int r, unsigned int atlas, unsigned int num, unsigned int aw, unsigned int ah, glzOrigin origin, float *uv)
{
	// same as above except i now also rotate the original uv coordinates

	float rm[4]= {1,0,0,1};
	

	if(r==1)  //90 degrees
	{	
		rm[0]=0;
		rm[1]=-1;
		rm[2]=1;
		rm[3]=0;
	}
	if(r==2)  //180 degrees
	{	
		rm[0]=-1;
		rm[1]=0;
		rm[2]=0;
		rm[3]=-1;
	}
	if(r==3)  //270 degrees
	{	
		rm[0]=0;
		rm[1]=1;
		rm[2]=-1;
		rm[3]=0;
	}

	float u,v;

	unsigned int i=0;
	while (i<num)
	{
	u=((uv[0+i*2]-0.5f)*rm[0])+((uv[1+i*2]-0.5f)*rm[1]);
	v=((uv[0+i*2]-0.5f)*rm[2])+((uv[1+i*2]-0.5f)*rm[3]);

	uv[0+i*2]=u+0.5f;
	uv[1+i*2]=v+0.5f;
	i++;
	}


	float quv[8];

	glzAtlasQuad(aw,ah,atlas,origin, quv);

	i=0;

	while (i<num)
	{
	uv[0+i*2]=glzRemapToRange(0,uv[0+i*2],1, quv[0], quv[4]);
	uv[1+i*2]=glzRemapToRange(0,uv[1+i*2],1, quv[1], quv[3]);
	i++;
	}

	return;

}



void glzAtlasUVarrayRemapRotate(unsigned int r, unsigned int atlas, unsigned int aw, unsigned int ah, glzOrigin origin, vector<poly3> *p, int group, int side)
{
	// same as above except i now also rotate the original uv coordinates

	double rm[4] = { 1, 0, 0, 1 };


	if (r == 1)  //90 degrees
	{
		rm[0] = 0;
		rm[1] = -1;
		rm[2] = 1;
		rm[3] = 0;
	}
	if (r == 2)  //180 degrees
	{
		rm[0] = -1;
		rm[1] = 0;
		rm[2] = 0;
		rm[3] = -1;
	}
	if (r == 3)  //270 degrees
	{
		rm[0] = 0;
		rm[1] = 1;
		rm[2] = -1;
		rm[3] = 0;
	}
	int i2 = 0;


	auto i = p->begin();
	double u, v;


	i2 = 0;
	while (i < p->end()) {

		if ((p->at(i2).group == group) && (p->at(i2).atlas == side))
		{

			u = ((p->at(i2).a.t.u - 0.5)*rm[0]) + ((p->at(i2).a.t.v - 0.5)*rm[1]);
			v = ((p->at(i2).a.t.u - 0.5)*rm[2]) + ((p->at(i2).a.t.v - 0.5)*rm[3]);
			p->at(i2).a.t.u = u + 0.5;
			p->at(i2).a.t.v = v + 0.5;

			u = ((p->at(i2).b.t.u - 0.5)*rm[0]) + ((p->at(i2).b.t.v - 0.5)*rm[1]);
			v = ((p->at(i2).b.t.u - 0.5)*rm[2]) + ((p->at(i2).b.t.v - 0.5)*rm[3]);
			p->at(i2).b.t.u = u + 0.5;
			p->at(i2).b.t.v = v + 0.5;

			u = ((p->at(i2).c.t.u - 0.5)*rm[0]) + ((p->at(i2).c.t.v - 0.5)*rm[1]);
			v = ((p->at(i2).c.t.u - 0.5)*rm[2]) + ((p->at(i2).c.t.v - 0.5)*rm[3]);
			p->at(i2).c.t.u = u + 0.5;
			p->at(i2).c.t.v = v + 0.5;

		}

		++i;
		i2++;
	}

	
	float quv[8];
	i2 = 0;

	glzAtlasQuad(aw, ah, atlas, origin, quv);

	i = p->begin();
	i2 = 0;
	while (i < p->end()) {

		if ((p->at(i2).group == group) && (p->at(i2).atlas == side))
		{
			p->at(i2).a.t.u = glzRemapToRange(0, p->at(i2).a.t.u, 1, quv[0], quv[4]);
			p->at(i2).a.t.v = glzRemapToRange(0, p->at(i2).a.t.v, 1, quv[1], quv[3]);

			p->at(i2).b.t.u = glzRemapToRange(0, p->at(i2).b.t.u, 1, quv[0], quv[4]);
			p->at(i2).b.t.v = glzRemapToRange(0, p->at(i2).b.t.v, 1, quv[1], quv[3]);

			p->at(i2).c.t.u = glzRemapToRange(0, p->at(i2).c.t.u, 1, quv[0], quv[4]);
			p->at(i2).c.t.v = glzRemapToRange(0, p->at(i2).c.t.v, 1, quv[1], quv[3]);

		}

		++i;
		i2++;
	}
	return;

}
// matrix stuff
// all these matrix functions behave exactly as ther openGL counterparts besides the matrix component that you need to pass around, it makes it a bit more flexible though

void glzProjectVertex(float  *vert, float Matrix[16])
{
	float v[3];
	v[0] = (vert[0] * Matrix[0]) + (vert[1] * Matrix[4]) + (vert[2] * Matrix[8]) +Matrix[12];
	v[1] = (vert[0] * Matrix[1]) + (vert[1] * Matrix[5]) + (vert[2] * Matrix[9]) +Matrix[13];
	v[2] = (vert[0] * Matrix[2]) + (vert[1] * Matrix[6]) + (vert[2] * Matrix[10]) +Matrix[14];

	vert[0] =v[0];
	vert[1] =v[1];
	vert[2] =v[2];
}

void glzProjectVertexArray(float  *vert, float Matrix[16], int num)
{
	int i=0;
	for(i=0; i<num; i++)
	{
		glzProjectVertex(&vert[i*3], Matrix);
	}	
}


void glzProjectVertex(poly3 *p, float Matrix[16], int group)
{
	double v[3];
	if (p->group != group) return;
	v[0] = (p->a.v.x * Matrix[0]) + (p->a.v.y * Matrix[4]) + (p->a.v.z * Matrix[8]) + Matrix[12];
	v[1] = (p->a.v.x * Matrix[1]) + (p->a.v.y * Matrix[5]) + (p->a.v.z * Matrix[9]) + Matrix[13];
	v[2] = (p->a.v.x * Matrix[2]) + (p->a.v.y * Matrix[6]) + (p->a.v.z * Matrix[10]) + Matrix[14];

	p->a.v.x = v[0];
	p->a.v.y = v[1];
	p->a.v.z = v[2];

	v[0] = (p->b.v.x * Matrix[0]) + (p->b.v.y * Matrix[4]) + (p->b.v.z * Matrix[8]) + Matrix[12];
	v[1] = (p->b.v.x * Matrix[1]) + (p->b.v.y * Matrix[5]) + (p->b.v.z * Matrix[9]) + Matrix[13];
	v[2] = (p->b.v.x * Matrix[2]) + (p->b.v.y * Matrix[6]) + (p->b.v.z * Matrix[10]) + Matrix[14];

	p->b.v.x = v[0];
	p->b.v.y = v[1];
	p->b.v.z = v[2];

	v[0] = (p->c.v.x * Matrix[0]) + (p->c.v.y * Matrix[4]) + (p->c.v.z * Matrix[8]) + Matrix[12];
	v[1] = (p->c.v.x * Matrix[1]) + (p->c.v.y * Matrix[5]) + (p->c.v.z * Matrix[9]) + Matrix[13];
	v[2] = (p->c.v.x * Matrix[2]) + (p->c.v.y * Matrix[6]) + (p->c.v.z * Matrix[10]) + Matrix[14];

	p->c.v.x = v[0];
	p->c.v.y = v[1];
	p->c.v.z = v[2];

}

void glzProjectVertex(poly3 *p, glzMatrix m, int group)
{
	if (p->group == group)
		{

			p->a.v.project(m);
			p->b.v.project(m);
			p->c.v.project(m);

			p->a.n.project(m);
			p->b.n.project(m);
			p->c.n.project(m);

		}
}



void glzProjectVertexArray(vector<poly3> *p, float Matrix[16], int group)
{

	double v[3];
	int i2 = 0;

	auto i = p->begin();
	i2 = 0;
	while (i < p->end()) {

		if (p->at(i2).group == group)
		{
		
			v[0] = (p->at(i2).a.v.x * Matrix[0]) + (p->at(i2).a.v.y * Matrix[4]) + (p->at(i2).a.v.z * Matrix[8]) + Matrix[12];
			v[1] = (p->at(i2).a.v.x * Matrix[1]) + (p->at(i2).a.v.y * Matrix[5]) + (p->at(i2).a.v.z * Matrix[9]) + Matrix[13];
			v[2] = (p->at(i2).a.v.x * Matrix[2]) + (p->at(i2).a.v.y * Matrix[6]) + (p->at(i2).a.v.z * Matrix[10]) + Matrix[14];

			p->at(i2).a.v.x = v[0];
			p->at(i2).a.v.y = v[1];
			p->at(i2).a.v.z = v[2];

			v[0] = (p->at(i2).b.v.x * Matrix[0]) + (p->at(i2).b.v.y * Matrix[4]) + (p->at(i2).b.v.z * Matrix[8]) + Matrix[12];
			v[1] = (p->at(i2).b.v.x * Matrix[1]) + (p->at(i2).b.v.y * Matrix[5]) + (p->at(i2).b.v.z * Matrix[9]) + Matrix[13];
			v[2] = (p->at(i2).b.v.x * Matrix[2]) + (p->at(i2).b.v.y * Matrix[6]) + (p->at(i2).b.v.z * Matrix[10]) + Matrix[14];

			p->at(i2).b.v.x = v[0];
			p->at(i2).b.v.y = v[1];
			p->at(i2).b.v.z = v[2];

			v[0] = (p->at(i2).c.v.x * Matrix[0]) + (p->at(i2).c.v.y * Matrix[4]) + (p->at(i2).c.v.z * Matrix[8]) + Matrix[12];
			v[1] = (p->at(i2).c.v.x * Matrix[1]) + (p->at(i2).c.v.y * Matrix[5]) + (p->at(i2).c.v.z * Matrix[9]) + Matrix[13];
			v[2] = (p->at(i2).c.v.x * Matrix[2]) + (p->at(i2).c.v.y * Matrix[6]) + (p->at(i2).c.v.z * Matrix[10]) + Matrix[14];

			p->at(i2).c.v.x = v[0];
			p->at(i2).c.v.y = v[1];
			p->at(i2).c.v.z = v[2];
		}
		++i;
		i2++;
	}
}


void glzProjectVertexArray(vector<poly3> *p, glzMatrix m, int group)
{

	int i2 = 0;

	auto i = p->begin();
	while (i < p->end()) {

		if (p->at(i2).group == group)
		{

			p->at(i2).a.v.project(m);
			p->at(i2).b.v.project(m);
			p->at(i2).c.v.project(m);

			p->at(i2).a.n.project(m);
			p->at(i2).b.n.project(m);
			p->at(i2).c.n.project(m);

		}
		++i;
		i2++;
	}
}

float glzScanVertexArray(float *vert, long num, glzBoundingScan scan)
{

	float r = 0, r2 = 0, r3 = 0;


	// set initial conditiona

	switch (scan)
	{

	case glzBoundingScan::LEFT:
	case glzBoundingScan::RIGHT:
	case glzBoundingScan::WIDTH:
	case glzBoundingScan::CENTER_X:

		r = vert[0];
		r2 = vert[0];
		r3 = vert[0];

		break;


	case glzBoundingScan::TOP:
	case glzBoundingScan::BOTTOM:
	case glzBoundingScan::HEIGHT:
	case glzBoundingScan::CENTER_Y:

		r = vert[1];
		r2 = vert[1];
		r3 = vert[1];

		break;


	}



	int i = 0;
	for (i = 0; i<num; i++)
	{

		switch (scan)
		{
		
			case glzBoundingScan::LEFT:
				
				if (r < vert[i * 3])
					r = vert[i * 3];

				break;

			case glzBoundingScan::RIGHT:

				if (r > vert[i * 3])
					r = vert[i * 3];

				break;

			case glzBoundingScan::TOP:

				if (r > vert[i * 3+1])
					r = vert[i * 3+1];

				break;

			case glzBoundingScan::BOTTOM:

				if (r < vert[i * 3+1])
					r = vert[i * 3+1];

				break;

			case glzBoundingScan::WIDTH:

				if (r2 > vert[i * 3])
					r2 = vert[i * 3];	

				if (r3 < vert[i * 3])
					r3 = vert[i * 3];

				r = r2 - r3;

				break;

			case glzBoundingScan::HEIGHT:

				if (r2 > vert[i * 3 + 1])
					r2 = vert[i * 3 + 1];

				if (r3 < vert[i * 3 + 1])
					r3 = vert[i * 3 + 1];

				r = r2 - r3;

				break;

			case glzBoundingScan::CENTER_X:

				if (r2 > vert[i * 3])
					r2 = vert[i * 3];

				if (r3 < vert[i * 3])
					r3 = vert[i * 3];

				r = r3 + r2*0.5f;

				break;

			case glzBoundingScan::CENTER_Y:

				if (r2 > vert[i * 3 + 1])
					r2 = vert[i * 3 + 1];

				if (r3 < vert[i * 3 + 1])
					r3 = vert[i * 3 + 1];

				r = r3 + r2*0.5f;

				break;
		}
		


		

	}


return r;
}


double glzScanVectorArray(vector<poly3> pdata, int group, glzBoundingScan scan)
{

	double r = 0, r2 = 0, r3 = 0;

	if (!pdata.size()) return 0.0;

	// set initial conditiona

	switch (scan)
	{

	case glzBoundingScan::LEFT:
	case glzBoundingScan::RIGHT:
	case glzBoundingScan::WIDTH:
	case glzBoundingScan::CENTER_X:

		r = pdata.at(0).a.v.x;
		r2 = pdata.at(0).a.v.x;
		r3 = pdata.at(0).a.v.x;

		break;


	case glzBoundingScan::TOP:
	case glzBoundingScan::BOTTOM:
	case glzBoundingScan::HEIGHT:
	case glzBoundingScan::CENTER_Y:

		r = pdata.at(0).a.v.y;
		r2 = pdata.at(0).a.v.y;
		r3 = pdata.at(0).a.v.y;

		break;


	}



	for (auto p:pdata)
	{
		if (p.group == group)
		{
			switch (scan)
			{

			case glzBoundingScan::LEFT:

				if (r < p.a.v.x)
					r = p.a.v.x;

				if (r < p.b.v.x)
					r = p.b.v.x;

				if (r < p.c.v.x)
					r = p.c.v.x;

				break;

			case glzBoundingScan::RIGHT:

				if (r > p.a.v.x)
					r = p.a.v.x;

				if (r > p.b.v.x)
					r = p.b.v.x;

				if (r > p.c.v.x)
					r = p.c.v.x;

				break;

			case glzBoundingScan::TOP:

				if (r > p.a.v.y)
					r = p.a.v.y;

				if (r > p.b.v.y)
					r = p.b.v.y;

				if (r > p.c.v.y)
					r = p.c.v.y;

				break;

			case glzBoundingScan::BOTTOM:

				if (r < p.a.v.y)
					r = p.a.v.y;

				if (r < p.b.v.y)
					r = p.b.v.y;

				if (r < p.c.v.y)
					r = p.c.v.y;

				break;

			case glzBoundingScan::WIDTH:

				if (r2 > p.a.v.x)
					r2 = p.a.v.x;

				if (r2 > p.b.v.x)
					r2 = p.b.v.x;

				if (r2 > p.c.v.x)
					r2 = p.c.v.x;

				if (r3 < p.a.v.x)
					r3 = p.a.v.x;

				if (r3 < p.b.v.x)
					r3 = p.b.v.x;

				if (r3 < p.c.v.x)
					r3 = p.c.v.x;

				r = r2 - r3;

				break;

			case glzBoundingScan::HEIGHT:

				if (r2 > p.a.v.y)
					r2 = p.a.v.y;

				if (r2 > p.b.v.y)
					r2 = p.b.v.y;

				if (r2 > p.c.v.y)
					r2 = p.c.v.y;

				if (r3 < p.a.v.y)
					r3 = p.a.v.y;

				if (r3 < p.b.v.y)
					r3 = p.b.v.y;

				if (r3 < p.c.v.y)
					r3 = p.c.v.y;

				r = r2 - r3;

				break;

			case glzBoundingScan::CENTER_X:

				if (r2 > p.a.v.x)
					r2 = p.a.v.x;

				if (r2 > p.b.v.x)
					r2 = p.b.v.x;

				if (r2 > p.c.v.x)
					r2 = p.c.v.x;

				if (r3 < p.a.v.x)
					r3 = p.a.v.x;

				if (r3 < p.b.v.x)
					r3 = p.b.v.x;

				if (r3 < p.c.v.x)
					r3 = p.c.v.x;

				r = r3 + r2*0.5f;

				break;

			case glzBoundingScan::CENTER_Y:

				if (r2 > p.a.v.y)
					r2 = p.a.v.y;

				if (r2 > p.b.v.y)
					r2 = p.b.v.y;

				if (r2 > p.c.v.y)
					r2 = p.c.v.y;

				if (r3 < p.a.v.y)
					r3 = p.a.v.y;

				if (r3 < p.b.v.y)
					r3 = p.b.v.y;

				if (r3 < p.c.v.y)
					r3 = p.c.v.y;

				r = r3 + r2*0.5f;

				break;
			}
		}
	}
	return r;
}


void glzRecenterVectorArray(vector<poly3> *pdata, int group, glzOrigin origin)
{

	glzMatrix m;
	m.LoadIdentity();

	if (!pdata->size()) return;

	float left = glzScanVectorArray(*pdata, group, glzBoundingScan::LEFT);
	float right = glzScanVectorArray(*pdata, group, glzBoundingScan::RIGHT);

	float top = glzScanVectorArray(*pdata, group, glzBoundingScan::TOP);
	float bottom = glzScanVectorArray(*pdata, group, glzBoundingScan::BOTTOM);

	float center_x = glzScanVectorArray(*pdata, group, glzBoundingScan::CENTER_X);
	float center_y = glzScanVectorArray(*pdata, group, glzBoundingScan::CENTER_Y);

	float shiftx = 0.0f;
	float shifty = 0.0f;

			switch (origin)
			{
			case glzOrigin::TOP_LEFT:
				shiftx = right;
				shifty = bottom;
				break;

			case glzOrigin::BOTTOM_LEFT:
				shiftx = right;
				shifty = top;
				break;

			case glzOrigin::BOTTOM_RIGHT:
				shiftx = left;
				shifty = top;
				break;

			case glzOrigin::TOP_RIGHT:
				shiftx = left;
				shifty = bottom;
				break;

			case glzOrigin::CENTERED:
				shiftx = (left +right)*0.5;
				shifty = (top + bottom)*0.5;
				break;
			}


		//	shiftx = left;
		//	shifty = bottom;

				m.translate(-shiftx, -shifty, 0);
	

			glzProjectVertexArray(pdata, m, group);

	return;
}






void glzMultMatrix(float *MatrixB,float  MatrixA[16])
{
   float  NewMatrix[16]; // should be done with doubles some day
   int i;

   for(i = 0; i < 4; i++){  //Cycle through each vector of first matrix.
	NewMatrix[i*4]   = MatrixA[i*4] * MatrixB[0] + MatrixA[i*4+1] * MatrixB[4] + MatrixA[i*4+2] * MatrixB[8] + MatrixA[i*4+3] * MatrixB[12];
	NewMatrix[i*4+1] = MatrixA[i*4] * MatrixB[1] + MatrixA[i*4+1] * MatrixB[5] + MatrixA[i*4+2] * MatrixB[9] + MatrixA[i*4+3] * MatrixB[13];
 	NewMatrix[i*4+2] = MatrixA[i*4] * MatrixB[2] + MatrixA[i*4+1] * MatrixB[6] + MatrixA[i*4+2] * MatrixB[10] + MatrixA[i*4+3] * MatrixB[14];	
 	NewMatrix[i*4+3] = MatrixA[i*4] * MatrixB[3] + MatrixA[i*4+1] * MatrixB[7] + MatrixA[i*4+2] * MatrixB[11] + MatrixA[i*4+3] * MatrixB[15];
     }
   /*this should combine the matrixes*/

   memcpy(MatrixB,NewMatrix,64); // but the method of transfering data must be change before doubles can be used

   return;
}



void glzLoadIdentity(float *m)
{
if(!isinited_glz) ini_glz();
	

  m[0]  = 1;
  m[1]  = 0;
  m[2]  = 0;
  m[3]  = 0;

  m[4]  = 0;
  m[5]  = 1;
  m[6]  = 0;
  m[7]  = 0;

  m[8]  = 0;
  m[9]  = 0;
  m[10] = 1;
  m[11] = 0;

  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;

  return;

}


void glzPerspective(float *m, float fov, float aspect,float zNear, float zFar)
{
const float h = 1.0f/tan(fov*(float)PI_OVER_360);
float neg_depth = zNear-zFar;

 float m2[16] = {0};

m2[0] = h / aspect;
m2[1] = 0;
m2[2] = 0;
m2[3] = 0;

m2[4] = 0;
m2[5] = h;
m2[6] = 0;
m2[7] = 0;

m2[8] = 0;
m2[9] = 0;
m2[10] = (zFar + zNear)/neg_depth;
m2[11] = -1;

m2[12] = 0;
m2[13] = 0;
m2[14] = 2.0f*(zNear*zFar)/neg_depth;
m2[15] = 0;

glzMultMatrix(m,m2);

return;

}


void glzOrtho(float *m, float left, float right, float bottom, float top, float Znear, float Zfar)
{

float m2[16] = {0};

m2[0] = 2/(right-left);
m2[1] = 0;
m2[2] = 0;
m2[3] = -((right+left)/(right-left));

m2[4] = 0;
m2[5] = 2/(top-bottom);
m2[6] = 0;
m2[7] = -((top+bottom)/(top-bottom));

m2[8] = 0;
m2[9] = 0;
m2[10] = 2/(Zfar-Znear);
m2[11] = -((Zfar+Znear)/(Zfar-Znear));

m2[12] = 0;
m2[13] = 0;
m2[14] = 0;
m2[15] = 1;

glzMultMatrix(m,m2);

return;

}

void glzOrtho2D(float *m, float left, float right, float bottom, float top)
{

	float Znear = -1.0f, Zfar = 1.0f;
	float m2[16] = { 0 };

	m2[0] = 2 / (right - left);
	m2[1] = 0;
	m2[2] = 0;
	m2[3] = -((right + left) / (right - left));

	m2[4] = 0;
	m2[5] = 2 / (top - bottom);
	m2[6] = 0;
	m2[7] = -((top + bottom) / (top - bottom));

	m2[8] = 0;
	m2[9] = 0;
	m2[10] = 2 / (Zfar - Znear);
	m2[11] = -((Zfar + Znear) / (Zfar - Znear));

	m2[12] = 0;
	m2[13] = 0;
	m2[14] = 0;
	m2[15] = 1;

	glzMultMatrix(m, m2);

	return;

}

void glzOrtho2DPixelspace(float *m, int x, int y, glzOrigin origin)
{

	float m2[16];
	glzLoadIdentity(m2);

	if (origin == glzOrigin::BOTTOM_LEFT)
	{
		glzOrtho2D(m2, -(x*0.5f), (x*0.5f), -(y*0.5f), (y*0.5f));
		glzTranslatef(m2, -(x*0.5f), -(y*0.5f), 0);
	}

	if (origin == glzOrigin::TOP_LEFT)
	{
		glzOrtho2D(m2, (x*0.5f), -(x*0.5f), -(y*0.5f), (y*0.5f));
		glzTranslatef(m2, -(x*0.5f), (y*0.5f), 0);
	}

	glzMultMatrix(m, m2);

	return;

}




void glzTranslatef(float *m,float x,float y, float z)
{
	
	
  float m2[16] = {0};
  float m3[16] = {0};

  m2[0]  = 1;
  m2[1]  = 0;
  m2[2]  = 0;
  m2[3]  = 0;

  m2[4]  = 0;
  m2[5]  = 1;
  m2[6]  = 0;
  m2[7]  = 0;

  m2[8]  = 0;
  m2[9]  = 0;
  m2[10] = 1;
  m2[11] = 0;

  m2[12] = x;
  m2[13] = y;
  m2[14] = z;
  m2[15] = 1;

  glzMultMatrix(m,m2);
  return;

}


void glzScalef(float *m,float x,float y, float z)
{
	
	
  float m2[16] = {0};
  float m3[16] = {0};

  m2[0]  = x;
  m2[1]  = 0;
  m2[2]  = 0;
  m2[3]  = 0;

  m2[4]  = 0;
  m2[5]  = y;
  m2[6]  = 0;
  m2[7]  = 0;

  m2[8]  = 0;
  m2[9]  = 0;
  m2[10] = z;
  m2[11] = 0;

  m2[12] = 0;
  m2[13] = 0;
  m2[14] = 0;
  m2[15] = 1;

  glzMultMatrix(m,m2);
  return;

}


void glzRotatef(float *m, float a, float x,float y, float z)
{

 
  float angle=a*(float)PI_OVER_180;

	
  float m2[16] = {0};

  m2[0]  = 1+(1-cos(angle))*(x*x-1);
  m2[1]  = -z*sin(angle)+(1-cos(angle))*x*y;
  m2[2]  = y*sin(angle)+(1-cos(angle))*x*z;
  m2[3]  = 0;

  m2[4]  = z*sin(angle)+(1-cos(angle))*x*y;
  m2[5]  = 1+(1-cos(angle))*(y*y-1);
  m2[6]  = -x*sin(angle)+(1-cos(angle))*y*z;
  m2[7]  = 0;

  m2[8]  = -y*sin(angle)+(1-cos(angle))*x*z; 
  m2[9]  = x*sin(angle)+(1-cos(angle))*y*z;
  m2[10] = 1+(1-cos(angle))*(z*z-1);
  m2[11] = 0;

  m2[12] = 0;
  m2[13] = 0;
  m2[14] = 0;
  m2[15] = 1;

  glzMultMatrix(m,m2);


return;
}


// Quaternion stuff


// By the way this is no way anywhere close to being a complete quaternion math library, i just include enough to to make things easier
// it alows you to use quaternions instead of the regular rotations to avoid gimbal lock and then integrate them into a 4x4 matrix
// Though gimbal lock only realy happens in realy specific situations like space and flight simulators in where you use all 3 axis for orientation
// so your FTPS game is pretty much unaffected, though if you use a lot of physics then it could be worth looking into.

// a quaternion is here defined as wxyz and the quick init code this type is thus float q[4] ={1,0,0,0};

// on a side note this is pretty much as close as your gonna get to black magic





void glzLoadQuaternionIdentity(float *q)
{
if(!isinited_glz) ini_glz();
	
  q[0]  = 1;
  q[1]  = 0;
  q[2]  = 0;
  q[3]  = 0; 
  return;
}

void glzMultQuaternion(float *qa, float qb[4])
{
	float  nq[4];
   
	nq[0] = -qa[1]*qb[1]  -  qa[2]*qb[2]  -  qa[3]*qb[3]  +  qa[0]*qb[0];
	nq[1] = qa[1]*qb[0]  +  qa[2]*qb[3]  -  qa[3]*qb[2]  +  qa[0]*qb[1];
	nq[2] = -qa[1]*qb[3]  +  qa[2]*qb[0]  +  qa[3]*qb[1]  +  qa[0]*qb[2];
 	nq[3] = qa[1]*qb[2]  -  qa[2]*qb[1]  +  qa[3]*qb[0]  +  qa[0]*qb[3];	
   
	qa[0] = nq[0];
	qa[1] = nq[1];
	qa[2] = nq[2];
	qa[3] = nq[3];
	
   return;
}

void glzNormalizeQuaternion(float *q)
{
	float n = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
   q[0] /= n;
   q[1] /= n;
   q[2] /= n;
   q[3] /= n;
   return;
}

void glzRotateQuaternionf(float *q, float a, float x, float y, float z)
{ 
	float angle = -a*(float)PI_OVER_360;
	float q2[4] = { 0, 0, 0, 0 };
 
	q2[0]  = cos(angle);
	q2[1]  = sin(angle)*x;
	q2[2]  = sin(angle)*y;
	q2[3]  = sin(angle)*z;

	glzNormalizeQuaternion(q);
	glzMultQuaternion(q,q2);

	return;
}




void glzQuaternionToMatrixf(float *m, float q[4])
{
 float m2[16] = {0};

  m2[0]  = 1- (2*q[2]*q[2]) - (2*q[3]*q[3]);
  m2[1]  = (2*q[1]*q[2]) + (2*q[0]*q[3]);
  m2[2]  = (2*q[1]*q[3]) - (2*q[0]*q[2]);
  m2[3]  = 0;

  m2[4]  = (2*q[1]*q[2]) - (2*q[0]*q[3]);
  m2[5]  = 1- (2*q[1]*q[1]) - (2*q[3]*q[3]);
  m2[6]  = (2*q[2]*q[3]) + (2*q[0]*q[1]);
  m2[7]  = 0;

  m2[8]  = (2*q[1]*q[3]) + (2*q[0]*q[2]); 
  m2[9]  = (2*q[2]*q[3]) - (2*q[0]*q[1]);
  m2[10] = 1- (2*q[1]*q[1]) - (2*q[2]*q[2]);
  m2[11] = 0;

  m2[12] = 0;
  m2[13] = 0;
  m2[14] = 0;
  m2[15] = 1;

  glzMultMatrix(m,m2);


return;
}


// 2D colisions
// i wrote this just for my LD48 #26 game "The escape", it is pretty ok if all your looking for is some mario style physics
// but in a future version this will be replaced with a much more advanced 2D physics toolkit (an by replaced i don't mean this will be deleted, possibly moved though)

unsigned int glzPhys2DColideBox2(float x1,float y1,float r1, float x2,float y2,float r2)
{
	// colides 2 boxes, returns 0 if no colision, 1 is box 1 from above box2, 2=right, 3=below, 4=Left 5= if boxes are exactly centered on each other

	if ((x1==x2) && (y1==y2)) return 5; // an easy check if they both have the same coordinate

	unsigned int colided=0;

	if ((abs(x1-x2)<r1+r2) && (abs(y1-y2)<r1+r2)) colided=1;
	// the way this works is by checking the distance between the coordinates on each axis separatly and if they are within r1+r2 on both axis then the boxes are colliding

	if (!colided) return 0;

	if (abs(x1-x2)>abs(y1-y2))
	{
		// if left right is largest
		if (x1>x2) return 2; //comming from right
		else return 4; //comming from left
	
	} else
	{
		// if top down is largest
		if (y1>y2) return 1; //comming from top
		else return 3; //comming from below
	
	}


	return 0; // default as in they are not touching
}

void setblendingmode(glzBlendingMode bmode)
{

	switch (bmode)
	{
	case glzBlendingMode::NONE:
		glDisable(GL_BLEND);
		return;
		break;

	case glzBlendingMode::ADDITIVE:
		glBlendFunc(GL_ONE, GL_ONE);
		break;

	case glzBlendingMode::ALPHA:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;

	case glzBlendingMode::MULTIPLY:
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		break;
	}

	glEnable(GL_BLEND);
}




