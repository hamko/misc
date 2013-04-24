/*!
  \file    dv_encoder.c
  \ingroup device
  \author  Iori <ik0313@gmail.com>
  \date    2010/07/12

  \brief   方向つきエンコーダの定義と関連関数
*/

#include "install_option.h"
#ifdef __INSTALL_DEVICE__

#include "dv_encoder.h"

int32_t dv_get_encoder_total(p_encoder_t enc){
    switch(enc->direction){
    case POSITIVE:
        return get_encoder_total(enc->hal);
    case NEGATIVE:
        return -get_encoder_total(enc->hal);
    default:
        // 不正な値
        return 0;
    }
}

int32_t dv_get_encoder_vel(p_encoder_t enc){
    switch(enc->direction){
    case POSITIVE:
        return get_encoder_velocity(enc->hal);
    case NEGATIVE:
        return -get_encoder_velocity(enc->hal);
    default:
        // 不正な値
        return 0;
    }
}

#endif
