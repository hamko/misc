
#ifndef _HAL_ADC_PRV_H_
#define _HAL_ADC_PRV_H_

#include "hal_adc.h"


struct hal_adc_tag
{
    unsigned int value;
};

void set_adc_value(hal_adc adc, unsigned int value);

#endif
