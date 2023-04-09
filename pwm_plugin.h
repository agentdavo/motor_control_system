#ifndef PWM_PLUGIN_H
#define PWM_PLUGIN_H

#include "plugin_interface.h"
#include "central_nexus.h"

// PWM plugin interface functions
bool pwm_plugin_init(plugin_config_t config);
bool pwm_plugin_process(motor_data_t *motor_data);
bool pwm_plugin_terminate(void);

#endif // PWM_PLUGIN_H
