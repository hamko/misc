/*!
  \file   hal_encoder.c
  \author TAKISAWA Jun'ichi
  \date   Fri Dec 21 12:27:32 2007

  \brief  仮想エンコーダ


 */

#include "hal_encoder_prv.h"

void clear_encoder(hal_encoder encoder)
{
    //! エンコーダのクリアを行う関数
    //! 初期値は速度、累積値ともに0。
    encoder -> velocity = 0;
    encoder -> total = 0;
}

long get_encoder_velocity(hal_encoder encoder)
{
    //! 現在の速度を返す。
    return encoder -> velocity;
}

long get_encoder_total(hal_encoder encoder)
{
    //! 現在の積算値を返す。
    return encoder -> total;
}

void set_encoder_velocity(hal_encoder encoder, long velocity)
{
    //! 仮想エンコーダの速度を設定する。
    //! これを明示的に呼び出すことは通常無いはずで、非公開関数とした。
    encoder -> velocity = velocity;
}

void set_encoder_total(hal_encoder encoder, long total)
{
    //! 仮想エンコーダの積算値を設定する。
    //! これを明示的に呼び出すことは通常無いはずで、非公開関数とした。
    encoder -> total = total;
}
