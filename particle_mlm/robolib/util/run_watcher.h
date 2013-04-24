/*!
  \file    run_watcher.h
  \ingroup run_watcher
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/17

  \brief   走行指令用安全装置
 */

#include "install_option.h"
#ifndef __INSTALL_RUN_WATCHER__
#error run_watcher is not installed
#endif

#ifndef __INCLUDED_RUN_WATCHER_H__
#define __INCLUDED_RUN_WATCHER_H__

//! 指定時間経過後、走行を停止する
/*!
  \param t 走行時間
 */
void set_run_watcher(int t);

//! カウントダウンをやめる
/*!
  走行停止を元に戻すことはできない
 */
void quit_run_watcher(void);

#endif
