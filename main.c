#include "config.h"
#include "central_nexus.h"
#include "plugin_interface.h"
#include "plugin_interface/pwm_plugin.h"
#include "plugin_interface/adc_plugin.h"
#include "plugin_interface/encoder_plugin.h"

int main(void) {
    // Initialize the central nexus with 1 motor
    central_nexus_init(1);

    // Initialize the plugin interface
    plugin_interface_init();

    // Register plugins
    pwm_plugin_t pwm_plugin = {
        .name = "pwm_plugin",
        .init = hal_pwm_init,
        .terminate = hal_pwm_stop,
        .process = pwm_plugin_process,
        .priority = 2,
        .stack_size = configMINIMAL_STACK_SIZE
    };
    plugin_interface_register_plugin((plugin_t*)&pwm_plugin);

    adc_plugin_t adc_plugin = {
        .name = "adc_plugin",
        .init = hal_adc_init,
        .terminate = NULL,
        .process = adc_plugin_process,
        .priority = 3,
        .stack_size = configMINIMAL_STACK_SIZE
    };
    plugin_interface_register_plugin((plugin_t*)&adc_plugin);

    encoder_plugin_t encoder_plugin = {
        .name = "encoder_plugin",
        .init = biss_encoder_init,
        .terminate = biss_encoder_terminate,
        .process = biss_encoder_process,
        .priority = 1,
        .stack_size = configMINIMAL_STACK_SIZE
    };
    plugin_interface_register_plugin((plugin_t*)&encoder_plugin);

    // Start the scheduler
    vTaskStartScheduler();

    return 0;
}
