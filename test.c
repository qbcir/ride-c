#include "ride.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    ride_compiler compiler;
    ride_init_compiler(&compiler);
    printf("begin\n");
    for (int i = 0; i < 4; i++) {
        ride_compile(&compiler, "true || false\n");
    }
    for (int i = 0; i < 4; i++) {
        ride_compile(&compiler, "false || true\n");
    }
    for (int i = 0; i < 4; i++) {
        ride_compile(&compiler, "true && false\n");
    }
    ride_destroy_compiler(&compiler);
    return 0;
}
