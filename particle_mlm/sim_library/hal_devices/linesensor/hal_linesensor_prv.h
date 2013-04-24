/*!
  \file   hal_linesensor_prv.h
  \author TAKISAWA <junichi@Hyperion.private.net>
  \date   Sat Dec 22 15:05:58 2007

  \brief  hal_linesensor.c用プライベートヘッダファイル
  プログラマへの公開不可。
 */

#ifndef _HAL_LINESENSOR_PRV_H_
#define _HAL_LINESENSOR_PRV_H_

#include "hal_linesensor.h"

//! 仮想ラインセンサ
struct hal_linesensor_tag
{
    //! センサ値を格納する変数
    unsigned char sensor;
};

#endif //_HAL_LINESENSOR_PRV_H_
