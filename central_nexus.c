#include "central_nexus.h"
#include <stdlib.h>
#include <string.h>
#include "plugin_interface.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

static SemaphoreHandle_t plugin_semaphore;
static QueueHandle_t plugin_queue;
static plugin_t* plugins[MAX_NUM_PLUGINS];
static int num_plugins = 0;
static motor_data_t motor_data[NUM_MOTORS];

bool central_nexus_init(int num_motors) {
    // Initialize the plugin semaphore and queue
    plugin_semaphore = xSemaphoreCreateMutex();
    plugin_queue = xQueueCreate(MAX_NUM_PLUGINS, sizeof(plugin_t*));

    if (plugin_semaphore == NULL || plugin_queue == NULL) {
        return false;
    }

    // Initialize motor data
    memset(&motor_data, 0, sizeof(motor_data_t) * NUM_MOTORS);

    return true;
}

bool central_nexus_register_plugin(plugin_t *plugin) {
    if (num_plugins >= MAX_NUM_PLUGINS) {
        return false;
    }

    // Add the plugin to the plugin queue
    if (xQueueSend(plugin_queue, &plugin, 0) != pdPASS) {
        return false;
    }

    // Increment the number of registered plugins
    num_plugins++;

    return true;
}

bool central_nexus_process_plugins(void) {
    // Get the next plugin from the queue
    plugin_t* plugin;
    if (xQueueReceive(plugin_queue, &plugin, 0) != pdPASS) {
        return false;
    }

    // Lock the plugin semaphore
    if (xSemaphoreTake(plugin_semaphore, 0) != pdPASS) {
        return false;
    }

    // Process the plugin
    bool result = plugin->process(&motor_data);

    // Unlock the plugin semaphore
    xSemaphoreGive(plugin_semaphore);

    // Add the plugin back to the queue
    xQueueSendToBack(plugin_queue, &plugin, 0);

    return result;
}

bool central_nexus_terminate(void) {
    // Terminate all registered plugins
    while (num_plugins > 0) {
        plugin_t* plugin;

        // Get the next plugin from the queue
        if (xQueueReceive(plugin_queue, &plugin, 0) == pdPASS) {
            // Lock the plugin semaphore
            if (xSemaphoreTake(plugin_semaphore, 0) == pdPASS) {
                // Terminate the plugin
                plugin->terminate();

                // Unlock the plugin semaphore
                xSemaphoreGive(plugin_semaphore);

                // Decrement the number of registered plugins
                num_plugins--;
            }
        }
    }

    // Delete the plugin semaphore and queue
    vSemaphoreDelete(plugin_semaphore);
    vQueueDelete(plugin_queue);

    return true;
}

motor_data_t* central_nexus_get_motor_data(int motor_index) {
    // Return a pointer to the motor data for the specified motor index
    if (motor_index < NUM_MOTORS) {
        return &motor_data[motor_index];
    }
    else {
        return NULL;
    }
}

bool central_nexus_register_plugin(plugin_t* plugin) {
    // Add the plugin to the list of registered plugins
    if (num_plugins < MAX_PLUGINS) {
        plugins[num_plugins++] = plugin;
        return true;
    }
    else {
        return false;
    }
}

void central_nexus_process_plugins(void) {
// Iterate over each registered plugin and call its process function
for (int i = 0; i < num_plugins; i++) {
plugin_t* plugin = plugins[i];
plugin->process(motor_data);
}
}

motor_data_t* central_nexus_get_motor_data(int motor_index) {
// Return a pointer to the motor data for the specified motor index
if (motor_index < NUM_MOTORS) {
return &motor_data[motor_index];
}
else {
return NULL;
}
}

bool central_nexus_register_plugin(plugin_t* plugin) {
// Add the plugin to the list of registered plugins
if (num_plugins < MAX_PLUGINS) {
plugins[num_plugins++] = plugin;
return true;
}
else {
return false;
}
}

void central_nexus_set_motor_state(int motor_index, motor_state_t state) {
// Set the motor state for the specified motor index
if (motor_index < NUM_MOTORS) {
motor_data[motor_index].state = state;
}
}

motor_state_t central_nexus_get_motor_state(int motor_index) {
// Get the motor state for the specified motor index
if (motor_index < NUM_MOTORS) {
return motor_data[motor_index].state;
}
else {
return MOTOR_STATE_IDLE;
}
}

void central_nexus_set_motor_voltage_limit(int motor_index, float voltage_limit) {
// Set the voltage limit for the specified motor index
if (motor_index < NUM_MOTORS) {
motor_data[motor_index].voltage_limit = voltage_limit;
}
}

float central_nexus_get_motor_voltage_limit(int motor_index) {
// Get the voltage limit for the specified motor index
if (motor_index < NUM_MOTORS) {
return motor_data[motor_index].voltage_limit;
}
else {
return 0.0f;
}
}
