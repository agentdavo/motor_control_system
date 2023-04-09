#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

typedef struct plugin {
    const char* name;
    bool (*init)(void);
    bool (*terminate)(void);
    bool (*process)(void* data);
} plugin_t;

typedef struct plugin_registration {
    plugin_t* plugin;
    SemaphoreHandle_t semaphore;
} plugin_registration_t;

bool plugin_interface_init(void);
bool plugin_interface_register_plugin(plugin_t* plugin);
void plugin_interface_process_plugins(void* data);
bool plugin_interface_terminate(void);

#endif
