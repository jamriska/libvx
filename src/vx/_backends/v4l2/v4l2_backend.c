#include "_source.h"


typedef struct {
	int fd;
} vx_source_v4l2_impl;


int v4l2_source_open(vx_source* s, const char* n)
{
	vx_source_v4l2_impl* impl = (vx_source_v4l2_impl*)s->impl;
}
