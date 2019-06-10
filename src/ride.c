#include "duktape.h"
#include "ride.h"
#include "ridejs.h"

#include <stdlib.h>
#include <string.h>

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
    ride_compile_result_t warmup;
    int warmup_ret = ride_compile(compiler, "true", &warmup); // warmup
    if (warmup_ret != 0)
    {
        free(warmup.error);
        return 1;
    }
    free(warmup.base64);
    return 0;
}

int ride_compile(ride_compiler* compiler, char* code, ride_compile_result_t* result)
{
    duk_int_t ret;
    duk_push_string(compiler->_ctx, "compile");
    duk_push_string(compiler->_ctx, code);
    ret = duk_pcall_prop(compiler->_ctx, -3, 1);
    duk_get_prop_string(compiler->_ctx, -1, "error");
    if (duk_is_undefined(compiler->_ctx, -1))
    {
        duk_pop(compiler->_ctx);
        result->error = NULL;
        duk_get_prop_string(compiler->_ctx, -1, "result");
        duk_get_prop_string(compiler->_ctx, -1, "complexity");
        result->complexity = duk_get_int(compiler->_ctx, -1);
        duk_pop(compiler->_ctx);
        duk_get_prop_string(compiler->_ctx, -1, "base64");
        result->base64 = strdup(duk_safe_to_string(compiler->_ctx, -1));
        duk_pop_3(compiler->_ctx);
        return 0;
    }
    result->base64 = NULL;
    result->error = strdup(duk_safe_to_string(compiler->_ctx, -1));
    duk_pop_2(compiler->_ctx);
    return 1;
}

int ride_destroy_compiler(ride_compiler* compiler)
{
    duk_destroy_heap(compiler->_ctx);
}

void ride_destroy_compile_result(ride_compile_result_t* result)
{
    if (result->base64)
    {
        free(result->base64);
        result->base64 = NULL;
    }
    if (result->error)
    {
        free(result->error);
        result->error = NULL;
    }
}
