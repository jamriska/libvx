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

#ifndef _LIBVX_VX_SINK_H_
#define _LIBVX_VX_SINK_H_ 1


/** @file */

#include <vx/cdecl.h>
#include <vx/frame.h>

VX_C_DECL_BEGIN

typedef struct vx_sink vx_sink;
typedef struct vx_source vx_source;


/**
 * Callback to retrieve data from the sink. Can be used also to manipulate
 * the capture pipeline, the sink attachments. The buffer is owned by the
 * underlying capture backend and should be copied as fast as possible.
 * Do not try to deallocate it or write to it. Further, the buffer is
 * memory mapped. Do not unmap it here.
 */
typedef void (*vx_frame_cb_t)
    (vx_source* source, vx_sink* sink, const vx_frame* f, void *userdata);

/**
  * Create a sink structure.
  *
  * @param name can be used to identify the sink
  * @return instance of a sink
  */
VX_API_CALL vx_sink* vx_sink_create(const char* name);

/**
  * Attach a callback to the sink
  */
VX_API_CALL int vx_sink_set_frame_callback(vx_sink* c, vx_frame_cb_t cb, void* userdata);

/**
  * Increase reference counter
  */
VX_API_CALL int vx_sink_ref(vx_sink* c);

/**
  * Decrease reference counter
  */
VX_API_CALL int vx_sink_unref(vx_sink* c);

VX_C_DECL_END


#endif
