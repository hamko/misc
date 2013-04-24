/*!
  \file    drive_stop.c
  \ingroup drive
  \author  Sota <jine0125@gmail.com>
  \date    2010/10/22

  \brief   駆動輪の停止

  駆動輪の種類に拠らない
 */

#include "install_option.h"
#ifdef __INSTALL_DRIVE__

#undef NULL
#define NULL ((void *)0)

static void (*_func)(void) = NULL;    //!< 停止関数

//! 駆動輪停止関数を設定
/*
  \param func 停止関数へのポインタ
 */
void set_drive_stop(void (*func)(void))
{
    _func = func;
}


//! 現在設定されている駆動輪停止関数を返す
void (*get_drive_stop(void))(void)
{
    return _func;
}


//! 駆動輪を停止する
/*
  set_drive_stopで登録された関数を呼び出し、駆動輪を停止する
 */
void drive_stop(void)
{
    (*_func)();
}


#endif
