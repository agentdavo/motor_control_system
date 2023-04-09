cmake_minimum_required(VERSION 3.10)
project(motor_control_system C)

set(CMAKE_C_STANDARD 99)

# Add FreeRTOS as a library
add_subdirectory(FreeRTOS)

# Include directories
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/central_nexus)
include_directories(${PROJECT_SOURCE_DIR}/plugin_interface)
include_directories(${PROJECT_SOURCE_DIR}/plugins)
include_directories(${PROJECT_SOURCE_DIR}/hal)

# Source files
file(GLOB_RECURSE SOURCES "central_nexus/*.c" "plugin_interface/*.c" "plugins/**/*.c" "hal/*.c")

# Executable
add_executable(motor_control_system main.c ${SOURCES})
target_link_libraries(motor_control_system PRIVATE FreeRTOS)
