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
#include "params.h"

void control(void)
{
    if (get_function(LABEL_DRIVE)==NULL){
        tw_set_v_omega(0,0);
    }
    call_function_list();
}
