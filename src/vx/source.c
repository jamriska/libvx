#include "_source.h"
#include "_context.h"

#include "vx/config.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <_backends/null/null_backend.h>

#if defined(HAVE_GSTREAMER)
	#include <_backends/gst010/gst010_backend.h>
#endif

#if defined(HAVE_AVFOUNDATION)
	#include "_backends/avfoundation/avfoundation_backend.h"
#endif

#if defined(HAVE_QTKIT)
	#include "_backends/qtkit/qtkit_backend.h"
#endif

int
vx_source_ref_init(vx_source *s) {
	if (s) { VX_OBJECT(s)->refCount = 0; return 0;}
	return -1;
}

int
vx_source_ref(vx_source *s) {
	if (s) { VX_OBJECT(s)->refCount++; return 0;}
	return -1;
}

int
vx_source_unref(vx_source *s) {
	if (s) {
		VX_OBJECT(s)->refCount--;

		if (VX_OBJECT(s)->refCount == 0) {
			free(s);
		}

		return 0;
	}
	return -1;
}


const char* vx_source_default()
{
	static char _null[] = "null";
	static char _gstreamer[] = "gstreamer";
	static char _avfoundation[] = "avfoundation";
	static char _qtkit[] = "qtkit";

#if defined(HAVE_GSTREAMER)
	return _gstreamer;
#elif defined(HAVE_QTKIT)
	return _qtkit;
#elif defined(HAVE_AVFOUNDATION)
	return _avfoundation;
#endif
	return _null;
}


void *
vx_source_create(const char *n) {

	vx_source* result = 0;

	if (n == 0) return vx_source_create(vx_source_default());

	if (0 == strcmp("null",n)) {
		result = vx_source_null_create();
	}
#if defined(HAVE_GSTREAMER)
	if (0 == strcmp("gstreamer",n)) {
		result = vx_source_gstreamer_create();
	}
#endif

#if defined(HAVE_AVFOUNDATION)
	if (0 == strcmp("avfoundation",n)) {
		result = vx_source_avfoundation_create();
	}
#endif

#if defined(HAVE_QTKIT)
	if (0 == strcmp("qtkit",n)) {
		result = vx_source_qtkit_create();
	}
#endif

	vx_source_ref_init(result);

	 return result;
}

int
vx_source_enumerate(vx_source *s, vx_source_description **e)
{
	return VX_SOURCE_CAST(s)->enumerate(s,e);
}

int
vx_source_open(vx_source *s, const char* n)
{
	s->sinkCount = 0;
	s->sink = 0;

	return VX_SOURCE_CAST(s)->open(s,n);
}

int
vx_source_close(vx_source *s)
{
	return VX_SOURCE_CAST(s)->close(s);
}

int
vx_source_set_state(vx_source *s, int newstate)
{
	return VX_SOURCE_CAST(s)->set_state(s,newstate);
}

int
vx_source_get_state(vx_source *s, int* state)
{
	return VX_SOURCE_CAST(s)->get_state(s,state);
}

int
vx_source_update(vx_source *s)
{
	return VX_SOURCE_CAST(s)->update(s);
}

int
vx_source_add_sink(vx_source* source, vx_sink* sink)
{
	int newSize = source->sinkCount+1;

	vx_sink* moreSink = realloc(source->sink,newSize*sizeof(vx_sink));

	if (moreSink)
	{
		moreSink[source->sinkCount] = *sink;

		source->sink = moreSink;
		source->sinkCount = newSize;

		return 0;
	}

	return -1;
}


int _vx_send_frame(vx_source* source,const vx_frame* frame)
{
	int i = 0;
	for(i = 0;i < source->sinkCount;++i)
	{
		source->sink[i].frameCallback(source,&source->sink[i], frame, source->sink[i].frameCallbackUserData);
	}

	return 0;
}
