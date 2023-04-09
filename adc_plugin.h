#ifndef ADC_PLUGIN_H
#define ADC_PLUGIN_H

#include "plugin_interface.h"
#include "hal_adc.h"
#include "central_nexus.h"

typedef struct {
    plugin_t plugin;
    hal_adc_config_t config;
    SemaphoreHandle_t data_ready_sem;
    QueueHandle_t data_queue;
    TaskHandle_t task_handle;
} adc_plugin_t;

bool adc_plugin_init(void);
bool adc_plugin_process(motor_data_t *motor_data);
bool adc_plugin_terminate(void);

#endif /* ADC_PLUGIN_H */
