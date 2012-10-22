#include "_source.h"
#include "_context.h"

#include <stdlib.h>

vx_source *
vx_source_create(const char *n) {
	vx_source* s = malloc(sizeof(vx_source));
	s->id = 0;
	s->refCount = 0;
	return s;
}

int
vx_source_ref(vx_source *s) {
	if (s) { s->refCount++; return 0;}
	return -1;
}

int
vx_source_unref(vx_source *s) {
	if (s) {
		s->refCount--;

		if (s->refCount == 0) {
			free(s);
		}

		return 0;
	}
	return -1;
}

int
vx_source_open(vx_source *s, const char* n)
{
	return (s ? s->open(s,n) : -1);
}

int
vx_source_close(vx_source *s)
{
	return (s ? s->close(s) : -1);
}

int
vx_source_set_state(vx_source *s, int newstate)
{
	return (s ? s->set_state(s,newstate) : -1);
}

int
vx_source_get_state(vx_source *s, int* state)
{
	return (s ? s->get_state(s,state) : -1);
}

int
vx_source_add_context(vx_source* s, vx_context* c)
{
}
