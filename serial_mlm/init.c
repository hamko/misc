/*!
  \file    init.c
  \author  Sota <jine0125@gmail.com>
  \date    2010/08/07

  \brief   初期化のサンプル
 */

#include "install_option.h"
#include "function_list.h"
#include "hal.h"
#include "hdwdef.h"
#include "params.h"
#ifdef __INSTALL_DRIVE__
#include "drive_stop.h"
#endif
#ifdef __INSTALL_DRIVE_TW__
#include "drive_tw.h"
#endif
#ifdef __INSTALL_DUTY_ODOMETRY__
#include "duty_odometry.h"
#endif
#ifdef __INSTALL_KAKITANI__
#include "kakitani.h"
#endif
#ifdef __INSTALL_LOCUS__
#include "locus.h"
#endif
#ifdef __INSTALL_MAP__
#include "map.h"
#include "odometry.h"
#endif
#ifdef __INSTALL_MLM__
#include "mlm.h"
#endif
#ifdef __INSTALL_SHELL__
#include "shell.h"
#endif
#ifdef __INSTALL_TIMER__
#include "timer_utility.h"
#endif

void init_robolib(void)
{
    init_hdwdef();
    init_function_list();
#ifdef __INSTALL_TIMER__
    init_timer_utility(CONTROL_CYCLE);
#endif
#ifdef __INSTALL_MAP__
    init_map();
    init_odometry(
        (odometry_t){
            .left_encoder  = LEFT_ENCODER,
            .right_encoder = RIGHT_ENCODER,
            .step_left     = STEP_LEFT,
            .step_right    = STEP_RIGHT,
            .encoder_width = ENCODER_WIDTH
         }
    );
#endif
#ifdef __INSTALL_DUTY_ODOMETRY__
    init_duty_odometry(
        (duty_odometry_t){
            .left_motor  = LEFT_MOTOR,
            .right_motor = RIGHT_MOTOR,
            .k_a = K_A,
            .k_v = K_V,
            .r = 0,
            .width = WHEEL_WIDTH,
         }
    );
#endif
#ifdef __INSTALL_DRIVE_TW__
    /*
    init_tw_tdf_pi(
        (tw_tdf_pi_t){
            .k_a = K_A,
            .k_v = K_V,
            .k_i = 5000000,
            .offset_l = LEFT_OFFSET,
            .offset_r = RIGHT_OFFSET,
            .width = WHEEL_WIDTH,
            .map = MAP_ROOT,
            .left_motor = LEFT_MOTOR,
            .right_motor = RIGHT_MOTOR,
            .default_shift_num = 1
        }
    );
    */
    init_tw_tdf(
        (tw_tdf_t){
            .k_a = K_A,
            .k_v = K_V,
            .offset_l = LEFT_OFFSET,
            .offset_r = RIGHT_OFFSET,
            .width = WHEEL_WIDTH,
            .map = MAP_ROOT,
            .left_motor = LEFT_MOTOR,
            .right_motor = RIGHT_MOTOR
        }
    );
    set_drive_stop(tw_free);
#endif
#ifdef __INSTALL_KAKITANI__
    init_kakitani(
        (kakitani_t){
            .k=KAKITANI_K,
            .stop_go_x = STOP_GO_X,
            .stop_go_y = STOP_GO_Y,
            .stop_go_theta = STOP_GO_THETA,
            .stop_turn_theta = STOP_TURN_THETA,
            .map = MAP_MAP,
            .submap = MAP_SUBMAP,
            .a_max = A_MAX,
            .turn_a = TURN_A,
            .max_omega = MAX_OMEGA,
            .alpha_max =ALPHA_MAX 
        }
    );
#endif
#ifdef __INSTALL_LOCUS__
    init_go_locus(
        (go_locus_t){
            .map = MAP_MAP,
            .submap = MAP_SUBMAP,
            .l = GO_LOCUS_R_MIN
        }
    );
#endif
#ifdef __INSTALL_MLM__
//    init_mlm(MAP_MAP, pmlms, 300, 80, 50, 100, 500, 11, 7, 0, 0, 0);
#endif
#ifdef __INSTALL_SHELL__
    init_shell();
#endif

#ifdef __INSTALL_TIMER__
    wait_cycles(CONTROL_CYCLE);
#endif

    return;
}
