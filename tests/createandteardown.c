

#include <vx/vx.h>
#include <stdio.h>

static void
frameCallback(vx_source* so,vx_sink* si, const vx_frame* frame,void* userData)
{
	printf("frame #%ld %dx%d stride: %d\n",frame->frame,
		   frame->width,frame->height,
		   frame->stride);
}

int main(int argc, char** argv)
{
	int i = 0;

	vx_source *source;
	vx_sink *sink;
	vx_source_description *d, *dIt;

	d = 0; dIt = d;

	if (argc > 1)
		source = vx_source_create(argv[1]);
	else
		source = vx_source_create(0);

    vx_source_enumerate(source,&d);

//	dIt = d;

//	while (dIt && dIt->name) {
//		printf("Name: %s - UUID:%s\n",dIt->name,dIt->uuid);
//		dIt++;
//	}

	sink = vx_sink_create("context");

	vx_sink_ref(sink);

	vx_sink_set_frame_callback(sink,&frameCallback,0);


//	vx_sink_unref(sink);

    if (vx_source_open(source,"blash") == 0)
	{
		vx_source_add_sink(source,sink);

		vx_source_set_state(source,VX_SOURCE_STATE_RUNNING);

		/* do something */
		for (i = 0; i < 1000; ++i) {
			//printf("%d\n",i);
			vx_source_update(source);
		}

		vx_source_set_state(source,VX_SOURCE_STATE_STOP);

		vx_source_close(source);

		vx_source_unref(source);

	}

	return 0;
}
