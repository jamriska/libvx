#include "_globals.h"
#include "_frame.h"

void
_vx_frame_create_header(int w, int h,unsigned int fourCC,vx_frame* frame) {

    frame->width = w;
    frame->height = h;
	frame->colorModel = fourCC;

    switch (fourCC) {
	case VX_E_COLOR_RGB24:
    case VX_E_COLOR_BGR24:
        frame->bpp = 24;
        frame->stride = w*3;
        frame->dataSize = frame->stride*h;
		break;
	case VX_E_COLOR_RGBA:
	case VX_E_COLOR_BGRA:
	case VX_E_COLOR_ARGB:
	case VX_E_COLOR_ABGR:
		frame->bpp = 32;
		frame->stride = w*4;
		frame->dataSize = frame->stride*h;
		break;
    }
}

void
_vx_frame_create(int w, int h,unsigned int fourCC,vx_frame* frame) {

    _vx_frame_create_header(w,h,fourCC,frame);
    frame->data = malloc(frame->dataSize);
}

