#ifndef HAL_ADC_H
#define HAL_ADC_H

void hal_adc_init(void);
float hal_adc_read_voltage(int channel);

#endif // HAL_ADC_H
