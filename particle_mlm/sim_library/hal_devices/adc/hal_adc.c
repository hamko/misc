/*!
  \file hal_adc.c
  \author ONO
  \date Sat Sep 27

  \brief 仮想A/D変換器

 */

#include "hal_adc_prv.h"

void clear_adc(hal_adc adc)
{
    adc->value = 0;
}

unsigned int get_adc_value(hal_adc adc)
{
    return adc->value;
}

void set_adc_value(hal_adc adc, unsigned int value)
{
    adc->value = value;
}
