#include "_source.h"
#include "_context.h"

#include <stdlib.h>
#include <string.h>

#include <_backends/null/null_backend.h>
#include <_backends/gst010/gst010_backend.h>

void *
vx_source_create(const char *n) {
	if (0 == strcmp("null",n)) {
		return vx_source_null_create();
	}
	if (0 == strcmp("gstreamer",n)) {
		return vx_source_gstreamer_create();
	}
	return NULL;
}

int
vx_source_ref(void *s) {
	if (s) { VX_OBJECT(s)->refCount++; return 0;}
	return -1;
}

int
vx_source_unref(void *s) {
	if (s) {
		VX_OBJECT(s)->refCount--;

		if (VX_OBJECT(s)->refCount == 0) {
			free(s);
		}

		return 0;
	}
	return -1;
}

int
vx_source_open(void *s, const char* n)
{
	return VX_SOURCE(s)->open(s,n);
}

int
vx_source_close(void *s)
{
	return VX_SOURCE(s)->close(s);
}

int
vx_source_set_state(void *s, int newstate)
{
	return VX_SOURCE(s)->set_state(s,newstate);
}

int
vx_source_get_state(void *s, int* state)
{
	return VX_SOURCE(s)->get_state(s,state);
}

int
vx_source_add_context(void* s, vx_context* c)
{
	return 0;
}
