/*!
  \file    device.h
  \ingroup device
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   HALに物理的意味づけを付加する
 */

#include "install_option.h"
#ifndef __INSTALL_DEVICE__
#error device is not installed
#endif

#ifndef __INCLUDED_DEVICE_H__
#define __INCLUDED_DEVICE_H__

//! デバイスの方向
typedef enum {
    POSITIVE,   //!< 正方向
    NEGATIVE    //!< 逆方向
} direction_t;

#endif
