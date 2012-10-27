

#include <vx/vx.h>

static void
frameCallback(vx_context* c, const vx_frame* frame,void* userData)
{
}

int main(int argc, char** argv) {
	
	void * s;
	vx_context *c;
	
	
	if (argc > 1) 
		s = vx_source_create(argv[1]);
	else
		s = vx_source_create("null");
	
	c = vx_context_create("context");

    vx_context_ref(c);

	vx_context_set_frame_callback(c,frameCallback,0);

	vx_source_add_context(s,c);

	vx_source_open(s,"0,320x240");

	vx_source_set_state(s,VX_SOURCE_STATE_RUNNING);

    /* do something */

    vx_context_unref(c);

    return 0;
}
