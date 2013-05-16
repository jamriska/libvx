/*
Copyright (c) 2006-2013 Hartmut Seichter
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the VideoExtractor Project.
*/
#ifndef _LIBVX_VX_FRAME_H_
#define _LIBVX_VX_FRAME_H_ 1

/** @file */

#include <vx/cdecl.h>

VX_C_DECL_BEGIN

/* make fourcc */
#define VX_FOURCC(a, b, c, d)\
    ((unsigned int)(a) | ((unsigned int)(b) << 8) | ((unsigned int)(c) << 16) | ((unsigned int)(d) << 24))

/* retrieve fourcc */
#define VX_FOURCC_TO_CHAR(fourcc,cstr)    \
    cstr[0] = (fourcc & 0xff); \
    cstr[1] = (fourcc & 0xff00)     >> 8;\
    cstr[2] = (fourcc & 0xff0000)   >> 16;\
    cstr[3] = (fourcc & 0xff000000) >> 24;



/* only using what has been mentioned here :
 * http://people.xiph.org/~xiphmont/containers/nut/nut4cc.txt
 */

/** color model and pixel format of the retrieved frame */
enum vx_colormodel {
	VX_E_COLOR_NONE                 = 0,
    VX_E_COLOR_GRAY8                = VX_FOURCC('Y','8','0','0'),
    VX_E_COLOR_RGB24                = VX_FOURCC('R','G','B','8'),
    VX_E_COLOR_BGR24                = VX_FOURCC('B','G','R','8'),
    VX_E_COLOR_ARGB                 = VX_FOURCC('A','R','G','B'),
    VX_E_COLOR_RGBA                 = VX_FOURCC('R','G','B','A'),
    VX_E_COLOR_ABGR                 = VX_FOURCC('A','B','G','R'),
    VX_E_COLOR_BGRA                 = VX_FOURCC('B','G','R','A'),
    VX_E_COLOR_YU12                 = VX_FOURCC('Y','V','1','2'),
    VX_E_COLOR_YUVY                 = VX_FOURCC('Y','U','Y','V'),
    VX_E_COLOR_422P                 = VX_FOURCC('4','2','2','P'),
    /* aliases */
    VX_E_COLOR_420P                 = VX_E_COLOR_YU12,
    VX_E_COLOR_422I                 = VX_E_COLOR_YUVY,

    /* compressed */
    VX_E_COLOR_MJPEG                = VX_FOURCC('M','J','P','G'),

	VX_E_COLOR_USERDEFINED          = 0xFF
};

/** data type of the retrieved frame */
enum vx_datatype {
	VX_E_DATATYPE_NONE          = 0,
	VX_E_DATATYPE_UCHAR,
	VX_E_DATATYPE_FLOAT,
    VX_E_DATATYPE_USERDEFINED   = 0xFFFFFFFF
};


/** structure for buffer probe */
typedef struct vx_frame {
	unsigned int width;             /**< width of frame in pixel */
	unsigned int height;            /**< height of frame in pixel */
	unsigned int stride;            /**< bytes per row */

	int colorModel;                 /**< pixel format and layout */
	unsigned short bpp;				/**< bits per pixel */

	int dataType;                   /**< data format of buffer */
	int dataSize;                   /**< size of the buffer in bytes */
	int owner;                      /**< should be 0 (backend owns the buffer */

	void* data;                     /**< pointer to the buffer */

	unsigned long frame;            /**< frame counter */
	unsigned long tick;             /**< timestamp of the buffer */

} vx_frame;

VX_C_DECL_END


#endif
