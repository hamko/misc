/*!
  \file   hal_switch.h
  \author TAKISAWA Jun'ichi.
  \date   Sat Dec 22 19:42:35 2007

  \brief  hal_switch.c用ヘッダファイル
 */

#ifndef _HAL_SWITCH_H_
#define _HAL_SWITCH_H_

#include "util.h"

//! 仮想スイッチの不完全型宣言。詳細はhal_switch_prv.h参照
typedef struct hal_switch_tag* hal_switch;

//! スイッチの初期化関数
void clear_switch(hal_switch _switch);

//! スイッチの状態取得関数
/*!
  \param switch 指定対象のスイッチ

  \return スイッチがON状態ならTRUE、そうでなければFALSEを返す。
 */
BOOLEAN get_switch_status(hal_switch _switch);

#endif //_HAL_SWITCH_H_
