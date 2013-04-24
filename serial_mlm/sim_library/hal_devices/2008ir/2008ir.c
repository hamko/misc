/*!
  \file   2008ir.c
  \author TAKISAWA Jun'ichi.
  \date   Sun Apr 27 02:00:19 2008

  \brief  2008年赤外線送信モジュール用HAL

 */

#include "2008ir_prv.h"

void clear_2008ir(hal_2008ir ir)
{
    ir -> mode = IRTX;
    ir -> data = IRDAT0;
}

void set_2008ir_mode(hal_2008ir ir, hal_ir_mode mode)
{
    ir -> mode = mode;
}

hal_ir_mode get_2008ir_mode(hal_2008ir ir)
{
    return ir -> mode;
}

void set_2008ir_data(hal_2008ir ir, hal_ir_data data)
{
    // 送信モードの場合のみデータの設定を行う。
    if(ir -> mode == IRTX)
    {
        ir -> data = data;
    }
}

hal_ir_data get_2008ir_data(hal_2008ir ir)
{
    return ir -> data;
}
