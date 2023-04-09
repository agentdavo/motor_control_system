#include "pwm_plugin.h"
#include "hal_gpio.h"
#include "hal_pwm.h"

bool pwm_plugin_init(plugin_config_t config) {
    // Initialize the PWM hardware
    hal_pwm_init();

    return true;
}

bool pwm_plugin_process(motor_data_t *motor_data) {
    // Calculate PWM duty cycles based on time-domain voltage
    for (int i = 0; i < NUM_MOTORS; i++) {
        // Get the time-domain voltage for this motor
        float v_alpha = motor_data->voltage[i][0];
        float v_beta = motor_data->voltage[i][1];
        float v_0 = motor_data->voltage[i][2];

        // Convert time-domain voltage to abc coordinates using inverse Clarke transform
        float v_a = v_alpha;
        float v_b = -0.5f*v_alpha + 0.866f*v_beta;
        float v_c = -0.5f*v_alpha - 0.866f*v_beta;

        // Calculate PWM duty cycles based on abc voltages
        float duty_u = (v_a + v_b + v_c + v_0) / 2.0f;
        float duty_v = (-v_a + v_b + v_c + v_0) / 2.0f;
        float duty_w = (-v_a - v_b + v_c + v_0) / 2.0f;

        // Set the PWM duty cycles for this motor
        hal_pwm_set_duty_cycle(i, HAL_PWM_U_CHANNEL, duty_u);
        hal_pwm_set_duty_cycle(i, HAL_PWM_V_CHANNEL, duty_v);
        hal_pwm_set_duty_cycle(i, HAL_PWM_W_CHANNEL, duty_w);
    }

    return true;
}

bool pwm_plugin_terminate(void) {
    // Clean up resources used by the PWM plugin
    hal_pwm_stop();

    return true;
}
