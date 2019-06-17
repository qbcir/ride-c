#include "ride.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    ride_compiler compiler;
    ride_compile_result_t res;
    res.base64 = NULL;
    res.error = NULL;
    ride_init_compiler(&compiler);
    for (int i = 0; i < 100; i++) {
        ride_compile(&compiler, "true || false\n", &res);
        printf("%d %s %s\n", res.complexity, res.base64, res.error);
        ride_destroy_compile_result(&res);
    }
    /*
    for (int i = 0; i < 4; i++) {
        ride_compile(&compiler, "false || true\n", &res);
        ride_destroy_compile_result(&res);
    }
    for (int i = 0; i < 4; i++) {
        ride_compile(&compiler, "true && false\n", &res);
        ride_destroy_compile_result(&res);
    }
    */
    ride_destroy_compiler(&compiler);
    return 0;
}
