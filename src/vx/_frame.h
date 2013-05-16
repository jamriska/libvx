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
#ifndef _LIBVX_INT_FRAME_H_
#define _LIBVX_INT_FRAME_H_ 1

#include "vx/vx.h"

/**
 * @brief create the header for the buffer
 * @param w width of the buffer
 * @param h height of the buffer
 * @param fourCC FourCC based code
 * @param frame only the internal structures are set, no memory is allocated
 */
void _vx_frame_create_header(int w, int h,unsigned int fourCC,vx_frame* frame);


/**
 * @brief create the header and the buffer
 * @param w width of the frame
 * @param h height of the frame
 * @param fourCC FourCC based code for declaring the pixelformat
 * @param frame structures are set and memory for the buffer are allocated
 */
void _vx_frame_create(int w, int h,unsigned int fourCC,vx_frame* frame);


void _vx_frame_draw_rectangle(int x, int y, int w, int h,
							  int r, int g, int b, int a,
							  vx_frame* frame);
#endif
