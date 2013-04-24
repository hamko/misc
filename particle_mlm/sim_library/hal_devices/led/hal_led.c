/*!
  \file   hal_led.c
  \author TAKISAWA Jun'ichi
  \date   Sun Dec 23 14:47:28 2007

  \brief  仮想LED
  実戦上特に意味はない。デバッグ向け。
 */

#include "hal_led_prv.h"

void clear_led(hal_led led)
{
    led -> status = FALSE;
}

void set_led(hal_led led)
{
    led -> status = TRUE;
}
