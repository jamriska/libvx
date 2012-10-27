#ifndef _LIBVX_INT_SOURCE_H_
#define _LIBVX_INT_SOURCE_H_ 1

#include "vx/source.h"
#include "_object.h"
#include "_context.h"

typedef int (*vx_source_open_cb_t)(void * s,const char* n);
typedef int (*vx_source_close_cb_t)(void * s);
typedef int (*vx_source_set_state_cb_t)(void * s,int newstate);
typedef int (*vx_source_get_state_cb_t)(void * s,int *state);

typedef struct vx_source {
	
	struct vx_object _object;
	
	vx_source_open_cb_t open;
	vx_source_close_cb_t close;
	vx_source_get_state_cb_t get_state;
	vx_source_set_state_cb_t set_state;

	vx_context* context;

} vx_source;


#define VX_SOURCE(ptr) \
	((vx_source*)ptr)



#endif
