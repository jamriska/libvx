#include "_globals.h"
#include "_frame.h"

void
_vx_frame_create_header(int w, int h,unsigned int fourCC,vx_frame* frame) {

    frame->width = w;
    frame->height = h;
	frame->colorModel = fourCC;

    switch (fourCC) {
	case VX_E_COLOR_GRAY8:
		frame->bpp = 8;
		frame->stride = w;
		frame->dataSize = frame->stride*h;
		break;
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


void _vx_frame_draw_rectangle(int x, int y, int w, int h,
							  int r, int g, int b, int a,
							  vx_frame* frame)
{
	int bytePerPixel = frame->bpp >> 3;

	int offsets[4];

	switch(frame->colorModel) {
	case VX_E_COLOR_RGB24:
		offsets[0] = 0;offsets[1] = 1;offsets[2] = 2;offsets[3] = -1;
		break;
	case VX_E_COLOR_BGR24:
		offsets[0] = 2;offsets[1] = 1;offsets[2] = 0;offsets[3] = -1;
		break;
	case VX_E_COLOR_BGRA:
		offsets[0] = 2;offsets[1] = 1;offsets[2] = 1;offsets[3] = 3;
		break;
	case VX_E_COLOR_RGBA:
		offsets[0] = 0;offsets[1] = 1;offsets[2] = 2;offsets[3] = 3;
		break;
	case VX_E_COLOR_ABGR:
		offsets[0] = 3;offsets[1] = 2;offsets[2] = 1;offsets[3] = 0;
		break;
	case VX_E_COLOR_ARGB:
		offsets[0] = 1;offsets[1] = 2;offsets[2] = 3;offsets[3] = 0;
		break;
	case VX_E_COLOR_GRAY8:
		offsets[0] = 0;offsets[1] = -1;offsets[2] = -1;offsets[3] = -1;
		break;
	}

	int i,j;
	for (    i = y; i < y+h; ++i )
	{
		char* row = frame->data + i *  frame->stride;
		for (    j = x; j < x+w; ++j )
		{
			if(offsets[0] >= 0) row[bytePerPixel*j + offsets[0]] = r;
			if(offsets[1] >= 0) row[bytePerPixel*j + offsets[1]] = g;
			if(offsets[2] >= 0) row[bytePerPixel*j + offsets[2]] = b;
			if(offsets[3] >= 0) row[bytePerPixel*j + offsets[3]] = a;
		}
	}
}
