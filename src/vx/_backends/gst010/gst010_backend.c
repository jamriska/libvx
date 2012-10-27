#include "gst010_backend.h"

#include "_source.h"

/* include GLib for GStreamer */
#include <glib.h>

/* include GStreamer itself */
#include <gst/gst.h>

/* some additional stuff */
#include <stdlib.h>


typedef struct vx_source_gstreamer {
	/* base class */
	vx_source super;
	
	/* GStreamer pipeline */
	GstElement *pipeline;
	
	/* GStreamer identity needed for probing */
	GstElement *probe;
	
	
} vx_source_gstreamer;


static
int vx_source_gstreamer_open(vx_source_gstreamer* s, const char* n)
{
	
	printf("%s %d\n",__FUNCTION__,__LINE__);
	
	return 0;
}

static
int vx_source_gstreamer_close(vx_source_gstreamer* s)
{
	return 0;
}

int vx_source_gstreamer_set_state(vx_source_gstreamer* s,int state)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_gstreamer_get_state(vx_source_gstreamer* s,int* state)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
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
	
	VX_SOURCE(s)->open = vx_source_gstreamer_open;
	VX_SOURCE(s)->close = vx_source_gstreamer_close;
	VX_SOURCE(s)->set_state = vx_source_gstreamer_set_state;
	VX_SOURCE(s)->get_state = vx_source_gstreamer_get_state;
		
	return s;
}
