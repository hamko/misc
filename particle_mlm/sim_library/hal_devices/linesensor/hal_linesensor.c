/*!
  \file   hal_linesensor.c
  \author TAKISAWA Jun'ichi
  \date   Sat Dec 22 15:03:04 2007

  \brief  仮想ラインセンサ
 */

#include "hal_linesensor_prv.h"

void clear_linesensor(hal_linesensor linesensor)
{
    linesensor -> sensor = 0;
}

unsigned char get_linesensor_value(hal_linesensor linesensor)
{
    return linesensor -> sensor;
}

unsigned char is_linesensor(hal_linesensor linesensor, unsigned char pos)
{
    switch(pos)
    {
        case 0:
            return linesensor -> sensor & 0x01 ? 1 : 0;
        case 1:
            return linesensor -> sensor & 0x02 ? 1 : 0;
        case 2:
            return linesensor -> sensor & 0x04 ? 1 : 0;
        case 3:
            return linesensor -> sensor & 0x08 ? 1 : 0;
        case 4:
            return linesensor -> sensor & 0x10 ? 1 : 0;
        case 5:
            return linesensor -> sensor & 0x20 ? 1 : 0;
        case 6:
            return linesensor -> sensor & 0x40 ? 1 : 0;
        case 7:
            return linesensor -> sensor & 0x80 ? 1 : 0;
        default :
            return 0;
    }
    return 0;
}

unsigned char is_linesensor0(hal_linesensor linesensor)
{
    return linesensor -> sensor & 0x01 ? 1 : 0;
}

unsigned char is_linesensor1(hal_linesensor linesensor)
{
    return linesensor -> sensor & 0x02 ? 1 : 0;
}

unsigned char is_linesensor2(hal_linesensor linesensor)
{
    return linesensor -> sensor & 0x04 ? 1 : 0;
}

unsigned char is_linesensor3(hal_linesensor linesensor)
{
    return linesensor -> sensor & 0x08 ? 1 : 0;
}

unsigned char is_linesensor4(hal_linesensor linesensor)
{
    return linesensor -> sensor & 0x10 ? 1 : 0;
}

unsigned char is_linesensor5(hal_linesensor linesensor)
{
    return linesensor -> sensor & 0x20 ? 1 : 0;
}

unsigned char is_linesensor6(hal_linesensor linesensor)
{
    return linesensor -> sensor & 0x40 ? 1 : 0;
}

unsigned char is_linesensor7(hal_linesensor linesensor)
{
    return linesensor -> sensor & 0x80 ? 1 : 0;
}
