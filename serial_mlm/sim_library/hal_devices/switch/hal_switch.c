/*!
  \file   hal_switch.c
  \author TAKISAWA Jun'ichi
  \date   Sat Dec 22 19:37:40 2007

  \brief  仮想スイッチ用関数群
 */

#include "hal_switch_prv.h"

void clear_switch(hal_switch _switch)
{
    _switch -> status = 1;
}

BOOLEAN get_switch_status(hal_switch _switch)
{
    return _switch -> status ? FALSE : TRUE;
}
