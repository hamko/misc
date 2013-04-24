/*!
  \file   hal_linesensor.h
  \author TAKISAWA <junichi@Hyperion.private.net>
  \date   Sat Dec 22 15:03:59 2007

  \brief  hal_linesensor.c用ヘッダファイル
 */

#ifndef _HAL_LINESENSOR_H_
#define _HAL_LINESENSOR_H_
//! 仮想ラインセンサの不完全型宣言。詳細は"hal_linesensor.c"参照。
typedef struct hal_linesensor_tag* hal_linesensor;

//! ラインセンサ初期化関数
/*!
  \param linesensor 初期化対象のラインセンサ
 */
void clear_linesensor(hal_linesensor linesensor);

//! ラインセンサ値取得関数
/*!
  \param linesensor 指定対象となるラインセンサ

  \return ラインセンサの値
 */
unsigned char get_linesensor_value(hal_linesensor linesensor);

//! 指定ビット判定関数
/*!
  \param linesensor 判定対象となるラインセンサ
  \param pos 判定位置。[0:7]の整数値。

  \return 指定ビットがラインに反応していれば1、していなければ0を返す。不正な位置が指定された場合は0を返す。
 */
unsigned char is_linesensor(hal_linesensor linesensor, unsigned char pos);

//! 0番ビット判定関数
/*!
  \return ラインに反応していれば1、していなければ0を返す。
 */
unsigned char is_linesensor0(hal_linesensor linesensor);

//! 1番ビット判定関数
/*!
  \return ラインに反応していれば1していなければ0を返す。
 */
unsigned char is_linesensor1(hal_linesensor linesensor);

//! 2番ビット判定関数
/*!
  \return ラインに反応していれば1していなければ0を返す。
 */
unsigned char is_linesensor2(hal_linesensor linesensor);

//! 3番ビット判定関数
/*!
  \return ラインに反応していれば1していなければ0を返す。
 */
unsigned char is_linesensor3(hal_linesensor linesensor);

//! 4番ビット判定関数
/*!
  \return ラインに反応していれば1していなければ0を返す。
 */
unsigned char is_linesensor4(hal_linesensor linesensor);

//! 5番ビット判定関数
/*!
  \return ラインに反応していれば1していなければ0を返す。
 */
unsigned char is_linesensor5(hal_linesensor linesensor);

//! 6番ビット判定関数
/*!
  \return ラインに反応していれば1していなければ0を返す。
 */
unsigned char is_linesensor6(hal_linesensor linesensor);

//! 7番ビット判定関数
/*!
  \return ラインに反応していれば1していなければ0を返す。
 */
unsigned char is_linesensor7(hal_linesensor linesensor);
#endif
