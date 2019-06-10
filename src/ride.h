#ifndef __RIDE_C_RIDE_H_20461__
#define __RIDE_C_RIDE_H_20461__

typedef struct ride_compiler_s {
    void* _ctx;
} ride_compiler;

typedef struct ride_compile_result_s {
    char* error;
    char* base64;
    int complexity;
} ride_compile_result_t;

int ride_init_compiler(ride_compiler* compiler);
int ride_compile(ride_compiler* compiler, char* code, ride_compile_result_t* result);
int ride_destroy_compiler(ride_compiler* compiler);
void ride_destroy_compile_result(ride_compile_result_t* result);

#endif /* __RIDE_C_RIDE_H_20461__ */
