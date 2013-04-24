/*!
  \file   2008ir.h
  \author TAKISAWA <junichi@Hyperion.private.net>
  \date   Sun Apr 27 02:07:50 2008
  
  \brief  2008ir.c用ヘッダファイル
  
  
*/

#ifndef _2008IR_H_
#define _2008IR_H_

typedef enum
{
  IRTX = 0,
  IRRX = 1
}hal_ir_mode;

typedef enum
{
  IRDAT0 = 0,
  IRDAT1 = 1,
  IRDAT2 = 2,
  IRDAT3 = 3
}hal_ir_data;

typedef struct hal_2008ir_tag* hal_2008ir;

//! 2008IRのHAL初期化関数
/*! 
  2008IRのHALをモードTX(送信)、データIRDAT0で初期化する。
\param ir 設定対象の2008IR。
*/
void clear_2008ir(hal_2008ir ir);

//! 2008IRのモード取得関数
/*! 
  2008IRの現行モードを取得する。
\param ir 取得対象の2008IR。

\return 現行モード[IRTX|IRRX]。
*/
hal_ir_mode get_2008ir_mode(hal_2008ir ir);

//! 2008IRの送信データ設定関数
/*! 
  2008IRの送信データを設定する。
  2008IRのモードがIRRXの場合、何も処理を行わない。

\param ir 設定対象の2008IR。
\param data 設定データ[IRDAT0|IRDAT1|IRDAT2|IRDAT3]
*/
void set_2008ir_data(hal_2008ir ir, hal_ir_data data);

//! 2008IRのデータ取得関数。
/*! 
  2008IRの現在のデータを取得する。
  モードがIRTXの場合は設定されているモードを、IRRXの場合は受信したモードを返す。
\param ir 取得対象の2008IR

\return 現在のデータ[IRDAT0|IRDAT1|IRDAT2|IRDAT3]。
*/
hal_ir_data get_2008ir_data(hal_2008ir ir);

#endif //_2008IR_H
