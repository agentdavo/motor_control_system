#ifndef ENCODER_PLUGIN_H
#define ENCODER_PLUGIN_H

#include "plugin_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

// Encoder plugin initialization function
bool encoder_plugin_init(void);

// Encoder plugin processing function
bool encoder_plugin_process(motor_data_t *motor_data);

// Encoder plugin termination function
bool encoder_plugin_terminate(void);

// Encoder plugin struct
extern plugin_t encoder_plugin;

#ifdef __cplusplus
}
#endif

#endif /* ENCODER_PLUGIN_H */
