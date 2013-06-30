// Copyright 2013 Peter Wallstr�m
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
#include "ztool-glz.h"
#include <stdio.h>
#include <windows.h>													// Header File For The Windows Library
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library
#include <gl/glext.h>
using namespace std;

static bool isinited_glz;


// currently this thing is entirely useless since this file doesn't even bother to mess with any APIs, it's almost entirely just math, but it might change someday which is why it's here
void ini_glz(void)
{
	
	isinited_glz=true;
}


// usefull stuff

float glzRandf(void)  //produces a value between 1 and 0
{
	return (float)(rand() % RAND_MAX)/RAND_MAX;  // you wouldn't belive how often doing this the wrong way have given me strange results
}


float glzRandfs(void)  //produces a value between 1 and -1
{
	float r;
	r=glzRandf();

	if (glzRandf()<0.5)
	return (r);
	else
	return(-1*r);
}



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
	return r;
}




// some general math functions


float glzDotproductf(float a[3], float b[3])
{
	return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

double glzDotproductd(double a[3], double b[3])
{
	return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

float glzMagnitudef(float a[3])
{

	return (float)sqrt((a[0]*a[0]) + (a[1]*a[1]) + (a[2]*a[2]));
}

double glzMagnituded(double a[3])
{

	return sqrt((a[0]*a[0]) + (a[1]*a[1]) + (a[2]*a[2]));
}

void glzNormalizef(float *a, float le)
{

	float l=le/(float)sqrt((a[0]*a[0]) + (a[1]*a[1]) + (a[2]*a[2]));

	a[0]*=l;
	a[1]*=l;
	a[2]*=l;
	return;
}

void glzNormalized(double *a, double le)
{

	double l=le/(double)sqrt((a[0]*a[0]) + (a[1]*a[1]) + (a[2]*a[2]));

	a[0]*=l;
	a[1]*=l;
	a[2]*=l;
	return;
}

void glzCrossproductf(float a[3], float b[3], float *r)
	{
		r[0]=b[1]*a[2]-a[1]*b[2];
		r[1]=b[2]*a[0]-a[2]*b[0];
		r[2]=b[0]*a[1]-a[0]*b[1];
		return;
	}

void glzCrossproductd(double a[3], double b[3], double *r)
	{
		r[0]=b[1]*a[2]-a[1]*b[2];
		r[1]=b[2]*a[0]-a[2]*b[0];
		r[2]=b[0]*a[1]-a[0]*b[1];
		return;
	}


//image code


// this will take xy coordinates and convert them into 1d coordinates suitable for adressing images
// use GLZ_INVERT as the type if the image data is stored bottom first but the y coordinate is top first

unsigned int glz2dTo1dImageRemap(unsigned int x, unsigned int y, unsigned int col, unsigned int step, unsigned int width, unsigned int height, unsigned int type)
{
	if (type & GLZ_INVERT) return (((((height-1)-y)*width)+x)*step)+col;
	else return ((((width*y)*width)+x)*step)+col;

}

// gets the inerpolated result of a 2d coordinate from a 1D array, it's more or less what openGL does
// this uses the function above so the type has the same function
float glzImageReadBilinear(float x, float y, unsigned int col, unsigned int step, int width, int height, unsigned int mirror, unsigned char *data, unsigned int type)
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


	value[0]=(float)data[glz2dTo1dImageRemap(ix,iy,col,step,width,height,type)]*(1-xfract)/255;
	value[0]+=(float)data[glz2dTo1dImageRemap(ix2,iy,col,step,width,height,type)]*(xfract)/255;
	
	value[1]=(float)data[glz2dTo1dImageRemap(ix,iy2,col,step,width,height,type)]*(1-xfract)/255;
	value[1]+=(float)data[glz2dTo1dImageRemap(ix2,iy2,col,step,width,height,type)]*(xfract)/255;


	r=(value[1]*yfract)+(value[0]*(1-yfract));

	return r;

}

// this will take a RGB color and convert it to pretty much any other color space
// likewise this will also take HSL HSV HSI and HCY (hue croma luma) and convert it back to rgb, that is if all things work properly
// so far any thesting has worked out as it should
// btw this is probably the only code in there i don't fully understand

float glzColorConvert(float a, float b, float c, unsigned int intype, unsigned int outtype)
{



	if ((intype==GLZ_RGB_8) || (intype==GLZ_RGB_32F))
	{
		if (intype==GLZ_RGB_8) {a/=255; b/=255; c/=255;}

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
		case GLZ_RED:
			return a;
			break;
		case GLZ_GREEN:
			return b;
			break;
		case GLZ_BLUE:
			return c;
			break;
		case GLZ_HUE:
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

		case GLZ_HUE2:
			if(max == min) return 0.0f;
			h=(float)atan2(beta,alpha)*(float)PI_OVER_180_REVERSE;
			if(h < 0)
			h += 360.0f;
			return h/360.f;
			break;

		case GLZ_CROMA:
			return d;
			break;
		case GLZ_CROMA2:

			return sqrt((alpha*alpha)+(beta*beta));
			break;

		case GLZ_VALUE:
			return maxval;
			break;

		case GLZ_LIGHTNESS:
			return l;
			break;

		case GLZ_INTENSITY:
			return i;
			break;

		case GLZ_LUMA:
			return y;
			break;

		case GLZ_SHSV:
			if(max == min) return 0.0f;
			return d/maxval;
			break;

		case GLZ_SHSL:
			if(max == min) return 0.0f;			

			if(l>0.5)	return d/(2-(2*l));
			else		return d/(2*l);
			break;

		case GLZ_SHSI:
			if(max == min) return 0.0f;
			return 1-(minval/i);
			break;

		default:
			return 0.0f;  // if all else fails
			break;

		}




		return 0.0f;  // if all else fails
	}



	if ((intype==GLZ_HSL_8) || (intype==GLZ_HSL_32F))
	{
		if (intype==GLZ_HSL_8) {a/=255; b/=255; c/=255;}
		
		float  h2=a*6.0f,rgb=0.0f;
		float d=0.0f,x=0.0f;		

		d = b*(1 - abs(2*c - 1));
		float minval=c-(0.5f*d);
		x = d*(1 - abs(((int)(h2)%2 + (h2) - (int)(h2)) - 1));


		//x=2*c - d;
		
		

		switch(outtype)
		{

		case GLZ_RED:
		
	
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

		case GLZ_GREEN:		
	
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

		case GLZ_BLUE:		
	
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
			
		
	if ((intype==GLZ_HSV_8) || (intype==GLZ_HSV_32F))
	{
		if (intype==GLZ_HSV_8) {a/=255; b/=255; c/=255;}


		float  h2=a*6.0f,rgb=0.0f;
		float d=b*c,x=0.0f;		
		float minval=c-d;
		x = d*(1 - abs(((int)(h2)%2 + (h2) - (int)(h2)) - 1));


		switch(outtype)
		{

		case GLZ_RED:		
	
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

		case GLZ_GREEN:		
	
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

		case GLZ_BLUE:		
	
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
	
	if ((intype==GLZ_HCY_8) || (intype==GLZ_HCY_32F))
	{
		if (intype==GLZ_HCY_8) {a/=255; b/=255; c/=255;}


		
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

		case GLZ_RED:
			return c_r+minval;
			break;

		case GLZ_GREEN:
			return c_g+minval;
			break;

		case GLZ_BLUE:
			return c_b+minval;
			break;

		default:
			return 0.0f;  // if all else fails
			break;

		}


	}

	if ((intype==GLZ_HSI_8) || (intype==GLZ_HSI_32F))
	{
		if (intype==GLZ_HSI_8) {a/=255; b/=255; c/=255;}
				
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

		case GLZ_RED:
			return c_r;
			break;

		case GLZ_GREEN:
			return c_g;
			break;

		case GLZ_BLUE:
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




void glzAtlasQuad(unsigned int xres, unsigned int yres, unsigned int atlas, float *uvOut)
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

}

void glzAtlasAniQuad(unsigned int xres, unsigned int yres, float time, float *uvout)
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

}


void glzAtlasUVarrayRemap(unsigned int atlas, unsigned int num, unsigned int aw, unsigned int ah, float *uv)
{
	// i use this to make normally mapped objects into atlas mapped objects
	float quv[8];

	glzAtlasQuad(aw,ah,atlas, quv);

	unsigned int i=0;

	while (i<num)
	{
	uv[0+i*2]=glzRemapToRange(0,uv[0+i*2],1, quv[0], quv[4]);
	uv[1+i*2]=glzRemapToRange(0,uv[1+i*2],1, quv[1], quv[3]);
	i++;
	}

	return;

}

void glzAtlasUVarrayRemapRotate(unsigned int r,unsigned int atlas, unsigned int num, unsigned int aw,unsigned int ah, float *uv)
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

	glzAtlasQuad(aw,ah,atlas, quv);

	i=0;

	while (i<num)
	{
	uv[0+i*2]=glzRemapToRange(0,uv[0+i*2],1, quv[0], quv[4]);
	uv[1+i*2]=glzRemapToRange(0,uv[1+i*2],1, quv[1], quv[3]);
	i++;
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


void glzMultMatrix(float *MatrixB,float  MatrixA[16])
{
   float  NewMatrix[16];
   int i;

   for(i = 0; i < 4; i++){  //Cycle through each vector of first matrix.
	NewMatrix[i*4]   = MatrixA[i*4] * MatrixB[0] + MatrixA[i*4+1] * MatrixB[4] + MatrixA[i*4+2] * MatrixB[8] + MatrixA[i*4+3] * MatrixB[12];
	NewMatrix[i*4+1] = MatrixA[i*4] * MatrixB[1] + MatrixA[i*4+1] * MatrixB[5] + MatrixA[i*4+2] * MatrixB[9] + MatrixA[i*4+3] * MatrixB[13];
 	NewMatrix[i*4+2] = MatrixA[i*4] * MatrixB[2] + MatrixA[i*4+1] * MatrixB[6] + MatrixA[i*4+2] * MatrixB[10] + MatrixA[i*4+3] * MatrixB[14];	
 	NewMatrix[i*4+3] = MatrixA[i*4] * MatrixB[3] + MatrixA[i*4+1] * MatrixB[7] + MatrixA[i*4+2] * MatrixB[11] + MatrixA[i*4+3] * MatrixB[15];
     }
   /*this should combine the matrixes*/

   memcpy(MatrixB,NewMatrix,64);

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
   
 
	/*this should combine the Quaternions*/

   memcpy(qa,nq,16);

   return;
}

void glzNormalizeQuaternion(float *q)
{
   float n = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
   q[0] /= n;
   q[1] /= n;
   q[2] /= n;
   q[3] /= n;
   return;
}


void glzRotateQuaternionf(float *q, float a, float x,float y, float z)
{ 
	float angle=-a*(float)PI_OVER_360;	
	float q2[4] = {0,0,0,0};
 
	q2[0]  = cos(angle);
	q2[1]  = x*sin(angle);
	q2[2]  = y*sin(angle);
	q2[3]  = z*sin(angle);

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




