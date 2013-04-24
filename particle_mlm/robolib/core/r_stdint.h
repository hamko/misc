/*!
  \file    r_stdint.h
  \ingroup core
  \author  satok
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   整数型の定義。

  マイコンの種類によって区々な型のサイズ定義を一括管理する。\n
  整数には極力以下の型を使用すること。
 */

#ifndef __INCLUDED_R_STDINT_H__
#define __INCLUDED_R_STDINT_H__

#include <stdint.h>
/*
// For 3052
typedef signed char int8_t; //!<  8ビット符号つき整数
typedef short int16_t;      //!< 16ビット符号つき整数
typedef long int32_t;       //!< 32ビット符号つき整数
typedef long long int64_t;  //!< 64ビット符号つき整数

typedef unsigned char uint8_t;          //!<  8ビット符号なし整数
typedef unsigned short uint16_t;        //!< 16ビット符号なし整数
typedef unsigned long uint32_t;         //!< 32ビット符号なし整数
typedef unsigned long long uint64_t;    //!< 64ビット符号なし整数

typedef short pr_int;   //!< printf用?
*/

#endif
