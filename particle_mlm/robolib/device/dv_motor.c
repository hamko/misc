/*!
  \file    dv_motor.c
  \ingroup device
  \author  Saki <aki0122@msn.com>
  \date    2010/07/14

  \brief   方向つきモータの定義と関連関数
*/

#include "install_option.h"
#ifdef __INSTALL_DEVICE__

#include "dv_motor.h"

void dv_set_motor(p_motor_t motor, motor_mode mode, int16_t duty){
    switch(motor->direction){
    case POSITIVE:
        switch(mode){
        case FORWARD:
            if(duty < 0){
                mode = BACK;
                duty *= -1;
            }
            break;
        case BACK:
            if(duty < 0){
                mode = FORWARD;
                duty *= -1;
            }
            break;
        default:
            break;
        }
        break;
    case NEGATIVE:
        switch(mode){
        case FORWARD:
            if(duty < 0){
                duty *= -1;
            }
            else{
                mode = BACK;
            }
            break;
        case BACK:
            if(duty < 0){
                duty *= -1;
            }
            else{
                mode = FORWARD;
            }
            break;
        default:
            break;
        }
        break;
    default:
        //! 不正な値
        return;
    }
    set_motor(motor->hal, mode, duty);
}

int16_t dv_get_signed_duty(p_motor_t motor){
    switch(motor->direction){
    case POSITIVE:
        switch(get_motor_mode(motor->hal)){
        case FORWARD:
            return get_motor_duty(motor->hal);
        case BACK:
            return -get_motor_duty(motor->hal);
        default:
            return 0;
        }
    case NEGATIVE:
        switch(get_motor_mode(motor->hal)){
        case FORWARD:
            return -get_motor_duty(motor->hal);
        case BACK:
            return get_motor_duty(motor->hal);
        default:
            return 0;
        }
    default:
        //! 不正な値
        return 0;
    }
}

motor_mode dv_get_motor_mode(p_motor_t motor){
    motor_mode mode = get_motor_mode(motor->hal);
    switch(motor->direction){
    case POSITIVE:
        return mode;
    case NEGATIVE:
        switch(mode){
        case FORWARD:
            return BACK;
        case BACK:
            return FORWARD;
        default:
            return mode;
        }
    default:
        //! 不正な値
        return FREE;
    }
}

#endif
