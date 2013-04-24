/*!
  \file   hal_led.h
  \author TAKISAWA Jun'ichi
  \date   Sun Dec 23 14:48:55 2007

  \brief  hal_led.c用ヘッダファイル
 */

#ifndef _HAL_LED_H_
#define _HAL_LED_H_

//! 仮想LEDの不完全型宣言。詳細はhal_led.c参照。
typedef struct hal_led_tag* hal_led;

//! LED消灯関数。
void clear_led(hal_led led);

//! LED点灯関数
void set_led(hal_led led);

#endif //_HAL_LED_H_
