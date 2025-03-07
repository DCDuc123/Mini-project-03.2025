#include <stdio.h>
#include "module_manager.h"
#include "function_handler.h"
#include "error_handler.h"

int main() {
    ModuleList* list = create_module_list(5);
    
    Module engine = {
        .id = 1,
        .name = strdup("Engine"),
        .status = 0,
        .control_action = engine_control
    };
    add_module(list, engine);

    if (setjmp(env) == 0) {
        Module* eng = find_module(list, 1);
        if (eng) {
            eng->control_action(eng, 1); // Turn on
            //eng->control_action(eng, 1); // Cause error
        }
    } else {
        printf("System recovered.\n");
    }

    destroy_module_list(list);
    return 0;
}
