/*!
  \file   2008_board_init.h
  \author TAKISAWA, Jun'ichi
  \date   Fri Feb  8 01:16:11 2008

  \brief  2008_board_init.c用ヘッダファイル。


 */

#ifndef _2008_BOARD_INIT_H_
#define _2008_BOARD_INIT_H_

#define GUI_MODE

//#define WAIT_TIME_MODE
#define WAIT_TIME_PER_CYCLE 800

//! 2008ボード初期化関数。
/*!
  DDRの初期設定とCPLD初期化を行う。
 */
void initialize_hal_library(void);

// GUI初期化関数
void gui_init(void);

#endif //_2008_BOARD_INIT_H_
