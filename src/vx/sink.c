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

#include "_sink.h"
#include "_globals.h"
#include "_source.h"

static int
_vx_frame_copy(const vx_frame* in,vx_frame* out)
{

    void* newData = 0;

    if (!in || !out || !in->data || !in->dataSize) return -1;

    /* new memory allocation */
    if (out->data == 0 && out->dataSize == 0) {
        void* newData = malloc(in->dataSize);
        if (newData) {
            out->data = newData;
            out->dataSize = in->dataSize;
        } else {
            return -1;
        }
    }

    /* resize buffer if necessary */
    if (out->data && (out->dataSize != in->dataSize)) {
        newData = realloc(out->data,in->dataSize);
        if (newData) {
            out->data = newData;
            out->dataSize = in->dataSize;
        } else {
            return -1;
        }
    }


    out->bpp = in->bpp;
    out->colorModel = in->colorModel;
    out->dataType = in->dataType;
    out->frame = in->frame;
    out->height = in->height;
    out->width = in->width;
    out->stride = in->stride;
    out->tick = in->tick;



    memcpy(out->data,in->data,in->dataSize);




    return 0;

}


static void
_vx_simple_copy_callback(struct vx_source* source, struct vx_sink* sink, const vx_frame* f)
{

    fprintf(stderr,"%p %s\n",sink,__FUNCTION__);

//    _vx_frame_copy(f,&sink->buffer);
}

static
void _vx_sink_destroy(vx_object* obj)
{
	struct vx_sink* sink = VX_SINK_CAST(obj);

	if (sink->buffer.data && sink->buffer.dataSize) {
		free(sink->buffer.data);
		sink->buffer.dataSize = 0;
		free(sink->name);
	}
}

vx_sink*
vx_sink_create(const char *name, unsigned int sinkType) {

	vx_sink* c = malloc(sizeof(struct vx_sink));

    memset(c,0,sizeof(struct vx_sink));
    memset(&c->buffer,0,sizeof(struct vx_frame));

	VX_OBJECT_CAST(c)->id = 0;
	VX_OBJECT_CAST(c)->refCount = 0;
	VX_OBJECT_CAST(c)->destroy = _vx_sink_destroy;

	c->frameCallback = 0;
	c->frameCallbackUserData = 0;

	/* preset */
	c->name = strdup(name);
	c->copyCallback = _vx_simple_copy_callback;

	c->sinkType = sinkType;

	return c;
}

int
vx_sink_ref(vx_sink *c) {
	if (c) { VX_OBJECT_CAST(c)->refCount++; return 0;}
	return -1;
}

int
vx_sink_unref(vx_sink *c) {
	if (c) {
		VX_OBJECT_CAST(c)->refCount--;
		if (VX_OBJECT_CAST(c)->refCount == 0) {
			free(c);
		}
		return 0;
	}
	return -1;
}


int
vx_sink_get_frame(vx_sink* sink,vx_frame** frame)
{
	*frame = &sink->buffer;
	return 0;
}

int
vx_sink_set_frame_callback(vx_sink *c, vx_frame_cb_t cb, void *userdata) {
	c->frameCallback = cb;
	c->frameCallbackUserData = userdata;
	return 0;
}


int
vx_sink_destroy(vx_sink **sink)
{
	if (*sink) {
		if(VX_OBJECT_CAST(*sink)->destroy) VX_OBJECT_CAST(*sink)->destroy(VX_OBJECT_CAST(*sink));
		free(*sink);
	}

	return 0;
}
