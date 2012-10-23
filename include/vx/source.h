/*

Copyright (c) 2006-2012 Hartmut Seichter
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

#ifndef _LIBVX_VX_SOURCE_H_
#define _LIBVX_VX_SOURCE_H_ 1

#include <vx/cdecl.h>
#include <vx/def.h>
#include <vx/backend.h>
#include <vx/context.h>

typedef struct vx_source vx_source;

enum {
	VX_SOURCE_STATE_NULL = 0,
	VX_SOURCE_STATE_PAUSED,
	VX_SOURCE_STATE_RUNNING,
	VX_SOURCE_STATE_FINISHED,
	VX_SOURCE_STATE_STOP
};


VX_API_CALL vx_source * vx_source_create(const char *n);

VX_API_CALL int vx_source_ref(vx_source *s);

VX_API_CALL int vx_source_unref(vx_source *s);


VX_API_CALL int vx_source_open(vx_source *s,const char* settings);

VX_API_CALL int vx_source_close(vx_source *s);

VX_API_CALL int vx_source_set_state(vx_source *s,int newstate);

VX_API_CALL int vx_source_get_state(vx_source *s,int* outstate);

VX_API_CALL int vx_source_add_context(vx_source* s, vx_context* c);

#endif
