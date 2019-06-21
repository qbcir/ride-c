#include "ride.h"
#include "ridejs.h"

#include "jsapi.h"
#include "js/Initialization.h"
#include "js/Conversions.h"

#include <stdlib.h>
#include <string.h>

static JSClassOps global_ops = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    JS_GlobalObjectTraceHook
};

/* The class of the global object. */
static JSClass global_class = {
    "global",
    JSCLASS_GLOBAL_FLAGS,
    &global_ops
};

JS::PersistentRootedObject global;

int ride_init_compiler(ride_compiler* compiler)
{
    JS_Init();
    compiler->_ctx = JS_NewContext(1024L * 1024 * 1024);
    auto ctx = (JSContext*)compiler->_ctx;
    if (!ctx)
    {
        return 1;
    }
    if (!JS::InitSelfHostedCode(ctx))
    {
        return 1;
    }
    JS_BeginRequest(ctx);
    JS::CompartmentOptions options;
    global.init(ctx, JS_NewGlobalObject(ctx, &global_class, nullptr, JS::FireOnNewGlobalHook, options));
    if (!global)
    {
        return 1;
    }
    JS_EnterCompartment(ctx, global);
    JS_InitStandardClasses(ctx, global);
    //JS_InitCTypesClass(ctx, global);

    JS::RootedValue rval(ctx);
    const char *filename = "noname";
    JS::CompileOptions opts(ctx);
    opts.setFileAndLine(filename, 1);
    bool ok = JS::Evaluate(ctx, opts, ride_min_js, strlen(ride_min_js), &rval);
    if (!ok)
    {
        return 1;
    }
    ride_compile_result_t res;
    memset(&res, 0, sizeof(ride_compile_result_t));
    if (ride_compile(compiler, "true", &res) != 0) // warmup
    {
        return 1;
    }
    int error = res.error != NULL ? 1 : 0;
    ride_destroy_compile_result(&res);
    return error;
}

int ride_compile(ride_compiler* compiler, const char* code, ride_compile_result_t* result)
{
    auto ctx = (JSContext*)compiler->_ctx;
    JS::RootedValue ret(ctx);
    JS::RootedString code_str(ctx, JS_NewStringCopyZ(ctx, code));
    JS::AutoValueArray<1> compile_args(ctx);
    compile_args[0].setString(code_str);
    JS::HandleValueArray args(compile_args);

    JS::RootedValue ridejs(ctx);
    JS::RootedObject RideJS(ctx);
    if (!JS_GetProperty(ctx, global, "RideJS", &ridejs))
    {
        return 1;
    }
    if (!JS_ValueToObject(ctx, ridejs, &RideJS))
    {
        return 1;
    }
    if (!JS_CallFunctionName(ctx, RideJS, "compile", args, &ret))
    {
        return 1;
    }
    // ret
    JS::RootedObject ret_obj(ctx);
    if (!JS_ValueToObject(ctx, ret, &ret_obj))
    {
        return 1;
    }
    // error ?
    JS::RootedValue error_val(ctx);
    if (!JS_GetProperty(ctx, ret_obj, "error", &error_val))
    {
        return 1;
    }
    if (JS_TypeOfValue(ctx, error_val) != JSTYPE_VOID)
    {
        auto error_str = JS::ToString(ctx, error_val);
        size_t buf_size = JS_GetStringLength(error_str);
        result->complexity = 0;
        result->base64 = NULL;
        result->error = (char*)malloc(buf_size + 1);
        buf_size = JS_EncodeStringToBuffer(ctx, error_str, result->error, buf_size+1);
        result->error[buf_size] = '\0';
        return 0;
    }
    // result
    JS::RootedObject result_obj(ctx);
    JS::RootedValue result_val(ctx);
    if (!JS_GetProperty(ctx, ret_obj, "result", &result_val))
    {
        return 1;
    }
    if (JS_TypeOfValue(ctx, result_val) != JSTYPE_OBJECT)
    {
        return 1;
    }
    if (!JS_ValueToObject(ctx, result_val, &result_obj))
    {
        return 1;
    }
    JS::RootedValue complexity_val(ctx);
    if (!JS_GetProperty(ctx, result_obj, "complexity", &complexity_val))
    {
        return 1;
    }
    uint32_t complexity;
    if (!JS::ToUint32(ctx, complexity_val, &complexity))
    {
        return 1;
    }
    JS::RootedValue base64_val(ctx);
    if (!JS_GetProperty(ctx, result_obj, "base64", &base64_val))
    {
        return 1;
    }
    JSString* base64_js_str = JS::ToString(ctx, base64_val);
    if (!base64_js_str)
    {
        return 1;
    }
    size_t buf_size = JS_GetStringLength(base64_js_str);
    result->error = NULL;
    result->base64 = (char*)malloc(buf_size + 1);
    buf_size = JS_EncodeStringToBuffer(ctx, base64_js_str, result->base64, buf_size+1);
    result->base64[buf_size] = '\0';
    result->complexity = complexity;
    return 0;
}

int ride_destroy_compiler(ride_compiler* compiler)
{
    auto ctx = (JSContext*)compiler->_ctx;
    JS_LeaveCompartment(ctx, 0);
    JS_EndRequest(ctx);
    JS_DestroyContext(ctx);
    JS_ShutDown();
    return 0;
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
