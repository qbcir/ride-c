#ifndef __RIDE_C_RIDE_H_20461__
#define __RIDE_C_RIDE_H_20461__

typedef struct ride_compiler_s {
    void* _ctx;
} ride_compiler;

int ride_init_compiler(ride_compiler* compiler);
char* ride_compile(ride_compiler* compiler, char* code);
int ride_destroy_compiler(ride_compiler* compiler);

#endif /* __RIDE_C_RIDE_H_20461__ */
