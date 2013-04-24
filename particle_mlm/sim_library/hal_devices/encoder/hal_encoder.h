/*!
  \file   hal_encoder.h
  \author TAKISAWA <junichi@Hyperion.private.net>
  \date   Fri Dec 21 12:31:25 2007

  \brief  hal_encoder.c用ヘッダファイル
 */

#ifndef _HAL_ENCODER_H_
#define _HAL_ENCODER_H_

//! encoderの不完全型宣言。内部構成はhal_encoder.c参照。
typedef struct hal_encoder_tag* hal_encoder;

//! エンコーダクリア関数
/*!
  \param encoder 指定対象となるエンコーダ
 */
void clear_encoder(hal_encoder encoder);

//!エンコーダ速度取得関数
/*!
  \param encoder 指定対象となるエンコーダ

  \return 速度
 */
long get_encoder_velocity(hal_encoder encoder);

//! エンコーダ積算値取得
/*!
  \param encoder 指定対象となるエンコーダ

  \return 積算値
 */
long get_encoder_total(hal_encoder encoder);


#endif //_HAL_ENCODER_H_

