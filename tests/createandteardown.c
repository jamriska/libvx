

#include <vx/vx.h>



int main(int argc, char** argv) {

    vx_context* c = vx_context_create("context");

    vx_context_ref(c);


    /* do someting */


    vx_context_unref(c);

    return 0;
}
