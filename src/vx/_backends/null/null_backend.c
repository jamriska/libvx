#include "null_backend.h"

#include "_source.h"

#include <stdlib.h>

typedef struct vx_source_null {
	vx_source super;
} vx_source_null;


int vx_source_null_open(void* s, const char* n)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_null_close(void* s)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_null_set_state(void* s,int state)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_null_get_state(void* s,int* state)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}



void*
vx_source_null_create()
{
	vx_source_null* s = malloc(sizeof(struct vx_source_null));
	
	VX_SOURCE(s)->open = vx_source_null_open;
	VX_SOURCE(s)->close = vx_source_null_close;
	VX_SOURCE(s)->set_state = vx_source_null_set_state;
	VX_SOURCE(s)->get_state = vx_source_null_get_state;
	
	return s;
}
