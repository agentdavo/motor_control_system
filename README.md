# motor_control_system

Our motor control system consists of a central nexus that manages the plugins responsible for various functions such as ADC, PWM, and encoder. Each plugin implements the plugin interface, which provides functions for initialization, processing, and termination.

The central nexus is responsible for registering and processing the plugins in a prioritized manner using FreeRTOS task priorities and task notifications.

We updated the plugin interface to use task notifications instead of semaphores and added a new plugin registration structure to track plugin registration and processing. We also updated the existing plugins to use the new interface and added a new callback function for plugins to register with the central nexus.

Our system consists of the following files:

    central_nexus.c/.h: Implements the central nexus that manages plugins and motor data.
    plugin_interface.c/.h: Implements the plugin interface for plugins to interact with the central nexus.
    adc_plugin.c/.h: Implements the ADC plugin.
    pwm_plugin.c/.h: Implements the PWM plugin.
    encoder_plugin.c/.h: Implements the encoder plugin.
    sin_table.c/.h: Implements a lookup table for sine and cosine values.
    main.c: The main application that initializes and runs the motor control system.
    CMakeLists.txt: The CMake configuration file for building the system.
