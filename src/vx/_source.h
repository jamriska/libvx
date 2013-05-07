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
#ifndef _LIBVX_INT_SOURCE_H_
#define _LIBVX_INT_SOURCE_H_ 1

#include "vx/source.h"
#include "_object.h"
#include "_context.h"

typedef int (*vx_source_enumerate_cb_t)(vx_source * s,vx_source_description** e);
typedef int (*vx_source_open_cb_t)(vx_source * s,const char* n);
typedef int (*vx_source_close_cb_t)(vx_source * s);
typedef int (*vx_source_set_state_cb_t)(vx_source * s,int newstate);
typedef int (*vx_source_get_state_cb_t)(vx_source * s,int *state);
typedef int (*vx_source_update_cb_t)(vx_source * s,unsigned int runloop);

struct vx_source {

	struct vx_object _object;

	vx_source_enumerate_cb_t enumerate;
	vx_source_open_cb_t open;
	vx_source_close_cb_t close;
	vx_source_get_state_cb_t get_state;
	vx_source_set_state_cb_t set_state;
	vx_source_update_cb_t update;

	int state;

	vx_sink* sink;
	int sinkCount;

};

#if defined (__cplusplus)
#define VX_CALL_C extern "C"
#else
#define VX_CALL_C extern
#endif


#define VX_SOURCE_CAST(ptr) \
	((vx_source*)(ptr))

VX_CALL_C int _vx_send_frame(vx_source* source, const vx_frame* frame);




#endif
