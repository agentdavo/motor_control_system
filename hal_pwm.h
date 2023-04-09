#ifndef HAL_PWM_H
#define HAL_PWM_H

void hal_pwm_init(void);
void hal_pwm_set_duty_cycle(int channel, float duty_cycle);

#endif // HAL_PWM_H
