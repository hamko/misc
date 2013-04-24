/*!
  \file    timer_utility.h
  \ingroup timer
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   タイマーとそれを利用したツール
 */

#include "install_option.h"
#ifndef __INSTALL_TIMER__
#error timer is not installed
#endif

#ifndef __INCLUDED_TIMER_H__
#define __INCLUDED_TIMER_H__

#include "r_stdint.h"

//! タイマー開始
/*!
  \param cycle 制御周期(単位:ms)
 */
void init_timer_utility(uint32_t cycle);

//! タイマー開始からの経過時間を取得
/*!
 \return タイマー初期化からの経過時間(単位 ms)
*/
int32_t get_time(void);

//! 指定割り込み回数だけ待つ
/*!
 割り込み内部では実行されない。
 */
void wait_cycles(uint32_t n);

//! 指定時間待つ
/*!
  \param ms 待ち時間(単位:msec)
  割り込み内部では実行されない。
 */
void wait_ms(uint32_t ms);


/* for implementer
wait_msなどには割り込み内で呼べないように、function_listのis_interruptingを参照したりさせるとよいかもしれない。
 */

#endif
