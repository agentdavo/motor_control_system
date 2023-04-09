#include "adc_plugin.h"
#include "central_nexus.h"

bool adc_plugin_init(void) {
    // Initialize the ADC plugin
    return true;
}

bool adc_plugin_process(motor_data_t *motor_data) {
    // Iterate over each motor
    for (int i = 0; i < NUM_MOTORS; i++) {
        // Read ADC values for each phase
        for (int j = 0; j < NUM_PHASES; j++) {
            motor_data->adc_value[i][j] = hal_adc_read(i, j);
        }
    }

    // Return true to indicate success
    return true;
}

bool adc_plugin_terminate(void) {
    // Clean up resources used by the ADC plugin
    return true;
}
