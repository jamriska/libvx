#include "_frame.h"


#include "_globals.h"

void
_vx_frame_create_header(int w, int h,unsigned int fourCC,vx_frame* frame) {

    frame->width = w;
    frame->height = h;

    switch (fourCC) {
    case VX_E_COLOR_BGR24:
        frame->colorModel = fourCC;
        frame->bpp = 24;
        frame->stride = w*3;
        frame->dataSize = frame->stride*h;
        break;
    }
}


void
_vx_frame_create(int w, int h,unsigned int fourCC,vx_frame* frame) {

    _vx_frame_create_header(w,h,fourCC,frame);
    frame->data = malloc(frame->dataSize);
}

