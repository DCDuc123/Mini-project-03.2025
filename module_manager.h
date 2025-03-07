#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct ModuleData {
    union {
        struct { int engine_rpm; } engine;
        struct { int brake_pressure; } abs;
        struct { int light_intensity; } light;
        struct { float temperature; } climate;
        struct { float temp_reading; } temp_sensor;
    };
} ModuleData;

typedef struct Module {
    int id;
    char* name;
    uint8_t status;
    ModuleData data;
    void (*control_action)(struct Module*, int);
} Module;

typedef struct ModuleList {
    Module* modules;
    size_t count;
    size_t capacity;
} ModuleList;

ModuleList* create_module_list(size_t initial_capacity);
void destroy_module_list(ModuleList* list);
int add_module(ModuleList* list, Module module);
int remove_module(ModuleList* list, int id);
Module* find_module(ModuleList* list, int id);

#endif
