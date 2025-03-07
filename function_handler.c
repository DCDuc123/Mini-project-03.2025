#include "error_handler.h"
#include "function_handler.h"
#include "bitmask_utils.h"
#include <stdio.h>

void engine_control(Module* module, int action) {
    if (action == 1) { // Turn on
        if (is_status_set(module->status, STATUS_ON)) {
            printf("Engine already ON!\n");
            set_status_bit(&module->status, STATUS_ERROR);
            handle_error();
        } else {
            set_status_bit(&module->status, STATUS_ON);
            printf("Engine started.\n");
        }
    } else if (action == 0) { // Turn off
        clear_status_bit(&module->status, STATUS_ON);
        printf("Engine stopped.\n");
    }
}

void abs_control(Module* module, int action) {
    // Similar implementation for ABS
}
