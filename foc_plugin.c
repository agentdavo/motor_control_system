#include "foc_plugin.h"
#include "central_nexus.h"
#include <math.h>

#define NUM_SINE_SAMPLES 3600 // Number of samples in the sine/cosine lookup table
#define SINE_TABLE_SCALE (NUM_SINE_SAMPLES / (2 * M_PI)) // Scale factor to convert angle to table index

// Generate sine/cosine lookup table
float sin_table[NUM_SINE_SAMPLES];
float cos_table[NUM_SINE_SAMPLES];
for (int i = 0; i < NUM_SINE_SAMPLES; i++) {
    float angle = i / SINE_TABLE_SCALE;
    sin_table[i] = sinf(angle);
    cos_table[i] = cosf(angle);
}

bool foc_plugin_init(void) {
    // Initialize the FOC plugin
    for (int i = 0; i < NUM_MOTORS; i++) {
        // Calculate sine and cosine values for encoder offset
        float theta_offset = foc_plugin.theta_offset[i];
        foc_plugin.sin_theta[i] = sinf(theta_offset);
        foc_plugin.cos_theta[i] = cosf(theta_offset);
    }
    return true;
}

bool foc_plugin_process(motor_data_t *motor_data) {
  
  // Use sine/cosine lookup table in FOC algorithm
  float sin_val = sin_table[angle * SINE_TABLE_SCALE];
  float cos_val = cos_table[angle * SINE_TABLE_SCALE];
  
    // Iterate over each motor
    for (int i = 0; i < NUM_MOTORS; i++) {
        // Get current phase voltages
        float v_u = motor_data->phase_voltage[i][0];
        float v_v = motor_data->phase_voltage[i][1];
        float v_w = motor_data->phase_voltage[i][2];

        // Convert voltage to alpha-beta coordinates
        float v_alpha = (2.0f/3.0f) * (v_u - 0.5f*v_v - 0.5f*v_w);
        float v_beta = (2.0f/3.0f) * (0.866f*v_v - 0.866f*v_w);

        // Get current rotor angle and speed
        float angle = motor_data->rotor_angle[i];
        float speed = motor_data->rotor_speed[i];

        // Calculate target current components in alpha-beta coordinates
        float i_alpha_ref = foc_plugin.i_mag[i] * cosf(foc_plugin.theta_ref[i]);
        float i_beta_ref = foc_plugin.i_mag[i] * sinf(foc_plugin.theta_ref[i]);

        // Calculate current error in alpha-beta coordinates
        float i_alpha_error = i_alpha_ref - motor_data->i_alpha[i];
        float i_beta_error = i_beta_ref - motor_data->i_beta[i];

        // Calculate error integral
        foc_plugin.error_integral[i][0] += i_alpha_error * FOC_PLUGIN_DT;
        foc_plugin.error_integral[i][1] += i_beta_error * FOC_PLUGIN_DT;

        // Calculate error derivative
        float error_d_alpha = (i_alpha_error - foc_plugin.last_error[i][0]) / FOC_PLUGIN_DT;
        float error_d_beta = (i_beta_error - foc_plugin.last_error[i][1]) / FOC_PLUGIN_DT;

        // Store current error as last error
        foc_plugin.last_error[i][0] = i_alpha_error;
        foc_plugin.last_error[i][1] = i_beta_error;

        // Calculate PID output in alpha-beta coordinates
        float pid_out_alpha = foc_plugin.kp[i][0] * i_alpha_error +
                              foc_plugin.ki[i][0] * foc_plugin.error_integral[i][0] +
                              foc_plugin.kd[i][0] * error_d_alpha;
        float pid_out_beta = foc_plugin.kp[i][1] * i_beta_error +
                             foc_plugin.ki[i][1] * foc_plugin.error_integral[i][1] +
                             foc_plugin.kd[i][1] * error_d_beta;

        // Calculate voltage vector components in alpha-beta coordinates
        float v_alpha_ref = pid_out_alpha + motor_data->i_alpha[i];
        float v_beta_ref = pid_out_beta + motor_data->i_beta[i];

        // Convert voltage vector components to abc coordinates
        float v_a = v_alpha_ref;
        float v_b = -0.5f*v_alpha_ref + 0.866f*v_beta_ref;
        float v_c = -0.5f*v_alpha_ref - 0.866f*v_beta_ref;

    // Convert abc voltage to time-domain voltage using inverse Park transform (cont'd)
    float v_beta_time = v_a * sin_val + v_b * cos_val;
    float v_0_time = v_c;

    // Limit voltage to maximum allowed voltage
    float voltage_limit = motor_data->voltage_limit[i];
    float max_voltage = max(fabs(v_alpha_time), fabs(v_beta_time));
    if (max_voltage > voltage_limit) {
        v_alpha_time *= voltage_limit / max_voltage;
        v_beta_time *= voltage_limit / max_voltage;
        v_0_time *= voltage_limit / max_voltage;
    }

    // Store time-domain voltage in motor_data
    motor_data->voltage[i][0] = v_alpha_time;
    motor_data->voltage[i][1] = v_beta_time;
    motor_data->voltage[i][2] = v_0_time;

    // Update rotor angle and speed
    float delta_angle = (speed * FOC_PLUGIN_DT) + (0.5f * foc_plugin.acceleration[i] * FOC_PLUGIN_DT * FOC_PLUGIN_DT);
    motor_data->rotor_angle[i] += delta_angle;
    motor_data->rotor_speed[i] += (foc_plugin.acceleration[i] * FOC_PLUGIN_DT);

    // Clamp rotor angle to 0 to 2*pi
    if (motor_data->rotor_angle[i] > 2.0f * PI) {
        motor_data->rotor_angle[i] -= 2.0f * PI;
    }
    else if (motor_data->rotor_angle[i] < 0.0f) {
        motor_data->rotor_angle[i] += 2.0f * PI;
    }
}

// Return true to indicate success
return true;
}


bool foc_plugin_terminate() {
    // Clean up resources used by the FOC plugin
    for (int i = 0; i < NUM_MOTORS; i++) {
        // Free memory allocated for PID parameters
        free(foc_plugin.kp[i]);
        free(foc_plugin.ki[i]);
        free(foc_plugin.kd[i]);

        // Free memory allocated for error integral
        free(foc_plugin.error_integral[i]);

        // Free memory allocated for last error
        free(foc_plugin.last_error[i]);
    }

    return true;
}
