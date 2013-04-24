/*!
  \file   2008ir_prv.h
  \author TAKISAWA Jun'ichi.
  \date   Sun Apr 27 02:05:17 2008

  \brief  2008ir.c用プライベートヘッダファイル。


 */

#ifndef _2008IR_PRV_H_
#define _2008IR_PRV_H_

#include "2008ir.h"

struct hal_2008ir_tag
{
    hal_ir_mode mode;
    hal_ir_data data;
};

//! 2008IRのモード設定関数。
/*!
  2008IRのモードを設定する。
  動作中の動的なモード変更は不可能であり、この関数は設定時に回路屋のみが用いる。
  このため、非公開領域にプロトタイプが設定されている。

  この関数の誤用はハードウェア的損害につながるおそれがあるので十分な注意の元使用すること。

  \param ir 設定対象となる2008IRモジュール。
  \param mode 通信モード[IRTX|IRRX]。
 */
void set_2008ir_mode(hal_2008ir ir, hal_ir_mode mode);

#endif //_2008IR_PRV_H_
