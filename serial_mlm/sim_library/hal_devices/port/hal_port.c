/*!
  \file   hal_port.c
  \author TAKISAWA, Jun'ichi.
  \date   Thu Mar 20 20:35:52 2008

  \brief  汎用ポートHALデバイス。
 */

#include"hal_port_prv.h"

void clear_port(hal_port port)
{
    port -> ddr = 0x00;
    port -> dr = 0x00;
}

void set_ddr_byte(hal_port port, unsigned char ddr)
{
    port -> ddr = ddr;
}

void set_ddr_bit(hal_port port, unsigned char bit)
{
    unsigned char mask = 0x01;

    mask <<= bit;

    port -> ddr |= mask;
}

void clear_ddr_bit(hal_port port, unsigned char bit)
{
    unsigned char mask = 0x01;

    mask <<= bit;
    mask = ~mask;

    port -> ddr &= mask;
}

unsigned char get_ddr_byte(hal_port port)
{
    return port -> ddr;
}

unsigned char get_ddr_bit(hal_port port, unsigned char bit)
{
    unsigned char mask = 0x01;

    mask <<= bit;

    return port -> ddr & mask ? 1 : 0;
}

void set_dr_byte(hal_port port, unsigned char dr)
{
    port -> dr = dr;
}

void set_dr_bit(hal_port port, unsigned char bit)
{
    unsigned char mask = 0x01;

    mask <<= bit;

    port -> dr |= mask;
}

void clear_dr_bit(hal_port port, unsigned char bit)
{
    unsigned char mask = 0x01;

    mask <<= bit;
    mask = ~mask;

    port -> dr &= mask;
}

unsigned char get_dr_byte(hal_port port)
{
    return port -> dr;
}

unsigned char get_dr_bit(hal_port port, unsigned char bit)
{
    unsigned char mask = 0x01;

    mask <<= bit;

    return port -> dr & mask ? 1 : 0;
}
