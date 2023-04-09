#include "encoder_plugin.h"
#include "hal_encoder.h"

// Encoder plugin initialization function
bool encoder_plugin_init(void) {
    // Initialize the encoder hardware
    hal_encoder_init();
    return true;
}

// Encoder plugin processing function
bool encoder_plugin_process(motor_data_t *motor_data) {
    // Read the encoder position and velocity
    int position = hal_encoder_get_position();
    int velocity = hal_encoder_get_velocity();

    // Update motor data with encoder position and velocity
    for (int i = 0; i < NUM_MOTORS; i++) {
        motor_data->encoder_position[i] = position;
        motor_data->encoder_velocity[i] = velocity;
    }

    // Return true to indicate success
    return true;
}

// Encoder plugin termination function
bool encoder_plugin_terminate(void) {
    // Clean up resources used by the encoder plugin
    hal_encoder_terminate();
    return true;
}

// Encoder plugin struct
plugin_t encoder_plugin = {
    .name = "Encoder Plugin",
    .init = encoder_plugin_init,
    .process = encoder_plugin_process,
    .terminate = encoder_plugin_terminate
};
