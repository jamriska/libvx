#ifndef _LIBVX_INT_CONTEXT_H_
#define _LIBVX_INT_CONTEXT_H_ 1

#include "_object.h"

#include "vx/frame.h"
#include "vx/context.h"

struct vx_context {
	vx_object _object;

	vx_frame_cb_t* frameCallback;
	void* frameCallbackUserData;
} ;


#endif
