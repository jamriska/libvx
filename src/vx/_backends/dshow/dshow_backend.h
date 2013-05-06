#ifndef _LIBVX_VX_DSHOW_BACKEND_H_
#define _LIBVX_VX_DSHOW_BACKEND_H_ 1

#include <_source.h>

#if defined (__cplusplus)
	#define VX_CREATE_CALL extern "C"
#else
	#define VX_CREATE_CALL extern
#endif

VX_CREATE_CALL void* vx_source_dshow_create();

#endif
