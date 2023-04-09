#include "adc_plugin.h"
#include <stdlib.h>

static void adc_plugin_task(void *params);

bool adc_plugin_init(void) {
    // Initialize the ADC plugin
    for (int i = 0; i < NUM_MOTORS; i++) {
        adc_plugin.config.channel = i;
        adc_plugin.config.callback = NULL;
        hal_adc_init(&adc_plugin.config);
        adc_plugin.data_ready_sem = xSemaphoreCreateBinary();
        adc_plugin.data_queue = xQueueCreate(10, sizeof(float));
        xTaskCreate(adc_plugin_task, "ADC Plugin Task", 256, NULL, 2, &adc_plugin.task_handle);
    }
    return true;
}

bool adc_plugin_process(motor_data_t *motor_data) {
    // Wait for ADC data to be available
    xSemaphoreTake(adc_plugin.data_ready_sem, portMAX_DELAY);

    // Get ADC data from the queue
    float data;
    xQueueReceive(adc_plugin.data_queue, &data, 0);

    // Update motor_data with ADC data
    motor_data->adc_data[adc_plugin.config.channel] = data;

    // Return true to indicate success
    return true;
}

bool adc_plugin_terminate(void) {
    // Clean up resources used by the ADC plugin
    for (int i = 0; i < NUM_MOTORS; i++) {
        vTaskDelete(adc_plugin.task_handle);
        vSemaphoreDelete(adc_plugin.data_ready_sem);
        vQueueDelete(adc_plugin.data_queue);
        hal_adc_terminate();
    }

    return true;
}

static void adc_plugin_task(void *params) {
    // Continuously sample ADC data
    while (1) {
        // Start ADC conversion
        hal_adc_start_conversion();

        // Wait for ADC conversion to complete
        xSemaphoreTake(adc_plugin.data_ready_sem, portMAX_DELAY);

        // Get ADC data
        float data = hal_adc_get_data();

        // Send ADC data to the queue
        xQueueSendToBack(adc_plugin.data_queue, &data, 0);
    }
}
