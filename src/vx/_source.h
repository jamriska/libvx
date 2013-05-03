#ifndef _LIBVX_INT_SOURCE_H_
#define _LIBVX_INT_SOURCE_H_ 1

#include "vx/source.h"
#include "_object.h"
#include "_context.h"

typedef int (*vx_source_enumerate_cb_t)(vx_source * s,vx_source_description** e);
typedef int (*vx_source_open_cb_t)(vx_source * s,const char* n);
typedef int (*vx_source_close_cb_t)(vx_source * s);
typedef int (*vx_source_set_state_cb_t)(vx_source * s,int newstate);
typedef int (*vx_source_get_state_cb_t)(vx_source * s,int *state);

struct vx_source {

	struct vx_object _object;

	int state;

	vx_source_enumerate_cb_t enumerate;
	vx_source_open_cb_t open;
	vx_source_close_cb_t close;
	vx_source_get_state_cb_t get_state;
	vx_source_set_state_cb_t set_state;

	vx_context* context;

};


#define VX_SOURCE_CAST(ptr) \
	((vx_source*)(ptr))



#endif
