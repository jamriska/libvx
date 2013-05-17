/* -*-c++-*-
*
* This file is part of SSTT and licensed under the terms in LICENSE.txt
*
* SSTT - Simplified Spatial Target Tracker
* (c) Copyright 2008-2009 Hartmut Seichter, All Rights Reserved
*
* SSTT is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
*/

#include "ds_capture.h"

#include <stdio.h>

#include "vx/frame.h"
#include "_source.h"

inline
bool ignore_compare(int measurement, int minVal) 
{
	return (minVal < 0) ? false : (measurement < minVal); 
}

#include <string>

//#define SSTT_VERBOSE_DEBUG 1
#undef SSTT_VERBOSE_DEBUG
#if defined(SSTT_VERBOSE_DEBUG)
#include <iostream>
//	#define SSTT_DS_DEBUG(message) \
//		Log::Get().Print(message);
	#define SSTT_DS_DEBUG(message) \
		fprintf(stderr,"%s (%s)\n",__FUNCTION__,message);
#else
	#define SSTT_DS_DEBUG(message)
#endif


#if !defined(_WIN32_WCE) && !defined(__MINGW32__)
#	include <wmsdkidl.h>
#endif
#if 0
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef struct
{
	int rtoy_tab[0x100], gtoy_tab[0x100], btoy_tab[0x100];
	int rtou_tab[0x100], gtou_tab[0x100], btou_tab[0x100];
	int rtov_tab[0x100], gtov_tab[0x100], btov_tab[0x100];

	int vtor_tab[0x100], vtog_tab[0x100];
	int utog_tab[0x100], utob_tab[0x100];
	// Used by init_yuv only
	int *vtor, *vtog, *utog, *utob;

	float vtor_float_tab[0x100], vtog_float_tab[0x100];
	float utog_float_tab[0x100], utob_float_tab[0x100];
	float *vtor_float, *vtog_float, *utog_float, *utob_float;

	int rtoy_tab16[0x10000], gtoy_tab16[0x10000], btoy_tab16[0x10000];
	int rtou_tab16[0x10000], gtou_tab16[0x10000], btou_tab16[0x10000];
	int rtov_tab16[0x10000], gtov_tab16[0x10000], btov_tab16[0x10000];

	int vtor_tab16[0x10000], vtog_tab16[0x10000];
	int utog_tab16[0x10000], utob_tab16[0x10000];
	int *vtor16, *vtog16, *utog16, *utob16;

	float v16tor_float_tab[0x10000], v16tog_float_tab[0x10000];
	float u16tog_float_tab[0x10000], u16tob_float_tab[0x10000];
	float *v16tor_float, *v16tog_float, *u16tog_float, *u16tob_float;
} cmodel_yuv_t;

cmodel_yuv_t *yuv_table = 0;

// Compression coefficients straight out of jpeglib
#define R_TO_Y    0.29900
#define G_TO_Y    0.58700
#define B_TO_Y    0.11400

#define R_TO_U    -0.16874
#define G_TO_U    -0.33126
#define B_TO_U    0.50000

#define R_TO_V    0.50000
#define G_TO_V    -0.41869
#define B_TO_V    -0.08131

// Decompression coefficients straight out of jpeglib
#define V_TO_R    1.40200
#define V_TO_G    -0.71414

#define U_TO_G    -0.34414
#define U_TO_B    1.77200

void cmodel_init_yuv(cmodel_yuv_t *yuv_table)
{
	int i;

	/* compression */
	for(i = 0; i < 0x100; i++)
	{
		yuv_table->rtoy_tab[i] = (int)(R_TO_Y * 0x10000 * i);
		yuv_table->rtou_tab[i] = (int)(R_TO_U * 0x10000 * i);
		yuv_table->rtov_tab[i] = (int)(R_TO_V * 0x10000 * i);

		yuv_table->gtoy_tab[i] = (int)(G_TO_Y * 0x10000 * i);
		yuv_table->gtou_tab[i] = (int)(G_TO_U * 0x10000 * i);
		yuv_table->gtov_tab[i] = (int)(G_TO_V * 0x10000 * i);

		yuv_table->btoy_tab[i] = (int)(B_TO_Y * 0x10000 * i);
		yuv_table->btou_tab[i] = (int)(B_TO_U * 0x10000 * i) + 0x800000;
		yuv_table->btov_tab[i] = (int)(B_TO_V * 0x10000 * i) + 0x800000;
	}

	/* compression */
	for(i = 0; i < 0x10000; i++)
	{
		yuv_table->rtoy_tab16[i] = (int)(R_TO_Y * 0x100 * i);
		yuv_table->rtou_tab16[i] = (int)(R_TO_U * 0x100 * i);
		yuv_table->rtov_tab16[i] = (int)(R_TO_V * 0x100 * i);

		yuv_table->gtoy_tab16[i] = (int)(G_TO_Y * 0x100 * i);
		yuv_table->gtou_tab16[i] = (int)(G_TO_U * 0x100 * i);
		yuv_table->gtov_tab16[i] = (int)(G_TO_V * 0x100 * i);

		yuv_table->btoy_tab16[i] = (int)(B_TO_Y * 0x100 * i);
		yuv_table->btou_tab16[i] = (int)(B_TO_U * 0x100 * i) + 0x800000;
		yuv_table->btov_tab16[i] = (int)(B_TO_V * 0x100 * i) + 0x800000;
	}




	/* decompression */
	yuv_table->vtor = &(yuv_table->vtor_tab[0x80]);
	yuv_table->vtog = &(yuv_table->vtog_tab[0x80]);
	yuv_table->utog = &(yuv_table->utog_tab[0x80]);
	yuv_table->utob = &(yuv_table->utob_tab[0x80]);
	for(i = -0x80; i < 0x80; i++)
	{
		yuv_table->vtor[i] = (int)(V_TO_R * 0x10000 * i);
		yuv_table->vtog[i] = (int)(V_TO_G * 0x10000 * i);

		yuv_table->utog[i] = (int)(U_TO_G * 0x10000 * i);
		yuv_table->utob[i] = (int)(U_TO_B * 0x10000 * i);
	}


	/* decompression */
	yuv_table->vtor_float = &(yuv_table->vtor_float_tab[0x80]);
	yuv_table->vtog_float = &(yuv_table->vtog_float_tab[0x80]);
	yuv_table->utog_float = &(yuv_table->utog_float_tab[0x80]);
	yuv_table->utob_float = &(yuv_table->utob_float_tab[0x80]);
	for(i = -0x80; i < 0x80; i++)
	{
		yuv_table->vtor_float[i] = V_TO_R * i / 0xff;
		yuv_table->vtog_float[i] = V_TO_G * i / 0xff;

		yuv_table->utog_float[i] = U_TO_G * i / 0xff;
		yuv_table->utob_float[i] = U_TO_B * i / 0xff;
	}


	/* decompression */
	yuv_table->vtor16 = &(yuv_table->vtor_tab16[0x8000]);
	yuv_table->vtog16 = &(yuv_table->vtog_tab16[0x8000]);
	yuv_table->utog16 = &(yuv_table->utog_tab16[0x8000]);
	yuv_table->utob16 = &(yuv_table->utob_tab16[0x8000]);
	for(i = -0x8000; i < 0x8000; i++)
	{
		yuv_table->vtor16[i] = (int)(V_TO_R * 0x100 * i);
		yuv_table->vtog16[i] = (int)(V_TO_G * 0x100 * i);

		yuv_table->utog16[i] = (int)(U_TO_G * 0x100 * i);
		yuv_table->utob16[i] = (int)(U_TO_B * 0x100 * i);
	}


	/* decompression */
	yuv_table->v16tor_float = &(yuv_table->v16tor_float_tab[0x8000]);
	yuv_table->v16tog_float = &(yuv_table->v16tog_float_tab[0x8000]);
	yuv_table->u16tog_float = &(yuv_table->u16tog_float_tab[0x8000]);
	yuv_table->u16tob_float = &(yuv_table->u16tob_float_tab[0x8000]);
	for(i = -0x8000; i < 0x8000; i++)
	{
		yuv_table->v16tor_float[i] = V_TO_R * i / 0xffff;
		yuv_table->v16tog_float[i] = V_TO_G * i / 0xffff;

		yuv_table->u16tog_float[i] = U_TO_G * i / 0xffff;
		yuv_table->u16tob_float[i] = U_TO_B * i / 0xffff;
	}
}



#undef CLAMP
#define CLAMP(x, y, z) ((x) = ((x) < (y) ? (y) : ((x) > (z) ? (z) : (x))))

#undef CLIP
#define CLIP(x, y, z) ((x) < (y) ? (y) : ((x) > (z) ? (z) : (x)))

// All variables are unsigned
// y -> 24 bits u, v, -> 8 bits r, g, b -> 8 bits
#define YUV_TO_RGB(y, u, v, r, g, b) \
{ \
	(r) = ((y + yuv_table->vtor_tab[v]) >> 16); \
	(g) = ((y + yuv_table->utog_tab[u] + yuv_table->vtog_tab[v]) >> 16); \
	(b) = ((y + yuv_table->utob_tab[u]) >> 16); \
	CLAMP(r, 0, 0xff); \
	CLAMP(g, 0, 0xff); \
	CLAMP(b, 0, 0xff); \
}

/* Blatantly stolen from
 * http://prdownloads.sourceforge.net/heroines/mix2005-1.2-src.tar.bz2
 *
 * LGPL (Lesser GPL)
 */
static inline void transfer_YUV_PLANAR_to_BGR888(unsigned char *(*output),
												 unsigned char *input_y,
												 unsigned char *input_u,
												 unsigned char *input_v)
{
	int y, u, v;
	int r, g, b;

	y = (*input_y << 16) | (*input_y << 8) | *input_y;
	u = *input_u;
	v = *input_v;
	YUV_TO_RGB(y, u, v, r, g, b)

		(*output)[0] = b;
	(*output)[1] = g;
	(*output)[2] = r;
	(*output) += 3;
}

static inline void transfer_YUV_PLANAR_to_BGR565(unsigned char *(*output),
												 unsigned char *input_y,
												 unsigned char *input_u,
												 unsigned char *input_v)
{
	int y, u, v;
	int r, g, b;

	y = (*input_y << 16) | (*input_y << 8) | *input_y;
	u = *input_u;
	v = *input_v;
	YUV_TO_RGB(y, u, v, r, g, b)

		*(uint16_t*)(*output) = ((b & 0xf8) << 8)
		+ ((g & 0xfc) << 3)
		+ ((r & 0xf8) >> 3);
	(*output) += 2;
}


static inline void transfer_YUV422_to_BGR888(unsigned char *(*output),
											 unsigned char *input,
											 int column)
{
	int y, u, v;
	int r, g, b;

	// Even pixel
	if(!(column & 1))
		y = (int)(input[0]) << 16;
	else
		// Odd pixel
		y = (int)(input[2]) << 16;
	u = input[1];
	v = input[3];
	YUV_TO_RGB(y, u, v, r, g, b)

		(*output)[0] = b;
	(*output)[1] = g;
	(*output)[2] = r;
	(*output) += 3;
}

//////////////////////////////////////////////////////////////////////////

/*To make sure that you are bounding your inputs in the range of 0 & 255*/
#define SATURATE8(x) ((unsigned int) x <= 255 ? x : (x < 0 ? 0: 255))

inline static
void icvYV12toBGR888(char *videoBuffer, char * buffer_rgb, unsigned int Height, unsigned int Width)
{
	char  *buffer_rgb_1strow, *buffer_rgb_2ndrow;
	unsigned char y_start, u_start, v_start;

	unsigned char * videoBuffer_1strow,*videoBuffer_2ndrow;
	int r,g,b, r_prod, g_prod, b_prod;
	int  y,u,v;

	unsigned int i_conv_ht =0,i_conv_wt =0;
	unsigned int y_size;
	unsigned int widthStride = Width * 3;

	char * cb_ptr;
	char * cr_ptr;

	/*Now do the conversion from YUV420  to BGR 888 */
	/*So allocate that many amount of buffer*/
	/*allocate memory to contain the one frame for RGB output:*/

	y_size = Width*Height;

	/*Calculate the Cb & Cr pointer*/
	//cb_ptr = videoBuffer + y_size;
	//cr_ptr = cb_ptr + y_size / 4;

	cr_ptr = videoBuffer + y_size;
	cb_ptr = cr_ptr + y_size / 4;

	videoBuffer_1strow = (unsigned char*)videoBuffer;
	videoBuffer_2ndrow = (unsigned char*)videoBuffer+Width;

	buffer_rgb_1strow = buffer_rgb ;
	buffer_rgb_2ndrow = buffer_rgb + widthStride;

	for (i_conv_ht =0; i_conv_ht<Height;i_conv_ht+=2)
	{
		for (i_conv_wt =0; i_conv_wt<Width;i_conv_wt+=2)

		{
			/*For a YUV 420 data with width & height is 4x6*/
			/*Following is the pattern it was stored*/
			/*Y1  Y2  Y3  Y4  Y5  Y6*/
			/*Y7  Y8  Y9  Y10 Y11 Y12*/
			/*Y13 Y14 Y15 Y16 Y17 Y18*/
			/*Y19 Y20 Y21 Y22 Y23 Y24*/
			/*U1  U2  U3  U4  U5  U6*/
			/*V1  V2  V3  V4  V5  V6*/
			/*Please note Y1 Y2 Y7 Y8 will be sharing U1 and V1*/
			/*&*/
			/*Y3 Y4 Y9 Y10 will be sharing U2 V2*/
			/*Following is the plan*/
			/*We know that Y1 & Y2 is going to pair with the first U (say U1) & V (say V1) */
			/*That means trigger the movement of cb & cr pointer*/
			/*only for 2 y movement*/

			/*Extract the Y value*/
			y_start = *videoBuffer_1strow++;
			y = (y_start -16)*298;

			/*extract  the value*/
			u_start = *cb_ptr++;
			v_start = *cr_ptr++;

			/*precalculate it*/
			u = u_start - 128;
			v = v_start - 128;

			r_prod = 409*v + 128;
			g_prod = 100*u + 208*v - 128;
			b_prod = 516*u;

			/*!!! now it is time to do the conversion*/
			r =(y + r_prod)>>8;
			g =(y - g_prod)>>8;
			b =(y + b_prod)>>8;

			/*Now clip and store */
			*buffer_rgb_1strow++ = SATURATE8(b);
			*buffer_rgb_1strow++ = SATURATE8(g);
			*buffer_rgb_1strow++ = SATURATE8(r);

			/*Extract the Y value*/
			y_start = *videoBuffer_1strow++;
			y = (y_start -16)*298;

			/*!!! now it is time to do the conversion*/
			r =(y + r_prod)>>8;
			g =(y - g_prod)>>8;
			b =(y + b_prod)>>8;

			/*Now clip and store */

			*buffer_rgb_1strow++ = SATURATE8(b);
			*buffer_rgb_1strow++ = SATURATE8(g);
			*buffer_rgb_1strow++ = SATURATE8(r);

			/*Extract the Y value*/
			y_start = *videoBuffer_2ndrow++;
			y = (y_start -16)*298;

			/*!!! now it is time to do the conversion*/
			r =(y + r_prod)>>8;
			g =(y - g_prod)>>8;
			b =(y + b_prod)>>8;

			/*Now clip and store */
			*buffer_rgb_2ndrow++ = SATURATE8(b);
			*buffer_rgb_2ndrow++ = SATURATE8(g);
			*buffer_rgb_2ndrow++ = SATURATE8(r);

			/*Extract the Y value*/
			y_start = *videoBuffer_2ndrow++;
			y = (y_start -16)*298;

			/*!!! now it is time to do the conversion*/
			r =(y + r_prod)>>8;
			g =(y - g_prod)>>8;
			b =(y + b_prod)>>8;


			/*Now clip and store */
			*buffer_rgb_2ndrow++ = SATURATE8(b);
			*buffer_rgb_2ndrow++ = SATURATE8(g);
			*buffer_rgb_2ndrow++ = SATURATE8(r);
		}
		videoBuffer_1strow += Width;
		videoBuffer_2ndrow += Width;

		buffer_rgb_1strow += widthStride;
		buffer_rgb_2ndrow += widthStride;
	}
}

inline static
void icvVY12toBGR888(char *videoBuffer, char * buffer_rgb, unsigned int Height, unsigned int Width)
{
	char  *buffer_rgb_1strow, *buffer_rgb_2ndrow;
	unsigned char y_start, u_start, v_start;

	unsigned char * videoBuffer_1strow,*videoBuffer_2ndrow;
	int r,g,b, r_prod, g_prod, b_prod;
	int  y,u,v;

	unsigned int i_conv_ht =0,i_conv_wt =0;
	unsigned int y_size;

	char * cb_ptr;
	char * cr_ptr;

	/*Now do the conversion from YUV420  to BGR 888 */
	/*So allocate that many amount of buffer*/
	/*allocate memory to contain the one frame for RGB output:*/

	y_size = Width*Height;

	/*Calculate the Cb & Cr pointer*/
	cb_ptr = videoBuffer + y_size;
	cr_ptr = cb_ptr + y_size / 4;

	videoBuffer_1strow = (unsigned char*)videoBuffer;
	videoBuffer_2ndrow = (unsigned char*)videoBuffer+Width;

	buffer_rgb_1strow = buffer_rgb ;
	buffer_rgb_2ndrow = buffer_rgb+(3*Width);

	for (i_conv_ht =0; i_conv_ht<Height;i_conv_ht+=2)
	{
		for (i_conv_wt =0; i_conv_wt<Width;i_conv_wt+=2)

		{
			/*For a YUV 420 data with width & height is 4x6*/
			/*Following is the pattern it was stored*/
			/*Y1  Y2  Y3  Y4  Y5  Y6*/
			/*Y7  Y8  Y9  Y10 Y11 Y12*/
			/*Y13 Y14 Y15 Y16 Y17 Y18*/
			/*Y19 Y20 Y21 Y22 Y23 Y24*/
			/*U1  U2  U3  U4  U5  U6*/
			/*V1  V2  V3  V4  V5  V6*/
			/*Please note Y1 Y2 Y7 Y8 will be sharing U1 and V1*/
			/*&*/
			/*Y3 Y4 Y9 Y10 will be sharing U2 V2*/
			/*Following is the plan*/
			/*We know that Y1 & Y2 is going to pair with the first U (say U1) & V (say V1) */
			/*That means trigger the movement of cb & cr pointer*/
			/*only for 2 y movement*/

			/*Extract the Y value*/
			y_start = *videoBuffer_1strow++;
			y = (y_start -16)*298;

			/*extract  the value*/
			u_start = *cb_ptr++;
			v_start = *cr_ptr++;

			/*precalculate it*/
			u = u_start - 128;
			v = v_start - 128;

			r_prod = 409*v + 128;
			g_prod = 100*u + 208*v - 128;
			b_prod = 516*u;

			/*!!! now it is time to do the conversion*/
			r =(y + r_prod)>>8;
			g =(y - g_prod)>>8;
			b =(y + b_prod)>>8;

			YUV_TO_RGB(y, u, v, r, g, b)

				/*Now clip and store */
				*buffer_rgb_1strow++ = SATURATE8(b);
			*buffer_rgb_1strow++ = SATURATE8(g);
			*buffer_rgb_1strow++ = SATURATE8(r);

			/*Extract the Y value*/
			y_start = *videoBuffer_1strow++;
			y = (y_start -16)*298;

			/*!!! now it is time to do the conversion*/
			r =(y + r_prod)>>8;
			g =(y - g_prod)>>8;
			b =(y + b_prod)>>8;

			/*Now clip and store */

			*buffer_rgb_1strow++ = SATURATE8(b);
			*buffer_rgb_1strow++ = SATURATE8(g);
			*buffer_rgb_1strow++ = SATURATE8(r);

			/*Extract the Y value*/
			y_start = *videoBuffer_2ndrow++;
			y = (y_start -16)*298;

			/*!!! now it is time to do the conversion*/
			r =(y + r_prod)>>8;
			g =(y - g_prod)>>8;
			b =(y + b_prod)>>8;

			/*Now clip and store */
			*buffer_rgb_2ndrow++ = SATURATE8(b);
			*buffer_rgb_2ndrow++ = SATURATE8(g);
			*buffer_rgb_2ndrow++ = SATURATE8(r);

			/*Extract the Y value*/
			y_start = *videoBuffer_2ndrow++;
			y = (y_start -16)*298;

			/*!!! now it is time to do the conversion*/
			r =(y + r_prod)>>8;
			g =(y - g_prod)>>8;
			b =(y + b_prod)>>8;


			/*Now clip and store */
			*buffer_rgb_2ndrow++ = SATURATE8(b);
			*buffer_rgb_2ndrow++ = SATURATE8(g);
			*buffer_rgb_2ndrow++ = SATURATE8(r);
		}
		videoBuffer_1strow += Width;
		videoBuffer_2ndrow += Width;

		buffer_rgb_1strow += 3*Width;
		buffer_rgb_2ndrow += 3*Width;
	}
}

void __noyet__(const IplImage* rgb_in, IplImage* rgb_out)
{

	for (int v = 0; v < 320/*rgb_in->height*/;v++)
		for (int u = 0; u < 320/*rgb_in->width*/; u++)
		{
			rgb_out->imageData[u * rgb_out->widthStep + v + 0] = 0xFF;//rgb_in->imageData[v * rgb_in->widthStep + u + 0];
			rgb_out->imageData[u * rgb_out->widthStep + v + 1] = 0;//rgb_in->imageData[v * rgb_in->widthStep + u + 1];
			rgb_out->imageData[u * rgb_out->widthStep + v + 2] = 0;//rgb_in->imageData[v * rgb_in->widthStep + u + 2];

			//std::swap(rgb_out->imageData[u * rgb_out->widthStep + v + 0], rgb_in->imageData[v * rgb_in->widthStep + u + 0]);
			//std::swap(rgb_out->imageData[u * rgb_out->widthStep + v + 1], rgb_in->imageData[v * rgb_in->widthStep + u + 1]);
			//std::swap(rgb_out->imageData[u * rgb_out->widthStep + v + 2], rgb_in->imageData[v * rgb_in->widthStep + u + 2]);
		}

#if 0
	//int half = cvFloor(rgb->nSize / 2.);
	//for (int u = half; u < 0; u--)
	//{
	//}

	char* dst = rgb->imageData;
	char* src = rgb->imageData;
	int dst_widthStep = rgb->widthStep;
	int src_widthStep = rgb->widthStep;
	int height = 100;
	int width = 100;

	 // 90 degrees CCW by blocks
	 int blockSize = 64;
	 for (int i = 0; i < width / blockSize + 1; i++)
		for (int j = 0; j < height / blockSize + 1; j++)
			for (int x = i * blockSize; x < min((i + 1) * blockSize, width); x++)
				for (int y = j * blockSize; y < min((j + 1) * blockSize, height); y++)
				{
					std::swap(dst[(width - x - 1) * dst_widthStep + y],
						src[y * src_widthStep + x]);
				}

#endif
}




//////////////////////////////////////////////////////////////////////////

/* color conversion functions from Bart Nabbe. */
/* corrected by Damien: bad coeficients in YUV2RGB */
#define YUV2RGB(y, u, v, r, g, b)\
	r = y + ((v*1436) >> 10);\
	g = y - ((u*352 + v*731) >> 10);\
	b = y + ((u*1814) >> 10);\
	r = r < 0 ? 0 : r;\
	g = g < 0 ? 0 : g;\
	b = b < 0 ? 0 : b;\
	r = r > 255 ? 255 : r;\
	g = g > 255 ? 255 : g;\
	b = b > 255 ? 255 : b

static inline void icvUYVYtoBGR( const IplImage* inputSrc, IplImage* destBgr, unsigned char* inputData )
{
	unsigned int NumPixels = inputSrc->width * inputSrc->height;

	const unsigned char *src = inputData;
	unsigned char *dest = (unsigned char*)destBgr->imageData;

	register int i = (NumPixels << 1) - 1;
	register int j = NumPixels + (NumPixels << 1) - 1;
	register int y0, y1, u, v;
	register int r, g, b;

	while (i > 0) {
		y1 = src[i--];
		v = src[i--] - 128;
		y0 = src[i--];
		u = src[i--] - 128;
		YUV2RGB(y1, u, v, r, g, b);
		dest[j--] = r;
		dest[j--] = g;
		dest[j--] = b;
		YUV2RGB(y0, u, v, r, g, b);
		dest[j--] = r;
		dest[j--] = g;
		dest[j--] = b;
	}
}

// 4:2:2 interleaved -> BGR
static inline void icvYUY2toBGR( const IplImage* inputSrc, IplImage* bgr_frame, uint8_t* inputData )
{

	unsigned int NumPixels = inputSrc->width * inputSrc->height;

	const unsigned char *src = inputData;
	unsigned char *dest = (unsigned char*)bgr_frame->imageData;

	register int i = (NumPixels << 1) - 1;
	register int j = NumPixels + (NumPixels << 1) - 1;
	register int y0, y1, u, v;
	register int r, g, b;

	while (i > 0) {
		v = src[i--] - 128;
		y1 = src[i--];
		u = src[i--] - 128;
		y0 = src[i--];
		YUV2RGB(y1, u, v, r, g, b);
		dest[j--] = r;
		dest[j--] = g;
		dest[j--] = b;
		YUV2RGB(y0, u, v, r, g, b);
		dest[j--] = r;
		dest[j--] = g;
		dest[j--] = b;
	}
}

#endif


//////////////////////////////////////////////////////////////////////////

STDAPI CreateMemoryAllocator( IMemAllocator **ppAllocator )
{
	return CoCreateInstance(CLSID_MemoryAllocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IMemAllocator,
		(void **)ppAllocator);
}


/****************************************************************************
* Implementation of our dummy directshow filter class
****************************************************************************/
CaptureFilter::CaptureFilter( vx_source_dshow* handle, AM_MEDIA_TYPE *mt, size_t mt_count ) :
	_capturepin(NULL),
	_state( State_Stopped ),
	i_ref( 1 ),
	_handle(handle),
	_filtergraph(NULL),
	_clock(NULL)
{

#if 0
	if (0 == yuv_table)
	{
		yuv_table = (cmodel_yuv_t*)malloc(sizeof(cmodel_yuv_t));
		cmodel_init_yuv(yuv_table);
	}
#endif
	size_t mediatypes_count = 2;
	AM_MEDIA_TYPE *mediatypes = NULL;

	mediatypes = (AM_MEDIA_TYPE *)CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE) * mediatypes_count);

	mediatypes[0].majortype = MEDIATYPE_Video;
	mediatypes[0].subtype   = MEDIASUBTYPE_NULL;// MEDIASUBTYPE_RGB24;
	mediatypes[0].pbFormat  = 0;
	mediatypes[0].cbFormat  = 0;
	mediatypes[0].pUnk      = 0;

	// needed for Windows Mobile
	mediatypes[1].majortype = MEDIATYPE_Video;
	mediatypes[1].subtype  =  MEDIASUBTYPE_RGB565;
	mediatypes[1].pbFormat  = 0;
	mediatypes[1].cbFormat  = 0;
	mediatypes[1].pUnk      = 0;


	_capturepin = new CapturePin( handle, this, mediatypes, mediatypes_count );
}

CaptureFilter::~CaptureFilter()
{
	_capturepin->Release();
}

/* IUnknown methods */
STDMETHODIMP CaptureFilter::QueryInterface( REFIID riid, void **ppv )
{
	SSTT_DS_DEBUG("CaptureFilter::QueryInterface - In");

	if( IsEqualIID(riid , IID_IUnknown ) ) {
		*ppv = static_cast<IUnknown*>(this);
	} else
	if( IsEqualIID(riid , IID_IPersist ) ) {
		*ppv = static_cast<IPersist*>(this);
	} else
	if( IsEqualIID(riid , IID_IMediaFilter ) ) {
		*ppv = static_cast<IMediaFilter*>(this);
	} else
	if( IsEqualIID(riid , IID_IBaseFilter ) ) {
		*ppv = static_cast<IBaseFilter*>(this);
	} else {

#if defined(SSTT_VERBOSE_DEBUG)
    fprintf(stderr,"%s unknown filter: 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",
        __FUNCTION__,
        riid.Data1,riid.Data2,riid.Data3,
        riid.Data4[0],riid.Data4[1],riid.Data4[2],riid.Data4[3],riid.Data4[4],riid.Data4[5],riid.Data4[6],riid.Data4[7]);
#endif

		SSTT_DS_DEBUG("CaptureFilter::QueryInterface - Fail");

		*ppv = NULL;
		return E_NOINTERFACE;

	}

	SSTT_DS_DEBUG("CaptureFilter::QueryInterface - Ok ");

	this->AddRef();

	return S_OK;

}

STDMETHODIMP_(ULONG) CaptureFilter::AddRef()
{
	InterlockedIncrement(&i_ref);

	return i_ref;
}


STDMETHODIMP_(ULONG) CaptureFilter::Release()
{
	if( !InterlockedDecrement(&i_ref) ) delete this;

	return 0;
}

/* IPersist method */
STDMETHODIMP CaptureFilter::GetClassID(CLSID *pClsID)
{
	SSTT_DS_DEBUG("CaptureFilter::GetClassID");

	return E_NOTIMPL;
};

/* IMediaFilter methods */
STDMETHODIMP CaptureFilter::GetState(DWORD dwMSecs, FILTER_STATE *State)
{

	SSTT_DS_DEBUG("CaptureFilter::GetState");

	*State = _state;

	return S_OK;
}

STDMETHODIMP CaptureFilter::SetSyncSource(IReferenceClock *pClock)
{
	SSTT_DS_DEBUG("CaptureFilter::SetSyncSource");

	if (pClock) {
		pClock->AddRef();
	}

	if (_clock) {
		_clock->Release();
	}

	_clock = pClock;

	return S_OK;
}

STDMETHODIMP CaptureFilter::GetSyncSource(IReferenceClock **pClock)
{
	SSTT_DS_DEBUG("CaptureFilter::GetSyncSource");

	if (_clock) {
		_clock->AddRef();
	}

	*pClock = (IReferenceClock*)_clock;

	return S_OK;
}

STDMETHODIMP CaptureFilter::Stop()
{

	SSTT_DS_DEBUG("CaptureFilter::Stop");

	//_capturepin->EndFlush();

	if (_state != State_Stopped)
	{
		//IPin* pin = this->CustomGetPin();

		//if (pin)
		//{
		//	//pin->Inactive();
		//}
	}

	_state = State_Stopped;
	return S_OK;
}

STDMETHODIMP CaptureFilter::Pause()
{

	SSTT_DS_DEBUG("CaptureFilter::Pause");

	_state = State_Paused;
	return S_OK;
}

STDMETHODIMP CaptureFilter::Run(REFERENCE_TIME tStart)
{
	SSTT_DS_DEBUG("CaptureFilter::Run");

	if (_state == State_Stopped)
	{
		HRESULT hr = this->Pause();
		if (FAILED(hr)) {
			return hr;
		}
	}

	IPin* pin = CustomGetPin();

	if (pin)
	{
		SSTT_DS_DEBUG("CaptureFilter::Run - Pin found");

		//PIN_INFO p_info;
		//IPin->QueryPinInfo(&p_info);

		//p_info.achName


	} else
	{
		SSTT_DS_DEBUG("CaptureFilter::Run - Pin *not* found");
	}

	_state = State_Running;
	return S_OK;
}

/* IBaseFilter methods */
STDMETHODIMP CaptureFilter::EnumPins( IEnumPins ** ppEnum )
{

	SSTT_DS_DEBUG("CaptureFilter::EnumPins");

	/* Create a new ref counted enumerator */

	*ppEnum = new CaptureEnumPins( _handle, this, NULL );

	return *ppEnum == NULL ? E_OUTOFMEMORY : S_OK;
}

STDMETHODIMP CaptureFilter::FindPin( LPCWSTR Id, IPin ** ppPin )
{
	SSTT_DS_DEBUG("CaptureFilter::FindPin");

	return E_NOTIMPL;
}


STDMETHODIMP CaptureFilter::QueryFilterInfo( FILTER_INFO * pInfo )
{

	SSTT_DS_DEBUG("CaptureFilter::QueryFilterInfo");

	pInfo->achName[0] = L'\0';


	pInfo->pGraph = _filtergraph;
	if ( _filtergraph ) _filtergraph->AddRef();


	return NOERROR;
};
STDMETHODIMP CaptureFilter::JoinFilterGraph( IFilterGraph * pGraph,
											LPCWSTR pName )
{
	SSTT_DS_DEBUG("CaptureFilter::JoinFilterGraph");


	_filtergraph = pGraph;

	return NOERROR;
};
STDMETHODIMP CaptureFilter::QueryVendorInfo( LPWSTR* pVendorInfo )
{
	SSTT_DS_DEBUG("CaptureFilter::QueryVendorInfo");
	return E_NOTIMPL;
};

/* Custom methods */
CapturePin *CaptureFilter::CustomGetPin()
{
	SSTT_DS_DEBUG("CaptureFilter::CustomGetPin");
	return _capturepin;
}

STDMETHODIMP CaptureFilter::Register()
{
	SSTT_DS_DEBUG("CaptureFilter::Register");

	return S_OK;
}

STDMETHODIMP CaptureFilter::Unregister()
{
	SSTT_DS_DEBUG("CaptureFilter::Unregister");

	return S_OK;
}

CapturePin::CapturePin(
					   vx_source_dshow* handle,
					   IBaseFilter* capturefilter,
					   AM_MEDIA_TYPE *mediatypes,
					   size_t mediatypes_count )
	: _refcount(1),
	_connected_pin(NULL),
	_filter(capturefilter),
	_handle(handle),
	_mediatypes(mediatypes),
	_mediatype_count(mediatypes_count),
	_allocator(NULL)
{

	InitializeCriticalSection(&_cs);

	SSTT_DS_DEBUG("CapturePin::CapturePin");

	_connected_mediatype.majortype = mediatypes[0].majortype;
	_connected_mediatype.subtype   = mediatypes[0].subtype;
	_connected_mediatype.pbFormat  = 0L;
	_connected_mediatype.cbFormat  = 0L;
	_connected_mediatype.pUnk      = 0L;

}

STDMETHODIMP CapturePin::QueryInterface( REFIID riid, void **ppv )
{
	SSTT_DS_DEBUG("CapturePin::QueryInterface");


	if( riid == IID_IUnknown ||
		riid == IID_IPin )
	{
		AddRef();
		*ppv = (IPin *)this;
		return NOERROR;
	}
	if( riid == IID_IMemInputPin )
	{
		AddRef();
		*ppv = (IMemInputPin *)this;
		return NOERROR;
	} else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	return E_NOTIMPL;
}



ULONG CapturePin::AddRef()
{
	InterlockedIncrement(&_refcount);
	return _refcount;
}

ULONG CapturePin::Release()
{
	if (!InterlockedDecrement(&_refcount)) delete this;

	return _refcount;
}

STDMETHODIMP CapturePin::Connect( IPin * pReceivePin, const AM_MEDIA_TYPE *pmt )
{

	SSTT_DS_DEBUG("CapturePin::Connect - In");


	if (static_cast<CaptureFilter*>(_filter)->_state != State_Stopped)
	{
		SSTT_DS_DEBUG("CapturePin::Connect - Graph already running");
		return VFW_E_NOT_STOPPED;
	}

	if( pmt->pbFormat && pmt->majortype == MEDIATYPE_Video  )
	{

		if( pmt->pbFormat &&
			( (((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biHeight == 0) ||
			(((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biWidth == 0) ) )
		{
			SSTT_DS_DEBUG("CapturePin::Connect - Bogus Format");
			return S_FALSE;
		}


		//if( !((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biHeight ||
		//	!((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biWidth )
		//{
		//	/*msg_Dbg( p_input, "CapturePin::Connect "
		//	"[video width/height == 0 ]" );*/
		//	return S_FALSE;
		//}
	}

	SSTT_DS_DEBUG("CapturePin::Connect - Out");

	return S_OK;
}

STDMETHODIMP CapturePin::ReceiveConnection( IPin * pConnector, const AM_MEDIA_TYPE *pmt )
{

	SSTT_DS_DEBUG("CapturePin::ReceiveConnection - In");

	if( !pConnector || !pmt )
		return E_POINTER;

	if( NULL != _connected_pin )
	{
		SSTT_DS_DEBUG("CapturePin::ReceiveConnection - Already Connected");

		return VFW_E_ALREADY_CONNECTED;
	}

	if( S_OK != QueryAccept(pmt) )
	{
		SSTT_DS_DEBUG("CapturePin::ReceiveConnection - Query not accepted");

		return VFW_E_TYPE_NOT_ACCEPTED;
	}

	SSTT_DS_DEBUG("CapturePin::ReceiveConnection - Ok");

	_connected_pin = pConnector;
	_connected_pin->AddRef();

	uFreeMediaType( _connected_mediatype );
	return uCopyMediaType( &_connected_mediatype, pmt );

}

STDMETHODIMP CapturePin::Disconnect()
{

	SSTT_DS_DEBUG("CapturePin::Disconnect - In");

	if (! _connected_pin)
	{

		SSTT_DS_DEBUG("CapturePin::Disconnect - Pin not connected");

		return S_FALSE;
	}

	_connected_pin->Release();
	_connected_pin = NULL;

	SSTT_DS_DEBUG("CapturePin::Disconnect - Ok");

	return S_OK;
}

STDMETHODIMP CapturePin::ConnectedTo( IPin **pPin )
{

	SSTT_DS_DEBUG("CapturePin::ConnectedTo");

	if( !_connected_pin )
	{
		SSTT_DS_DEBUG("CapturePin::ConnectedTo - Not connected to query");
		return VFW_E_NOT_CONNECTED;
	}

	_connected_pin->AddRef();
	*pPin = _connected_pin;

	return S_OK;
}

STDMETHODIMP CapturePin::ConnectionMediaType( AM_MEDIA_TYPE *pmt )
{
	SSTT_DS_DEBUG("CapturePin::ConnectionMediaType");

	if( !_connected_pin )
		return VFW_E_NOT_CONNECTED;

	return uCopyMediaType( pmt, &_connected_mediatype);
}

STDMETHODIMP CapturePin::QueryPinInfo( PIN_INFO * pInfo )
{
	SSTT_DS_DEBUG("CapturePin::QueryPinInfo - In");

	pInfo->pFilter = _filter;
	if ( _filter )
		_filter->AddRef();

  //  if (pInfo->achName) 
		//memcpy(pInfo->achName,L'\0',sizeof(WCHAR));

	pInfo->dir = PINDIR_INPUT;

	SSTT_DS_DEBUG("CapturePin::QueryPinInfo - Out");

	return S_OK;
}

STDMETHODIMP CapturePin::QueryDirection( PIN_DIRECTION * pPinDir )
{

	SSTT_DS_DEBUG("CapturePin::QueryDirection");

	*pPinDir = PINDIR_INPUT;
	return S_OK;
}

STDMETHODIMP CapturePin::QueryId( LPWSTR * Id )
{
	SSTT_DS_DEBUG("CapturePin::QueryId");


    *Id = L"libVX Capture Pin";

	return S_OK;
}

STDMETHODIMP CapturePin::QueryAccept( const AM_MEDIA_TYPE *pmt )
{

	SSTT_DS_DEBUG("CapturePin::QueryAccept - In");

	if( pmt->majortype == MEDIATYPE_Video )
	{
		std::string cs_name = "NONE";

		if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_YV12))
		{
			cs_name = "YV12";
		} else
		if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_RGB565))
		{
			cs_name = "RGB565";
		} else
		if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_Y411))
		{
			cs_name = "Y411";
		} else
		if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_YUY2))
		{
			cs_name = "YUY2";
		} else
		if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_MJPG))
		{

			SSTT_DS_DEBUG("CapturePin::QueryAccept - Can't handle MJPEG");

			return S_FALSE;

			//MessageBox(0,"MJPEG","Command",MB_OK | MB_ICONINFORMATION);

		} else {

			SSTT_DS_DEBUG("CapturePin::QueryAccept - Unhandled format!");

		}

		SSTT_DS_DEBUG("CapturePin::QueryAccept - Intermediate");

		if (0) //_handle->_enumerating)
		{
			SSTT_DS_DEBUG("CapturePin::QueryAccept - Enumeration Mode");

#if defined(SSTT_VERBOSE_DEBUG)
			fprintf(stdout,"%s\nFormat\nwidth=%ld, height=%ld, fps=%f, bitcount=%d, planes=%d (%s)\n\n",
				__FUNCTION__,
				((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biWidth,
				((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biHeight,
				10000000.0f/((float)((VIDEOINFOHEADER *)pmt->pbFormat)->AvgTimePerFrame),
				((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biBitCount,
				((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biPlanes,
				cs_name.c_str()
				);
#endif

			return S_FALSE;
		}

		if( pmt->pbFormat &&
			( (((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biHeight == 0) ||
			(((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biWidth == 0) ) )
		{
			SSTT_DS_DEBUG("CapturePin::QueryAccept - Bogus mode!");

			return S_FALSE;
		} else {

			int actualWidth = abs(((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biWidth);
			int actualHeight = abs(((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biHeight);

			if ( true == 
                ( ignore_compare(actualWidth, /*_handle->_traits.minWidth*/640) |
                  ignore_compare(actualHeight,/*_handle->_traits.minHeight*/480)))
			{
				//printf("ignore %dx%d\n",actualWidth,actualHeight);

				SSTT_DS_DEBUG("CapturePin::QueryAccept - Skip format");

#if defined(SSTT_VERBOSE_DEBUG)
				fprintf(stdout,"%s Format\nwidth=%ld, height=%ld, fps=%f, bit count=%d, planes=%d\n\n",
					__FUNCTION__,
					((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biWidth,
					((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biHeight,
					10000000.0f/((float)((VIDEOINFOHEADER *)pmt->pbFormat)->AvgTimePerFrame),
					((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biBitCount,
					((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biPlanes
					);
#endif

				return S_FALSE;
			}

			/* we could also just skip the format! */
			SSTT_DS_DEBUG("CapturePin::QueryAccept - Ok");

            if (1)
			{
                //vx_frame f;

                _temp.width = abs(((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biWidth);
                _temp.height = abs(((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biHeight);

				if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_YV12) ||
					//IsEqualGUID(pmt->subtype,MEDIASUBTYPE_I420) ||
					IsEqualGUID(pmt->subtype,MEDIASUBTYPE_IYUV) ||
					IsEqualGUID(pmt->subtype,MEDIASUBTYPE_YUY2) ||
					IsEqualGUID(pmt->subtype,MEDIASUBTYPE_YUYV) ||
					IsEqualGUID(pmt->subtype,MEDIASUBTYPE_UYVY)
					)
                {
                    //cvInitImageHeader(&_temp,size,IPL_DEPTH_8U,3);
				}
#if !defined(_WIN32_WCE)
				if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_ARGB32)) {
                    _temp.colorModel = VX_E_COLOR_ABGR;
                    _temp.stride = _temp.width * ((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biBitCount / 8;
                    //cvInitImageHeader(&_temp,size,IPL_DEPTH_8U,4);
				}
#endif
				if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_RGB24)) {
                    _temp.colorModel = VX_E_COLOR_BGR24;
                    //cvInitImageHeader(&_temp,size,IPL_DEPTH_8U,3);
				}
			}

#if defined(SSTT_VERBOSE_DEBUG)
            fprintf(stdout,"%s - Selected format: width=%ld, height=%ld, fps=%f, bitcount=%d, planes=%d (%s)\n",
				__FUNCTION__,
				((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biWidth,
				((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biHeight,
				10000000.0f/((float)((VIDEOINFOHEADER *)pmt->pbFormat)->AvgTimePerFrame),
				((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biBitCount,
				((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biPlanes,
				cs_name.c_str()
				);
#endif

		}

	} else {
	//	Log::Get().Printf("CapturePin::QueryAccept - Media Format is not a video");
		return S_FALSE;
	}


	if( _connected_pin )
	{
//		Log::Get().Printf("CapturePin::QueryAccept - Freeing up pin");

		uFreeMediaType( _connected_mediatype );
		uCopyMediaType( &_connected_mediatype, pmt );
	}

    _temp.frame = 0;

	SSTT_DS_DEBUG("CapturePin::QueryAccept - Out: Ok");

	return S_OK;

}

STDMETHODIMP CapturePin::EnumMediaTypes( IEnumMediaTypes **ppEnum )
{

	SSTT_DS_DEBUG("CapturePin::EnumMediaTypes");

	*ppEnum = new CaptureEnumMediaTypes( _handle, this, NULL );

	if( *ppEnum == NULL ) return E_OUTOFMEMORY;

	return NOERROR;
}

STDMETHODIMP CapturePin::QueryInternalConnections( IPin* *apPin, ULONG *nPin )
{
	SSTT_DS_DEBUG("CapturePin::QueryInternalConnections");

	return E_NOTIMPL;
}

STDMETHODIMP CapturePin::EndOfStream( void )
{
	SSTT_DS_DEBUG("CapturePin::EndOfStream");
	return S_OK;
}


STDMETHODIMP CapturePin::BeginFlush( void )
{
	SSTT_DS_DEBUG("CapturePin::BeginFlush");

	return S_OK;
}

STDMETHODIMP CapturePin::EndFlush( void )
{
	SSTT_DS_DEBUG("CapturePin::EndFlush");

	return S_OK;
}

STDMETHODIMP CapturePin::NewSegment( REFERENCE_TIME tStart,
									REFERENCE_TIME tStop,
									double dRate )
{
	SSTT_DS_DEBUG("CapturePin::NewSegment");
	return S_OK;
}


STDMETHODIMP CapturePin::GetAllocator( IMemAllocator **ppAllocator )
{
	SSTT_DS_DEBUG("CapturePin::GetAllocator - In");

	if (_allocator == NULL)
	{
		HRESULT hr = CreateMemoryAllocator(&_allocator);
		if (FAILED(hr))
		{

			SSTT_DS_DEBUG("CapturePin::GetAllocator - Failed");

			return hr;
		}

		SSTT_DS_DEBUG("CapturePin::GetAllocator - Created New");
	}

	*ppAllocator = _allocator;

	_allocator->AddRef();

	SSTT_DS_DEBUG("CapturePin::GetAllocator - Ok");


	return S_OK;
}

STDMETHODIMP CapturePin::NotifyAllocator( IMemAllocator *pAllocator,
										 BOOL bReadOnly )
{
	SSTT_DS_DEBUG("CapturePin::NotifyAllocator - In");

	IMemAllocator* old_allocator = _allocator;

	pAllocator->AddRef();

	_allocator = pAllocator;

	if (NULL != old_allocator)
	{
		old_allocator->Release();
	}

	_readonly = bReadOnly;

	return S_OK;

}
STDMETHODIMP CapturePin::GetAllocatorRequirements( ALLOCATOR_PROPERTIES *pProps )
{

	return E_NOTIMPL;


	SSTT_DS_DEBUG("CapturePin::GetAllocatorRequirements - In");

	if (NULL == _allocator)
	{
		SSTT_DS_DEBUG("CapturePin::GetAllocatorRequirements - No allocator yet");

		return E_UNEXPECTED;
	}

	_allocator->GetProperties(&_allocator_properties);

	*pProps = _allocator_properties;

	SSTT_DS_DEBUG("CapturePin::GetAllocatorRequirements - Ok");

	return S_OK;
}


STDMETHODIMP CapturePin::Receive( IMediaSample *pSample )
{

	SSTT_DS_DEBUG("CapturePin::Receive - Entry");

//    fprintf(stdout,"%s 0x%x\n",__FUNCTION__,this->_handle);

	//sstt_autotrylock<sstt_mutex> trylock(_handle->get_parent()->_mutex);

	if (pSample)
	{
		//EnterCriticalSection(&_cs);

		pSample->AddRef();

		BYTE *ppBuffer;

		pSample->GetPointer(&ppBuffer);

        _temp.frame++;
		_temp.data = ppBuffer;

		_vx_send_frame((vx_source*)this->_handle,&_temp);




		//REFERENCE_TIME rr,ll;

		//pSample->GetTime(&rr,&ll);


#if 0
		if (_connected_mediatype.subtype == MEDIASUBTYPE_RGB24)
		{
			/* working properly now */
			cvSetData(&_temp,ppBuffer,_temp.widthStep);
			cvCopy(&_temp,_handle->get_parent()->get_image(SSTT_IMAGE_BGR24));

		} else

#if !defined(_WIN32_WCE)
		if (IsEqualGUID(_connected_mediatype.subtype,MEDIASUBTYPE_ARGB32))
		{
			//CvSize size = cvGetSize(&_temp);
			cvSetImageData(&_temp,ppBuffer,_temp.widthStep);
			cvCvtColor(&_temp,_handle->get_parent()->get_image(SSTT_IMAGE_BGR24),CV_BGRA2BGR);
			
	
		} else
#endif
		if (IsEqualGUID(_connected_mediatype.subtype,MEDIASUBTYPE_YV12))
		{
			/* working with new method */
			icvYV12toBGR888( (char*)ppBuffer, _handle->get_parent()->get_image(SSTT_IMAGE_BGR24)->imageData, _temp.height, _temp.width );

		} else
		if( IsEqualGUID(_connected_mediatype.subtype,MEDIASUBTYPE_I420)||
			IsEqualGUID(_connected_mediatype.subtype,MEDIASUBTYPE_IYUV) )
		{
			/* working with new method */
			icvVY12toBGR888( (char*)ppBuffer,_handle->get_parent()->get_image(SSTT_IMAGE_BGR24)->imageData, _temp.height, _temp.width );

		} else
		if (IsEqualGUID(_connected_mediatype.subtype,MEDIASUBTYPE_RGB565))
		{
			CvSize size = cvGetSize(&_temp);
			cvSetData(&_temp,ppBuffer,size.width * 2);
			cvCvtColor(&_temp,_handle->get_parent()->get_image(SSTT_IMAGE_BGR24),CV_BGR5652BGR);

		} else
		if (IsEqualGUID(_connected_mediatype.subtype,MEDIASUBTYPE_YUY2) ||
			IsEqualGUID(_connected_mediatype.subtype,MEDIASUBTYPE_YUYV))
		{
			/* working with new method */
			CvSize size = cvGetSize(&_temp);
			cvSetData(&_temp,ppBuffer,CV_AUTO_STEP);
			icvYUY2toBGR(&_temp,_handle->get_parent()->get_image(SSTT_IMAGE_BGR24),ppBuffer);
		} else
		if (IsEqualGUID(_connected_mediatype.subtype,MEDIASUBTYPE_UYVY))
		{
			/* working with new method */
			CvSize size = cvGetSize(&_temp);
			cvSetData(&_temp,ppBuffer,CV_AUTO_STEP);
			icvUYVYtoBGR(&_temp,_handle->get_parent()->get_image(SSTT_IMAGE_BGR24),ppBuffer);
		}
		else  {

			//IsEqualGUID(_connected_mediatype.subtype,MEDIASUBTYPE_RGB24)

			SSTT_DS_DEBUG("CapturePin::Receive - Unknown format");

			//std::cout << "Unknown format" << std::endl;

			if (0/*_handle->_file*/)
			{
				fprintf(stdout/*_handle->_file*/,"CapturePin::Receive - Unknown format {0x%x,0x%x}",
					_connected_mediatype.subtype.Data1,
					_connected_mediatype.subtype.Data2);
			}
		}

		// should be in the core!
		cvCvtColor(_handle->get_parent()->get_image(SSTT_IMAGE_BGR24),
			_handle->get_parent()->get_image(SSTT_IMAGE_GRAY),CV_BGR2GRAY);

#endif
		pSample->Release();

		//_handle->get_parent()->submit();

		//LeaveCriticalSection(&_cs);

	}


	return S_OK;
}

STDMETHODIMP CapturePin::ReceiveMultiple( IMediaSample **pSamples,
										 long nSamples,
										 long *nSamplesProcessed )
{


	SSTT_DS_DEBUG("CapturePin::ReceiveMultiple");

	HRESULT hr = S_OK;

	*nSamplesProcessed = 0;
	while( nSamples-- > 0 )
	{
		hr = Receive( pSamples[*nSamplesProcessed] );
		if( hr != S_OK ) break;
		(*nSamplesProcessed)++;
	}

	return hr;
}



STDMETHODIMP CapturePin::ReceiveCanBlock( void )
{
	SSTT_DS_DEBUG("CapturePin::ReceiveCanBlock");

	// was S_OK - but we actually don't want

	return S_FALSE;
}


CapturePin::~CapturePin()
{

	SSTT_DS_DEBUG("CapturePin::~CapturePin");

	for( size_t c = 0; c < _mediatype_count; c++ )
	{
		uFreeMediaType(_mediatypes[c]);
	}

	uFreeMediaType(_connected_mediatype);

	DeleteCriticalSection(&_cs);

}



/****************************************************************************
* Implementation of our dummy directshow enumpins class
****************************************************************************/



CaptureEnumMediaTypes::CaptureEnumMediaTypes(
	vx_source_dshow* handle, CapturePin *_p_pin,
	CaptureEnumMediaTypes *pEnumMediaTypes
	)
	: _pin( _p_pin ), _refcount( 1 ), _handle(handle), _position(0)
{
	/* Hold a reference count on our filter */
	_pin->AddRef();

	/* Are we creating a new enumerator */
	if( pEnumMediaTypes == NULL )
	{
		uCopyMediaType(&_connected_mediatype, &_pin->_connected_mediatype);
		_position = 0;
	}
	else
	{
		uCopyMediaType(&_connected_mediatype, &pEnumMediaTypes->_connected_mediatype);
		_position = pEnumMediaTypes->_position;
	}
}

CaptureEnumMediaTypes::~CaptureEnumMediaTypes()
{
	uFreeMediaType(_connected_mediatype);
	_pin->Release();
}

/* IUnknown methods */
STDMETHODIMP CaptureEnumMediaTypes::QueryInterface( REFIID riid, void **ppv )
{
	if( riid == IID_IUnknown ||
		riid == IID_IEnumMediaTypes )
	{
		AddRef();
		*ppv = (IEnumMediaTypes *)this;
		return NOERROR;
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
}

STDMETHODIMP_(ULONG) CaptureEnumMediaTypes::AddRef()
{
	return _refcount++;
}

STDMETHODIMP_(ULONG) CaptureEnumMediaTypes::Release()
{
	if( !InterlockedDecrement(&_refcount) ) delete this;

	return 0;
}

/* IEnumMediaTypes */
STDMETHODIMP CaptureEnumMediaTypes::Next( ULONG cMediaTypes,
										 AM_MEDIA_TYPE ** ppMediaTypes,
										 ULONG * pcFetched )
{

	ULONG copied = 0;
	ULONG offset = 0;
	ULONG max = _pin->_mediatype_count;

	if( ! ppMediaTypes )
		return E_POINTER;

	if( (! pcFetched)  && (cMediaTypes > 1) )
		return E_POINTER;

	/*
	** use connection media type as first entry in iterator if it exists
	*/
	copied = 0;
	if( _connected_mediatype.subtype != GUID_NULL )
	{
		++max;
		if( _position == 0 )
		{
			ppMediaTypes[copied] =
				(AM_MEDIA_TYPE *)CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));
			if( uCopyMediaType(ppMediaTypes[copied], &_connected_mediatype) != S_OK )
				return E_OUTOFMEMORY;
			++_position;
			++copied;
		}
	}

	while( (copied < cMediaTypes) && (_position < max)  )
	{
		ppMediaTypes[copied] =
			(AM_MEDIA_TYPE *)CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));

		if( uCopyMediaType( ppMediaTypes[copied],
			&_pin->_mediatypes[_position-offset]) != S_OK )
			return E_OUTOFMEMORY;

		++copied;
		++_position;
	}

	if( pcFetched )  *pcFetched = copied;

	return (copied == cMediaTypes) ? S_OK : S_FALSE;
};
STDMETHODIMP CaptureEnumMediaTypes::Skip( ULONG cMediaTypes )
{
	ULONG max =  _pin->_mediatype_count;
	if( _connected_mediatype.subtype != GUID_NULL )
	{
		max = 1;
	}

	_position += cMediaTypes;
	return (_position < max) ? S_OK : S_FALSE;
};
STDMETHODIMP CaptureEnumMediaTypes::Reset()
{
	uFreeMediaType(_connected_mediatype);
	uCopyMediaType(&_connected_mediatype, &_pin->_connected_mediatype);

	_position = 0;
	return S_OK;
};

STDMETHODIMP CaptureEnumMediaTypes::Clone( IEnumMediaTypes **ppEnum )
{
	*ppEnum = new CaptureEnumMediaTypes( _handle, _pin, this );
	if( *ppEnum == NULL ) return E_OUTOFMEMORY;

	return NOERROR;
};




CaptureEnumPins::CaptureEnumPins( vx_source_dshow *handle,
								 CaptureFilter *_p_filter,
								 CaptureEnumPins *pEnumPins )
								 : _handle(handle), p_filter( _p_filter ), i_ref( 1 )
{
	SSTT_DS_DEBUG("CaptureEnumPins::CaptureEnumPins");

	/* Hold a reference count on our filter */
	p_filter->AddRef();

	/* Are we creating a new enumerator */
	if( pEnumPins == NULL )
	{
		i_position = 0;
	}
	else
	{
		i_position = pEnumPins->i_position;
	}
}

CaptureEnumPins::~CaptureEnumPins()
{

	SSTT_DS_DEBUG("CaptureEnumPins::~CaptureEnumPins");

	p_filter->Release();
}

/* IUnknown methods */
STDMETHODIMP CaptureEnumPins::QueryInterface( REFIID riid, void **ppv )
{
	SSTT_DS_DEBUG("CaptureEnumPins::QueryInterface - In ");

	if( IsEqualIID ( riid, IID_IUnknown ) ||
		IsEqualIID ( riid, IID_IEnumPins ) )
	{
		*ppv = static_cast<IEnumPins*>(this);

	} else {

		SSTT_DS_DEBUG("CaptureEnumPins::QueryInterface - Fail ");

		*ppv = NULL;
		return E_NOINTERFACE;
	}

	AddRef();

	SSTT_DS_DEBUG("CaptureEnumPins::QueryInterface - Ok ");

	return S_OK;
}

STDMETHODIMP_(ULONG) CaptureEnumPins::AddRef()
{
	return i_ref++;
}


STDMETHODIMP_(ULONG) CaptureEnumPins::Release()
{
	if( !InterlockedDecrement(&i_ref) ) delete this;

	return 0;
};

/* IEnumPins */
STDMETHODIMP CaptureEnumPins::Next( ULONG cPins, IPin ** ppPins,
								   ULONG * pcFetched )
{
	SSTT_DS_DEBUG("CaptureEnumPins::Next");

	unsigned int i_fetched = 0;

	if( i_position < 1 && cPins > 0 )
	{
		IPin *pPin = p_filter->CustomGetPin();
		*ppPins = pPin;
		pPin->AddRef();
		i_fetched = 1;
		i_position++;
	}

	if( pcFetched ) *pcFetched = i_fetched;

	return (i_fetched == cPins) ? S_OK : S_FALSE;
}

STDMETHODIMP CaptureEnumPins::Skip( ULONG cPins )
{
	SSTT_DS_DEBUG("CaptureEnumPins::Skip");

	i_position += cPins;

	if( i_position > 1 )
	{
		return S_FALSE;
	}

	return S_OK;
};
STDMETHODIMP CaptureEnumPins::Reset()
{
	SSTT_DS_DEBUG("CaptureEnumPins::Reset");

	i_position = 0;
	return S_OK;
}


STDMETHODIMP CaptureEnumPins::Clone( IEnumPins **ppEnum )
{
	SSTT_DS_DEBUG("CaptureEnumPins::Clone");

	*ppEnum = new CaptureEnumPins( _handle,  p_filter, this );
	if( *ppEnum == NULL ) return E_OUTOFMEMORY;

	return NOERROR;
}


