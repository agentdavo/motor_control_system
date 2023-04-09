#include "plugin_interface.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void plugin_init(void) {
    // Initialize the plugin system
    plugin_system_t* plugin_system = plugin_system_get();
    plugin_system->num_plugins = 0;
    for (int i = 0; i < MAX_PLUGINS; i++) {
        plugin_system->plugins[i] = NULL;
    }

    // Initialize the plugin queue
    plugin_system->plugin_queue = xQueueCreate(MAX_PLUGINS, sizeof(plugin_t*));
}

bool plugin_register(plugin_t* plugin) {
    // Get the plugin system
    plugin_system_t* plugin_system = plugin_system_get();

    // Add the plugin to the plugin system
    if (plugin_system->num_plugins < MAX_PLUGINS) {
        plugin_system->plugins[plugin_system->num_plugins++] = plugin;
        return true;
    }
    else {
        return false;
    }
}

void plugin_process_all(motor_data_t* motor_data) {
    // Get the plugin system
    plugin_system_t* plugin_system = plugin_system_get();

    // Iterate over each registered plugin and call its process function
    for (int i = 0; i < plugin_system->num_plugins; i++) {
        plugin_t* plugin = plugin_system->plugins[i];
        xTaskNotifyGive(plugin_system->plugin_tasks[i]);
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        plugin->process(motor_data);
    }
}

void plugin_task(void* pvParameters) {
    // Get the plugin system and the plugin index
    plugin_system_t* plugin_system = plugin_system_get();
    int plugin_index = (int)pvParameters;

    // Infinite loop
    while (1) {
        // Wait for a notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Call the plugin's process function
        plugin_t* plugin = plugin_system->plugins[plugin_index];
        plugin->process(NULL);

        // Notify the central nexus task
        xTaskNotifyGive(plugin_system->central_nexus_task);
    }
}
