/*!
  \file   control.c
  \author TAKISAWA Jun'ichi
  \date   Fri Dec 14 18:14:13 2007

  \brief  制御プログラム群
 */
//#define _DEBUG_

#include "hal.h"
#include "util.h"
#include "function_list.h"
#include "hdwdef.h"
#include "map.h"
#include "odometry.h"
#include "drive_tw.h"
#include "dv_linesensor.h"

void control(void)
{
    if (get_function(LABEL_DRIVE)==NULL){
        tw_set_v_omega(0,0);
    }
    call_function_list();

/*
    int i;
    for (i = 0; i < 8; i++)
        printf("%d", dv_is_linesensor(plsm[0], i));
    printf("|");
    for (i = 0; i < 8; i++)
        printf("%d", dv_is_linesensor(plsm[1], i));
    printf("\n");
   */ 
    int i;
    for (i = 0; i < 16; i++) {
        position_t pos = get_lslbit_coord_avg_in_map(plsl, get_odometry_map(), i);
//        printf("%d ", pos.x);
        printf("%d", dv_is_linesensor_list(plsl, i));
    }
    printf("\n");
}
