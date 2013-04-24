#ifndef _HAL_ADC_H_
#define _HAL_ADC_H_

typedef struct hal_adc_tag *hal_adc;


void clear_adc(hal_adc adc);


unsigned int get_adc_value(hal_adc adc);

#endif
