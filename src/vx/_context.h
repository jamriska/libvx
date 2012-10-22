#ifndef _LIBVX_INT_CONTEXT_H_
#define _LIBVX_INT_CONTEXT_H_ 1

#include "vx/frame.h"
#include "vx/context.h"

struct vx_context {
	int id;
	int refCount;

	vx_frame_cb_t* frameCallback;
	void* frameCallbackUserData;
};


#endif
