/*!
  \file   hal_encoder_prv.h
  \author TAKISAWA Jun'ichi
  \date   Fri Dec 21 17:49:59 2007

  \brief  hal_encoder.c用プライベートヘッダファイル。

  プログラマへの公開は不可。
 */

#ifndef _HAL_ENCODER_PRV_H_
#define _HAL_ENCODER_PRV_H_

#include "hal_encoder.h"

//! 仮想エンコーダ
struct hal_encoder_tag
{
    //! 速度用変数
    long velocity;
    //! 累積値用変数
    long total;
};

//! 仮想エンコーダの速度設定関数
/*!
  \param encoder 指定対象となるエンコーダ
  \param velocity 指定する速度
 */
void set_encoder_velocity(hal_encoder encoder, long velocity);

//! 仮想エンコーダの積算値設定関数
/*!
  \param encoder 指定対象となるエンコーダ
  \param total 指定する積算値
 */
void set_encoder_total(hal_encoder encoder, long total);


#endif //_HAL_ENCODER_PRV_H_
