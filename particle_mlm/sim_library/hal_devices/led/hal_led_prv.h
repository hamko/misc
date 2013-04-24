/*!
  \file   hal_led_prv.h
  \author TAKISAWA <junichi@Hyperion.private.net>
  \date   Sun Dec 23 14:53:03 2007

  \brief  hal_led.c用プライベートヘッダファイル

 */

#ifndef _HAL_LED_PRV_H_
#define _HAL_LED_PRV_H_

#include "hal_led.h"
#include "util.h"

struct hal_led_tag
{
    BOOLEAN status;
};

#endif //_HAL_LED_PRV_H_
