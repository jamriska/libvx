
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
    int i = 0, j = 0;

	vx_source *source;
	vx_sink *sink;
	vx_device_description *devices;
	int deviceCount;

	if (argc > 1)
		source = vx_source_create(argv[1]);
	else
		source = vx_source_create(0);

	vx_source_enumerate(source,&devices,&deviceCount);

	if (deviceCount == 0)
		return -1;

	for (i = 0; i < deviceCount;++i) {
		fprintf(stdout,"Name: %s - UUID:%s\n",devices[i].name,devices[i].uuid);

        for (j = 0; j < devices[i].capabilitiesCount;++j) {

            vx_device_capability* devcap = &devices[i].capabilities[j];

            char fourcc[5];
            VX_FOURCC_TO_CHAR(devcap->pixelFormat,fourcc);
            fourcc[4] = '\0';

            fprintf(stdout,"\t%dx%d @%f/%f fps fourcc:%s\n",
                     devcap->width,devcap->height,devcap->speed.numerator,devcap->speed.denominator,fourcc);
        }
	}

    fflush(stdout);

	sink = vx_sink_create("context",VX_SINK_TYPE_DIRECT);

	vx_sink_ref(sink);

	vx_sink_set_frame_callback(sink,&frameCallback,0);


//	vx_sink_unref(sink);

    if (vx_source_open(source,devices[0].uuid,0) == 0)
	{
		vx_source_add_sink(source,sink);

		vx_source_set_state(source,VX_SOURCE_STATE_RUNNING);

		/* do something */
		for (i = 0; i < 1000; ++i) {
			//printf("%d\n",i);
			vx_source_update(source,VX_SOURCE_UPDATE_PEEK);
		}

		vx_source_set_state(source,VX_SOURCE_STATE_STOP);

		vx_source_close(source);

		vx_source_unref(source);

	}

	return 0;
}
