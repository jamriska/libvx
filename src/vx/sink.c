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
#include "_frame.h"




//void convert_YUYV_to_RGB24(const vx_frame* inputSrc, vx_frame* destBgr)
//{
//  #define K1 (int)(1.402f * (1 << 16))
//  #define K2 (int)(0.714f * (1 << 16))
//  #define K3 (int)(0.334f * (1 << 16))
//  #define K4 (int)(1.772f * (1 << 16))

//  unsigned char* out_ptr = destBgr->data;
//  const int pitch = inputSrc->width * 2; // 2 bytes per one YU-YV pixel

//  int y,x;

//  for (y=0; y < inputSrc->height; y++) {
//    const unsigned char* src = inputSrc->data + pitch * y;
//    for (x=0; x<inputSrc->width*2; x+=4) { // Y1 U Y2 V
//      unsigned char Y1 = src[x + 0];
//      unsigned char U  = src[x + 1];
//      unsigned char Y2 = src[x + 2];
//      unsigned char V  = src[x + 3];

//      unsigned char uf = U - 128;
//      unsigned char vf = V - 128;

//      int R = Y1 + (K1*vf >> 16);
//      int G = Y1 - (K2*vf >> 16) - (K3*uf >> 16);
//      int B = Y1 + (K4*uf >> 16);

//      saturate(&R, 0, 255);
//      saturate(&G, 0, 255);
//      saturate(&B, 0, 255);

//      *out_ptr++ = R;
//      *out_ptr++ = G;
//      *out_ptr++ = B;

//      R = Y2 + (K1*vf >> 16);
//      G = Y2 - (K2*vf >> 16) - (K3*uf >> 16);
//      B = Y2 + (K4*uf >> 16);

//      saturate(&R, 0, 255);
//      saturate(&G, 0, 255);
//      saturate(&B, 0, 255);

//      *out_ptr++ = R;
//      *out_ptr++ = G;
//      *out_ptr++ = B;
//    }

//  }
//}
//
///* color conversion functions from Bart Nabbe. */
///* corrected by Damien: bad coeficients in YUV2RGB */
//#define YUV2RGB(y, u, v, r, g, b)\
//    r = y + ((v*1436) >> 10);\
//    g = y - ((u*352 + v*731) >> 10);\
//    b = y + ((u*1814) >> 10);\
//    r = r < 0 ? 0 : r;\
//    g = g < 0 ? 0 : g;\
//    b = b < 0 ? 0 : b;\
//    r = r > 255 ? 255 : r;\
//    g = g > 255 ? 255 : g;\
//    b = b > 255 ? 255 : b; \
//
//
//static void _UYVYtoBGR( const vx_frame* inputSrc, vx_frame* destBgr)
//{
//
//    unsigned const char *src = (unsigned char*)inputSrc->data;
//
//    if (destBgr->data == 0)
//        _vx_frame_create(inputSrc->width,inputSrc->height,destBgr->colorModel,destBgr);
//
//    char *dest = (char*)destBgr->data;
//    unsigned int NumPixels = inputSrc->width * inputSrc->height;
//
//    register int i = (NumPixels << 1) - 1;
//    register int j = NumPixels + (NumPixels << 1) - 1;
//    register short y0, y1, u, v;
//    register int r, g, b;
//
//    while (i > 0) {
//#if 1
//
//        y1 = src[i--];
//        v = src[i--] - 128;
//        y0 = src[i--];
//        u = src[i--] - 128;
//        YUV2RGB(y1, u, v, r, g, b);
//        dest[j--] = r;
//        dest[j--] = g;
//        dest[j--] = b;
//        YUV2RGB(y0, u, v, r, g, b);
//        dest[j--] = r;
//        dest[j--] = g;
//        dest[j--] = b;
//#else
//        y1 = src[i--];
//        v = src[i--];
//        y0 = src[i--];
//        u = src[i--];
//        YUV2RGB(y1, u, v, r, g, b);
//        dest[j--] = b;
//        dest[j--] = g;
//        dest[j--] = r;
//        YUV2RGB(y0, u, v, r, g, b);
//        dest[j--] = b;
//        dest[j--] = g;
//        dest[j--] = r;
//#endif
//    }
//}

static int
_vx_frame_conversion(const vx_frame* in,vx_frame* out)
{
//    char fourCCin[5]; fourCCin[4] = '\0';
//    char fourCCout[5]; fourCCout[4] = '\0';

//    VX_FOURCC_TO_CHAR(in->colorModel,fourCCin);
//    VX_FOURCC_TO_CHAR(out->colorModel,fourCCout);

//    fprintf(stdout,"%s in:%d -> out:%d   (%s > %s)\n",__FUNCTION__,in->colorModel,out->colorModel,fourCCin,fourCCout);

    int done = 0;

    if (out->data == 0)
        _vx_frame_create(in->width,in->height,out->colorModel,out);


    switch (in->colorModel) {
    case VX_E_COLOR_YUYV:
        if (out->colorModel == VX_E_COLOR_BGR24) {
//            _UYVYtoBGR(in,out);
//            convert_YUYV_to_RGB24(in,out);
            done++;
        }
        return 0;
        break;
    default:
//        fprintf(stdout,"Unhandled input format: %d\n",in->colorModel);
        break;
    }

    if (done) {
        out->frame = in->frame;
        out->height = in->height;
        out->width = in->width;
        out->tick = in->tick;
    }

//    fflush(stdout);


    return 0;
}

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
    _vx_frame_copy(f,&sink->buffer);
}



static void
_vx_simple_conversion_callback(struct vx_source* source, struct vx_sink* sink, const vx_frame* f)
{
    _vx_frame_conversion(f,&sink->buffer);
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
    c->conversionCallback = _vx_simple_conversion_callback;

	c->sinkType = sinkType;

    /* hack */
    if (sinkType == VX_SINK_TYPE_CONVERTED)
        c->buffer.colorModel = VX_E_COLOR_BGR24;


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
