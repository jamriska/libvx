#ifndef _LIBVX_INT_SOURCE_H_
#define _LIBVX_INT_SOURCE_H_ 1

#include "vx/source.h"

typedef struct vx_context vx_context;
typedef struct vx_source vx_source;

typedef int (*vx_source_open_cb_t)(vx_source* s,const char* n);
typedef int (*vx_source_close_cb_t)(vx_source* s);
typedef int (*vx_source_set_state_cb_t)(vx_source* s,int newstate);
typedef int (*vx_source_get_state_cb_t)(vx_source* s,int *state);

struct vx_source {
	int id;
	int refCount;

	void* impl;

	vx_source_open_cb_t open;
	vx_source_close_cb_t close;
	vx_source_get_state_cb_t get_state;
	vx_source_set_state_cb_t set_state;

	vx_context* context;
};


#endif
