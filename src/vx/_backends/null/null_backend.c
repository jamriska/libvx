#include "null_backend.h"

#include "_source.h"
#include "_frame.h"


#include <stdlib.h>
#include <stdio.h>

typedef struct vx_source_null {
	vx_source super;

    vx_frame fakeFrame;

} vx_source_null;


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
    s->deviceCount = 1;
    s->devices = &null_cap;

	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_null_update(vx_source* s,unsigned int runloop)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);

    struct vx_source_null* null = VX_NULL_CAST(s);

    if (null->fakeFrame.data) {
        _vx_send_frame(s,&null->fakeFrame);
    } else {

        int w = 640;
        int h = 480;

        _vx_frame_create(w,h,VX_E_COLOR_BGR24,&null->fakeFrame);

        unsigned char* data = null->fakeFrame.data;

        int i,j;
        for (    i = 0; i < w; ++i ) {
            for (j = 0; j < h; ++j ) {

                data[j*w+i] = 0;
                data[j*w+i] = 0xFF;
                data[j*w+i] = 0;
            }
        }
    }


	return 0;
}

void*
vx_source_null_create()
{
	vx_source_null* s = malloc(sizeof(struct vx_source_null));

	VX_SOURCE_CAST(s)->open = vx_source_null_open;
	VX_SOURCE_CAST(s)->close = vx_source_null_close;
	VX_SOURCE_CAST(s)->set_state = vx_source_null_set_state;
	VX_SOURCE_CAST(s)->get_state = vx_source_null_get_state;
	VX_SOURCE_CAST(s)->enumerate = vx_source_null_enumerate;
	VX_SOURCE_CAST(s)->update = vx_source_null_update;

	return s;
}
