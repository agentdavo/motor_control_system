#ifndef CENTRAL_NEXUS_H
#define CENTRAL_NEXUS_H

#include "plugin_interface.h"

typedef struct {
    float phase_voltage[NUM_MOTORS][NUM_PHASES];
    float i_alpha[NUM_MOTORS];
    float i_beta[NUM_MOTORS];
    float voltage[NUM_MOTORS][NUM_PHASES];
    float rotor_angle[NUM_MOTORS];
    float rotor_speed[NUM_MOTORS];
    float voltage_limit[NUM_MOTORS];
    float adc_value[NUM_MOTORS][NUM_PHASES];
} motor_data_t;

bool central_nexus_init(int num_motors);
void central_nexus_register_plugin(plugin_t *plugin);
void central_nexus_process_plugins(void);
bool central_nexus_terminate(void);

#endif // CENTRAL_NEXUS_H
