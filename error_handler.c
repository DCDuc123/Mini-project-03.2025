#include "error_handler.h"
#include <stdio.h>

jmp_buf env;

void handle_error() {
    printf("Error encountered! Entering safe state.\n");
    longjmp(env, 1);
}
