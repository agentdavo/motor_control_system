#ifndef CONFIG_H
#define CONFIG_H

// Define the number of motors
#define NUM_MOTORS 1

// Define the FOC plugin parameters
#define FOC_PLUGIN_DT 0.001f // FOC loop time step
#define FOC_PLUGIN_SPEED_FILTER_COEFF 0.01f // Low-pass filter coefficient for speed estimate

#endif // CONFIG_H
