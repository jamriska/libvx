#include "null_backend.h"

#include "_source.h"

#include <stdlib.h>

typedef struct {
	vx_source super;
} vx_source_null;


vx_source*
vx_source_null_create()
{
	vx_source_null* s = malloc(sizeof(vx_source_null));
	return (vx_source*)s;
}


int vx_source_null_open(vx_source_null* s, const char* n)
{
	return 0;
}