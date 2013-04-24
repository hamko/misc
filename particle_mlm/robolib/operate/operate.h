/*!
  \file    operate.h
  \ingroup operate
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   制御指令関数用ツール

  指令関数<name>は、<name>(...)の他に<name>_with(fp, ...)が用意されており、fpはラベルがBLOCKの関数である。\n
  ノーブロックで呼び出したいときには、<name>_with(noblock, ...)とすればよいし、
  終了条件判定関数を自作した場合は、それを<name>_(<original>, x, y, ...)として組み込むこともできる。
 */

#include "install_option.h"
#ifndef __INSTALL_OPERATE__
#error operate is not installed
#endif

#ifndef __INCLUDED_OPERATE_H__
#define __INCLUDED_OPERATE_H__

//! 制御指令の終了判定を行わない
/*!
  noblockを終了条件として登録することで(何も登録しないことで)、
  wait_for_controlは何もせずに終了する
 */
#define noblock (void *)(0)

//! 足回り制御用の割り込み関数を登録
/*!
  旧関数を削除してから登録する。
 */
void set_drive_function(void (*fp)(void));

//! 足回り制御用の割り込み関数を削除する
void remove_drive_function(void);


//! 足回り制御の終了まで待機する
/*!
  PRIORITY_BLOCKにラベルづけされた関数がリストに登録されている間、whileで待機しつづける
 */
void wait_for_control(void);


#endif
