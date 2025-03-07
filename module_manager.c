#include "module_manager.h"

ModuleList* create_module_list(size_t initial_capacity) {
    ModuleList* list = malloc(sizeof(ModuleList));
    list->modules = malloc(sizeof(Module) * initial_capacity);
    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

void destroy_module_list(ModuleList* list) {
    if (list) {
        for (size_t i = 0; i < list->count; i++) {
            free(list->modules[i].name);
        }
        free(list->modules);
        free(list);
    }
}

int add_module(ModuleList* list, Module module) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        Module* new_modules = realloc(list->modules, sizeof(Module) * list->capacity);
        if (!new_modules) return -1;
        list->modules = new_modules;
    }
    list->modules[list->count++] = module;
    return 0;
}

int remove_module(ModuleList* list, int id) {
    for (size_t i = 0; i < list->count; i++) {
        if (list->modules[i].id == id) {
            free(list->modules[i].name);
            for (size_t j = i; j < list->count - 1; j++) {
                list->modules[j] = list->modules[j+1];
            }
            list->count--;
            return 0;
        }
    }
    return -1;
}

Module* find_module(ModuleList* list, int id) {
    for (size_t i = 0; i < list->count; i++) {
        if (list->modules[i].id == id) {
            return &list->modules[i];
        }
    }
    return NULL;
}
