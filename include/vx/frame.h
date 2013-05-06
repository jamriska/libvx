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

#include <vx/cdecl.h>

VX_C_DECL_BEGIN

enum vx_colormodel {
	VX_E_COLOR_NONE		= 0,
	VX_E_COLOR_GRAY8,
	VX_E_COLOR_RGB24,
	VX_E_COLOR_BGR24,
	VX_E_COLOR_ARGB,
	VX_E_COLOR_RGBA,
	VX_E_COLOR_ABGR,
	VX_E_COLOR_BGRA,
	VX_E_COLOR_YU12
};

enum vx_datatype {
	VX_E_DATATYPE_NONE		= 0,
	VX_E_DATATYPE_UCHAR
};

typedef struct vx_frame {
	unsigned int width;
	unsigned int height;
	unsigned int stride;

	int colorModel;

	int dataType;
	int dataSize;
	int owner;

	void* data;

	unsigned long frame;
	unsigned long tick;

} vx_frame;

VX_C_DECL_END


#endif
