/*!
  \file   hal_servo.h
  \author TAKISAWA <junichi@Hyperion.private.net>
  \author ktns
  \author Tatsuru WATANABE
  \date   Tue Dec  4 18:25:19 2007
  
  \brief  hal_servo.c用ヘッダファイル
    
*/

#ifndef _HAL_SERVO_H_
#define _HAL_SERVO_H_

#include "util.h"

//! 抽象サーボの不完全型宣言
/*! 
  実際の構造体定義はhal_servo.c参照。

*/
typedef struct hal_servo_tag* hal_servo;

//  サーボ固定関数
/*
\param servo 固定するサーボ
*/
void engage_servo(hal_servo servo);
/* /\**  */
/*  * サーボ初期化関数 */
/*  *  */
/*  * \param servo 初期化するサーボ */
/*  *\/ */
/* void clear_servo(hal_servo servo); */

//  サーボ開放関数

/*
\param servo 開放するサーボ
*/
void release_servo(hal_servo servo);

//! サーボ角度設定関数
/*! 
\param servo 設定の対象となるサーボ
\param angle 指定する角度(-32768~32767)
*/
void set_servo_angle(hal_servo servo,short angle);

//! サーボ角度取得関数
/*! 
\param servo 対象となるサーボ

\return 対象サーボの角度
*/
short get_servo_angle(hal_servo servo);

//! サーボ角度取得関数
/*! 
\param servo 対象となるサーボ

\return 対象サーボの角度
*/
BOOLEAN get_servo_status(hal_servo servo);


#endif //_HAL_SERVO_H_
