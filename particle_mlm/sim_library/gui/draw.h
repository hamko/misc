#ifndef _DRAW_H
#define _DRAW_H

#include <gtk/gtk.h>

// undefしないとエラーになる
#undef FALSE
#undef TRUE
#include "hal.h"
#include "dv_motor.h"

// added for GUI
hal_motor get_left_motor(void);
hal_motor get_right_motor(void);

extern float _x;		/* mm */
extern float _y;		/* mm */
extern float _theta;		/* rad */
extern float _v;		/* mm/s */
extern float _omega;		/* rad/s */

// 起動時からの割り込み回数
extern int interrupt_count;

#endif //_DRAW_H
