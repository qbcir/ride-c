#include "duktape.h"
#include "ride.h"
#include "ridejs.h"

//#include "v8.h"

int ride_init_compiler(ride_compiler* compiler)
{
    duk_int_t ret;
    compiler->_ctx = duk_create_heap_default();
    ret = duk_peval_string(compiler->_ctx, ride_min_js);
    if (ret != 0)
    {
        return ret;
    }
    duk_get_global_string(compiler->_ctx, "RideJS");
    ride_compile(compiler, "true"); // warmup
    return 0;
}

char* ride_compile(ride_compiler* compiler, char* code)
{
    duk_int_t ret;
    duk_push_string(compiler->_ctx, "compile");
    duk_push_string(compiler->_ctx, code);
    ret = duk_pcall_prop(compiler->_ctx, -3, 1);
    duk_get_prop_string(compiler->_ctx, -1, "result");
    duk_get_prop_string(compiler->_ctx, -1, "complexity");
    const char* res = duk_to_string(compiler->_ctx, -1);
    printf("res=%s\n", res);
    duk_pop_3(compiler->_ctx);
}

int ride_destroy_compiler(ride_compiler* compiler)
{
    duk_destroy_heap(compiler->_ctx);
}
