/*!
  \file   hal_port_prv.h
  \author TAKISAWA, Jun'ichi.
  \date   Thu Mar 20 20:42:54 2008

  \brief  hal_port.c用プライベートヘッダファイル。


 */

#ifndef _HAL_PORT_PRV_H_
#define _HAL_PORT_PRV_H_

#include"hal_port.h"

struct hal_port_tag
{
    //! Data Direction Register. 0が入力、1が出力に対応。
    unsigned char ddr;
    //! Data Register.
    unsigned char dr;
};

#endif
