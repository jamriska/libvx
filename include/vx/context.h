/*

Copyright (c) 2006-2013 Hartmut Seichter

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef _LIBVX_VX_SINK_H_
#define _LIBVX_VX_SINK_H_ 1

#include <vx/cdecl.h>
#include <vx/frame.h>

VX_C_DECL_BEGIN

typedef struct vx_sink vx_sink;

typedef void (*vx_frame_cb_t) (vx_sink* c, const vx_frame* f, void *userdata);

VX_API_CALL vx_sink* vx_sink_create(const char* name);

VX_API_CALL int vx_sink_ref(vx_sink* c);

VX_API_CALL int vx_sink_unref(vx_sink* c);

VX_API_CALL int vx_sink_set_frame_callback(vx_sink* c, vx_frame_cb_t cb, void* userdata);

VX_C_DECL_END


#endif
