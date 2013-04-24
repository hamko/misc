/*!
  \file    run_watcher.c
  \ingroup run_watcher
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/17

  \brief   走行指令用安全装置
 */

#include "install_option.h"
#ifdef __INSTALL_RUN_WATCHER__

#include "drive_stop.h"
#include "function_list.h"

#ifndef NULL
#define NULL ((void *)0)    //!< ヌルポインタ
#endif

static void run_watcher(void);
static int t_rem = 0;   //!< 残り時間

//! 指定時間経過後、走行を停止する
void set_run_watcher(int t)
{
    t_rem = t;
    set_function(run_watcher, LABEL_USER_PRE);
}


//! カウントダウンをやめる
/*!
  走行停止を元に戻すことはできない
 */
void quit_run_watcher(void)
{
    remove_function(run_watcher);
}


//! 割り込み関数
/*!
  カウントダウン、停止を行う
 */
static void run_watcher(void)
{
    // 駆動関数が動いているときのみカウントする
    if (get_function(LABEL_DRIVE)==NULL){
        return;
    }
    if ((--t_rem)<=0){
        drive_stop();
        remove_function(run_watcher);
        remove_labeled_functions(LABEL_DRIVE);
        remove_labeled_functions(LABEL_BLOCK);
    }
}

#endif
