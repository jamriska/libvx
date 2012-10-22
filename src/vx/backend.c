#include "vx/backend.h"

#include <stdlib.h>

//struct vx_backend {
//	int id;
//	int refCount;
//};

//vx_backend*
//vx_backend_create(const char* n) {
//	vx_backend* c = malloc(sizeof(vx_backend));
//	c->id = 0;
//	c->refCount = 0;
//	return c;
//}

//int
//vx_backend_ref(vx_backend *c) {
//	if (c) { c->refCount++; return 0;}
//	return -1;
//}

//int
//vx_backend_unref(vx_backend *c) {
//	if (c) {
//		c->refCount--;

//		if (c->refCount == 0) {
//			free(c);
//		}

//		return 0;
//	}
//	return -1;
//}
