#include "gst010_backend.h"


/* include GLib for GStreamer */
#include <glib.h>

/* include GStreamer itself */
#include <gst/gst.h>

/* some additional stuff */
#include <stdlib.h>


typedef struct vx_source_gstreamer {
	/* base class */
	vx_source source;

	/* GStreamer pipeline */
	GstElement *pipeline;

	/* GStreamer identity needed for probing */
	GstElement *probe;


} vx_source_gstreamer;


/* helper */
#define VX_GSTREAMER_CAST(ptr) \
	((vx_source_gstreamer*)(ptr))


static
gboolean
bufferProbeCallback(GstPad* pad, GstBuffer* buffer, gpointer user_data) {
	const GstCaps *caps;
	GstStructure *str;

	gint width,height;
	gdouble rate;


	vx_source_gstreamer *vid = (vx_source_gstreamer*)user_data;

	if(vid->source.state != VX_source_STATE_RUNNING)
		return TRUE;

	caps = gst_pad_get_negotiated_caps(pad);


	if (caps) {

		str=gst_caps_get_structure(caps,0);

		/* Get some data about the frame */
		gst_structure_get_int(str,"width",&width);
		gst_structure_get_int(str,"height",&height);
		gst_structure_get_double(str,"framerate",&rate);

	}
	vx_frame probe;

	probe.width = width;
	probe.height = height;
	probe.dataSize = GST_BUFFER_SIZE(buffer);
	probe.data = GST_BUFFER_DATA(buffer);
	probe.stride = probe.dataSize/probe.height;
	
	_vx_send_frame(vid->source,&frame);

	// vid->_source.context->frameCallback(vid->_source.context,&probe,vid->_source.context->frameCallbackUserData);

//	printf("%s %d\n",__FUNCTION__,__LINE__);

/*
	if (vid->videoBuffer)
	{
		vid->frame++;
		memcpy(vid->videoBuffer, buffer->data, buffer->size);
	}
*/
	return TRUE;

}


static
void
videoCapsCallback(GObject* obj, GParamSpec* pspec, gpointer data) {

	const GstCaps *caps;
	GstStructure *str;

	gint width,height;
	gdouble rate;

	vx_source_gstreamer *vid = (vx_source_gstreamer*)data;

	caps = gst_pad_get_negotiated_caps((GstPad*)obj);

	if (caps) {

		str=gst_caps_get_structure(caps,0);

		/* Get some data about the frame */
		gst_structure_get_int(str,"width",&width);
		gst_structure_get_int(str,"height",&height);
		gst_structure_get_double(str,"framerate",&rate);

		g_print("libvx: GStreamer negotiated %dx%d @%3.3fps\n", width, height,rate);

		/*
		vid->width = width;
		vid->height = height;

		g_print("libvx: allocating %d bytes\n",(vid->width * vid->height * AR_PIX_SIZE_DEFAULT));

		arMalloc(vid->videoBuffer, ARUint8, (vid->width * vid->height * AR_PIX_SIZE_DEFAULT) );

		*/
	}
}


void
set_video_caps(int width, int height, GstElement* elem)
{
	GstCaps *caps;

	caps = gst_caps_new_simple ("video/x-raw-rgb",
		"bpp",G_TYPE_INT,24,
		"depth",G_TYPE_INT,24,
		"width", G_TYPE_INT, width,
		"height", G_TYPE_INT, height,
		NULL);

	g_object_set (G_OBJECT (elem), "caps", caps, NULL);
}

static
int vx_source_gstreamer_open(vx_source *s, const char* n)
{
	GstIterator* iter = 0;
	char* env = getenv("VX_GSTREAMER");
	const char* srcConfig  = 0;
	char config[500];
	GError* error = 0;
	unsigned char is_live = 0;
	int ret = 0;
	GstPad *pad, *peerpad = 0;

	const char* pipelineTemplate =
			"%s ! identity name=libvx ! fakesink";

	vx_source_gstreamer* source = (vx_source_gstreamer*)s;

	if (env == 0 && n == 0) {
		srcConfig = "videotestsrc";
	}

	if (n) srcConfig  = n;

	snprintf(config,500,pipelineTemplate,srcConfig);

	source->pipeline = gst_parse_launch (config, &error);

	if (0 == source->pipeline) {
			g_print ("Parse error: %s\n", error->message);
			return 0;
	};

	source->source.state = VX_SOURCE_STATE_READY;

	while (iter = gst_bin_iterate_sinks(GST_BIN(source->pipeline)))
	{
	}

	/* get the video sink */
	source->probe = gst_bin_get_by_name(GST_BIN(source->pipeline), "libvx");

	if (!source->probe) {
		g_print("Pipeline has no element named 'libvx'!\n");
		return 0;
	};

	gst_pipeline_auto_clock(GST_PIPELINE(source->pipeline));

	/* get the pad from the probe (the source pad seems to be more flexible) */
	pad = gst_element_get_static_pad (source->probe, "sink");

	/* get the peerpad aka sink */
	peerpad = gst_pad_get_peer(pad);

	/* install the probe callback for capturing */
	gst_pad_add_buffer_probe (pad, G_CALLBACK (bufferProbeCallback), source);

	g_signal_connect(pad, "notify::caps", G_CALLBACK(videoCapsCallback), source);

	/* Needed to fill the information for ARVidInfo */
	ret = gst_element_set_state (source->pipeline, GST_STATE_READY);

	/* wait until it's up and running or failed */
	if (gst_element_get_state (source->pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE) {
		g_error ("libvx: failed to put GStreamer into READY state!\n");
	} else {

		is_live = (ret == GST_STATE_CHANGE_NO_PREROLL) ? 1 : 0;
		g_print ("libvx: GStreamer pipeline is READY!\n");
	}

	/* Needed to fill the information for ARVidInfo */
	ret = gst_element_set_state (source->pipeline, GST_STATE_PAUSED);

	is_live = (ret == GST_STATE_CHANGE_NO_PREROLL) ? 1 : 0;

	/* wait until it's up and running or failed */
	if (gst_element_get_state (source->pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE) {
		g_error ("libvx: failed to put GStreamer into PAUSED state!\n");
	} else {
		g_print ("libvx: GStreamer pipeline is PAUSED!\n",is_live);
	}

	/* dismiss the pad */
	gst_object_unref (pad);

	/* dismiss the peer-pad */
	gst_object_unref (peerpad);

	/* now preroll for live sources */
	if (is_live) {

		g_print ("libvx: need special prerolling for live sources\n");

		/* set playing state of the pipeline */
		gst_element_set_state (source->pipeline, GST_STATE_PLAYING);

		/* wait until it's up and running or failed */
		if (gst_element_get_state (source->pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE) {
			g_error ("libvx: failed to put GStreamer into PLAYING state!\n");
		} else {
			g_print ("libvx: GStreamer pipeline is PLAYING!\n");
		}

		/* set playing state of the pipeline */
		gst_element_set_state (source->pipeline, GST_STATE_PAUSED);

		/* wait until it's up and running or failed */
		if (gst_element_get_state (source->pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE) {
			g_error ("libvx: failed to put GStreamer into PAUSED state!\n");
		} else {
			g_print ("libvx: GStreamer pipeline is PAUSED!\n");
		}
	}

	source->source.state = VX_SOURCE_STATE_PAUSED;

	return 0;
}

static int
vx_source_gstreamer_close(vx_source *s)
{
	vx_source_gstreamer* source = (vx_source_gstreamer*)s;
	gst_object_unref (GST_OBJECT (source->pipeline));

	source->source.state = VX_SOURCE_STATE_NULL;

	return 0;
}

static int
vx_source_gstreamer_set_state(vx_source *s,int state) {

	printf("%s %d\n",__FUNCTION__,__LINE__);

	vx_source_gstreamer* source = (vx_source_gstreamer*)s;

	GstStateChangeReturn ret;

	if (source->source.state > VX_SOURCE_STATE_NULL) {

		switch(state) {
		case VX_SOURCE_STATE_RUNNING:
			ret = gst_element_set_state (source->pipeline, GST_STATE_PLAYING);
			if (ret == GST_STATE_CHANGE_ASYNC)
			{
				/* wait until it's up and running or failed */
				if (gst_element_get_state (source->pipeline,
						NULL, NULL, GST_CLOCK_TIME_NONE) == GST_STATE_CHANGE_FAILURE)
				{
					g_error ("libvx: failed to put GStreamer into PLAYING state!\n");
					return -1;
				} else {
					g_print ("libvx: GStreamer pipeline is PLAYING!\n");
				}
			}
			VX_SOURCE_CAST(source)->state = VX_SOURCE_STATE_RUNNING;
			break;

		case VX_SOURCE_STATE_PAUSED:
			ret = gst_element_set_state (source->pipeline, GST_STATE_PAUSED);
			break;

		default:
			g_print("libvx: GStreamer pipeline can not switch to this state");
			break;
		}

	} else {
		g_print("libvx: GStreamer pipeline not initialized");
	}

	return 0;
}

int vx_source_gstreamer_get_state(vx_source *s,int *state)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}


int 
vx_source_gstreamer_enumerate(vx_source* s,vx_source_description** e)
{
	return 0;
}


void*
vx_source_gstreamer_create()
{
	/* initialise GStreamer */
	gst_init(0,0);


	/* report the current version and features */
	g_print ("Backend - %s\n", gst_version_string());

	vx_source_gstreamer* s = malloc(sizeof(struct vx_source_gstreamer));

	VX_SOURCE_CAST(s)->enumerate = vx_source_gstreamer_enumerate;
	VX_SOURCE_CAST(s)->open = vx_source_gstreamer_open;
	VX_SOURCE_CAST(s)->close = vx_source_gstreamer_close;
	VX_SOURCE_CAST(s)->set_state = vx_source_gstreamer_set_state;
	VX_SOURCE_CAST(s)->get_state = vx_source_gstreamer_get_state;
	VX_SOURCE_CAST(s)->update = vx_source_gstreamer_update;


	return s;
}
