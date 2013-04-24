/*!
  \file   hal_motor.h
  \author TAKISAWA <junichi@Hyperion.private.net>
  \date   Tue Dec  4 18:25:19 2007

  \brief  hal_motor.c用ヘッダファイル

 */

#ifndef _HAL_MOTOR_H_
#define _HAL_MOTOR_H_

//! モータ動作モードリスト
/*!
  モータの動作モードを定義する。
  以下の4つのモードが使用可能。
  -FORWARD
  -BACK
  -BRAKE
  -FREE
 */
typedef enum
{
    FORWARD = 0,
    BACK = 1,
    BRAKE = 2,
    FREE = 3
} motor_mode;

//! 指定可能なDutyの最大値
#define MOTOR_MAX_DUTY 1000


//! 抽象モータの不完全型宣言
/*!
  実際の構造体定義はhal_motor.c参照。
 */
typedef struct hal_motor_tag* hal_motor;

//! モータクリア関数
/*!
  \param motor 指定対象となるモータ
 */
void clear_motor(hal_motor motor);

//! モータ設定関数
/*!
  \param motor 指定対象となるモータ
  \param mode 指定するモード
  \param duty 指定するDuty
 */
void set_motor(hal_motor motor, motor_mode mode, unsigned int duty);

//! モータ動作モード設定関数
/*!
  \param motor モードの指定対象となるモータ
  \param mode 指定するモード
 */
void set_motor_mode(hal_motor motor, motor_mode mode);

//! モータDuty設定関数
/*!
  \param motor Duty設定の対象となるモータ
  \param duty 指定するDuty値
 */
void set_motor_duty(hal_motor motor, unsigned int duty);

//! モータ動作モード取得関数
/*!
  \param motor モード取得対象となるモータ

  \return 対象モータのモード
 */
motor_mode get_motor_mode(hal_motor motor);

//! モータDuty取得関数
/*!
  \param motor 対象となるモータ

  \return 対象モータのDuty
 */
unsigned int get_motor_duty(hal_motor motor);

#endif //_HAL_MOTOR_H_
