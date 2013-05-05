#include <vx/vx.h>
#include <stdio.h>

#define UUID_1 "0xfa1410000ac83420"
#define UUID_2 "0xfa1440000ac83410"

static void
frameCallback(vx_source* so,vx_sink* si, const vx_frame* frame,void* userData)
{
	printf("frame #%ld %dx%d stride: %d datasize:%d (device: 0x%x)\n",frame->frame,
		   frame->width,frame->height,
		   frame->stride,frame->dataSize,
		   (unsigned int)so);
}

int main(int argc, char** argv)
{
	int i = 0;

	vx_source *source_1,*source_2;
	vx_sink *sink;
	vx_source_description *d, *dIt;

	source_1 = vx_source_create(vx_source_default());
	source_2 = vx_source_create(vx_source_default());

	vx_source_enumerate(source_1,&d);

	dIt = d;

	while (dIt && dIt->name) {
		printf("Name: %s - UUID:%s\n",dIt->name,dIt->uuid);
		dIt++;
	}

	sink = vx_sink_create("context");

	vx_sink_ref(sink);

	vx_sink_set_frame_callback(sink,&frameCallback,0);

	if ((vx_source_open(source_1,UUID_1) == 0) &&
		(vx_source_open(source_2,UUID_2) == 0))
	{
		vx_source_add_sink(source_1,sink);
		vx_source_add_sink(source_2,sink);

		if (vx_source_set_state(source_1,VX_SOURCE_STATE_RUNNING)) {
			printf("Error starting source 1\n");
		}
		if (vx_source_set_state(source_2,VX_SOURCE_STATE_RUNNING)) {
			printf("Error starting source 2\n");
		}

		int state;
		vx_source_get_state(source_1,&state);
		printf("Source 1 state:%d\n",state);
		vx_source_get_state(source_2,&state);
		printf("Source 2 state:%d\n",state);

		/* do something */
		for (i = 0; i < 1000; ++i) {
			vx_source_update(source_1);
//			vx_source_update(source_2);
		}

		vx_source_set_state(source_1,VX_SOURCE_STATE_STOP);
		vx_source_set_state(source_2,VX_SOURCE_STATE_STOP);

		vx_source_close(source_1);
		vx_source_close(source_2);


	}

	vx_source_unref(source_1);
	vx_source_unref(source_2);


	return 0;
}
