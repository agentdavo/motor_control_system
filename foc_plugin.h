#ifndef FOC_PLUGIN_H
#define FOC_PLUGIN_H

#include "plugin_interface/plugin_interface.h"

typedef struct {
    plugin_t plugin;
    float target_current[3][2];
    float i_alpha[3], i_beta[3];
    float v_alpha[3], v_beta[3];
    float sin_theta[3], cos_theta[3];
    float last_error[3][2], error_integral[3][2];
    float kp[3][2], ki[3][2], kd[3][2];
} foc_plugin_t;

bool foc_plugin_init(void);
bool foc_plugin_process(void);
bool foc_plugin_terminate(void);

#endif // FOC_PLUGIN_H
