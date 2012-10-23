#ifndef _LIBVX_INT_OBJECT_H_
#define _LIBVX_INT_OBJECT_H_ 1


typedef struct vx_object {
	int id;
	int refCount;
} vx_object;

#define VX_OBJECT(ptr) \
	((vx_object*)ptr)


#endif
