/*!
  \file    operate.c
  \ingroup operate
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/12

  \brief   制御指令関数用ツール
 */

#include "install_option.h"
#ifdef __INSTALL_OPERATE__

#include "function_list.h"
#include "labels.h"
#include "timer_utility.h"

#ifndef NULL
#define NULL ((void *)0)    //!< ヌルポインタ
#endif

static void (*_fp)(void) = NULL;    // 割り込み関数用ポインタ

//! 足回り制御用の割り込み関数を登録
void set_drive_function(void (*fp)(void))
{
    remove_function(_fp);
    _fp = fp;
    set_function(_fp, LABEL_DRIVE);
}

//! 足回り制御用の割り込み関数を削除する
void remove_drive_function(void)
{
    remove_function(_fp);
    _fp = NULL;
}

//! 足回り制御の終了まで待機する
void wait_for_control(void)
{
    while(get_function(LABEL_BLOCK)!=NULL){
        wait_cycles(1);
    }
}

#endif
