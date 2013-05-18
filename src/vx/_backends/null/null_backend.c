#include "_globals.h"
#include "_source.h"
#include "_frame.h"

#include "null_backend.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct vx_source_null {
	vx_source super;

    vx_frame fakeFrame;

} vx_source_null;


static vx_device_description null_desc;
static vx_device_capability null_cap;


/* helpers */
#define VX_NULL_CAST(ptr) \
    ((struct vx_source_null*)(ptr))


int vx_source_null_open(vx_source* s, const char* uuid,vx_device_capability* cap)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_null_close(vx_source* s)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_null_set_state(vx_source* s,int state)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_null_get_state(vx_source* s,int* state)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_null_enumerate(vx_source* s)
{

	null_desc.capabilities = &null_cap;

    s->deviceCount = 1;
	s->devices = &null_desc;


	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_null_update(vx_source* s,unsigned int runloop)
{
	int w,h;
	char* data;
	int i;

    struct vx_source_null* null = VX_NULL_CAST(s);

    if (null->fakeFrame.data) {
        _vx_send_frame(s,&null->fakeFrame);

		_vx_broadcast(s);

    } else {

		printf("%s %d\n",__FUNCTION__,__LINE__);

		w = 640;
		h = 480;

		_vx_frame_create(w,h,VX_E_COLOR_RGBA,&null->fakeFrame);

		data = null->fakeFrame.data;

		for (    i = 0; i < null->fakeFrame.dataSize; ++i )				{
			data[i] = 0xCC;
		}

		_vx_frame_draw_rectangle(10,h/2-5,10,10,0xFF,0x00,0x00,0xFF,&null->fakeFrame);
		_vx_frame_draw_rectangle(30,h/2-5,10,10,0x00,0xFF,0x00,0xFF,&null->fakeFrame);
		_vx_frame_draw_rectangle(50,h/2-5,10,10,0x00,0x00,0xFF,0xFF,&null->fakeFrame);

		_vx_frame_draw_rectangle(0,h-30,w,10,0xFF,0x00,0x00,0xFF,&null->fakeFrame);
		_vx_frame_draw_rectangle(0,h-20,w,10,0x00,0xFF,0x00,0xFF,&null->fakeFrame);
		_vx_frame_draw_rectangle(0,h-10,w,10,0x00,0x00,0xFF,0xFF,&null->fakeFrame);

		_vx_frame_draw_rectangle(0,20,w,10,0xFF,0xFF,0xFF,0xFF,&null->fakeFrame);
		_vx_frame_draw_rectangle(0,10,w,10,0xAA,0xAA,0xAA,0xFF,&null->fakeFrame);
		_vx_frame_draw_rectangle(0, 0,w,10,0x33,0x33,0x33,0xFF,&null->fakeFrame);
	}
	return 0;
}

void*
vx_source_null_create()
{
	struct vx_source_null* s = malloc(sizeof(struct vx_source_null));

	memset((void*)s,0,sizeof(struct vx_source_null));

	VX_SOURCE_CAST(s)->open = vx_source_null_open;
	VX_SOURCE_CAST(s)->close = vx_source_null_close;
	VX_SOURCE_CAST(s)->set_state = vx_source_null_set_state;
	VX_SOURCE_CAST(s)->get_state = vx_source_null_get_state;
	VX_SOURCE_CAST(s)->enumerate = vx_source_null_enumerate;
	VX_SOURCE_CAST(s)->update = vx_source_null_update;

	return s;
}
