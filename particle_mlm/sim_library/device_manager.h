/*!
  \file   device_manager.h
  \author TAKISAWA, Jun'ichi.
  \date   Fri Jan 11 10:05:31 2008

  \brief  divice_manager.c用ヘッダファイル
 */

#ifndef _DEVICE_MANAGER_H_
#define _DEVICE_MANAGER_H_

#include "libhal_prv.h"

typedef union
{
    struct hal_2008ir_tag _2008ir;
    struct hal_adc_tag adc;
    struct hal_encoder_tag encoder;
    struct hal_led_tag led;
    struct hal_linesensor_tag linesensor;
    struct hal_motor_tag motor;
    struct hal_port_tag port;
    struct hal_switch_tag _switch;
    struct hal_pscontroller_tag pscontroller;
    struct hal_servo_tag servo;
}hal_device;

typedef struct
{
    hal_device device;
    void (*presync)(hal_device *device);
    void (*postsync)(hal_device *device);
}DM_device_info;

//! デバイスリスト初期化関数
void DM_initialize(void);

/* //! 未使用デバイス取得関数 */
/* /\*!  */
/* \return 未使用デバイスへのポインタ。未使用デバイスが無い場合はNULLを返す。 */
/* *\/ */
/* device_info* get_device(void); */

//! 新規デバイス登録関数
/*!
  \param presync presync(Hardware -> HAL方向)の更新を行う関数。
  \param postsync postsync(HAL -> Hardware方向)の更新を行う関数。

  \return 登録したデバイスへのポインタ。登録エラーの場合は強制終了する。
 */
hal_device* DM_add_device(void (*presync)(hal_device *device),
        void (*postsync)(hal_device *device));

//! 登録デバイスとHALのpresyncを行う。
void DM_presync(void);

//! 登録デバイスとHALのpostsyncを行う。
void DM_postsync(void);

#endif //_DEVICE_MANAGER_H_
