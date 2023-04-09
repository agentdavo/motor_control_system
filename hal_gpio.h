#ifndef HAL_GPIO_H
#define HAL_GPIO_H

typedef enum {
    GPIO_INPUT,
    GPIO_OUTPUT
} gpio_mode_t;

void hal_gpio_init(int pin, gpio_mode_t mode);
void hal_gpio_write(int pin, int value);
int hal_gpio_read(int pin);

#endif // HAL_GPIO_H
