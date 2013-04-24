/*!
  \file   hal_port.h
  \author TAKISAWA, Jun'ichi.
  \date   Thu Mar 20 20:38:44 2008

  \brief  hal_prot.c用ヘッダファイル

 */

#ifndef _HAL_PORT_H_
#define _HAL_PORT_H_

#include "util.h"

//! 仮想ポートの不完全型宣言。詳細はhal_port_prv.h参照。
typedef struct hal_port_tag* hal_port;


//! hal_port初期化関数。
/*!
  \param port 初期化対象ポート
 */
void clear_port(hal_port port);

//! DDRに1バイトデータを設定する。
/*!
  \param port 設定対象ポート
  \param ddr 設定DDR値
 */
void set_ddr_byte(hal_port port, unsigned char ddr);

//! DDRの指定ビットを1にする。
/*!
  \param port 設定対象ポート
  \param bit 設定対象ビット番号(0~7)
 */
void set_ddr_bit(hal_port port, unsigned char bit);

//! DDRの指定ビットを0にする。
/*!
  \param port 設定対象ポート
  \param bit 設定対象ビット番号(0~7)
 */
void clear_ddr_bit(hal_port port, unsigned char bit);

//! 現行DDRの情報を取得する。
/*!
  \param port 対象ポート

  \return 対象ポートのDDR値
 */
unsigned char get_ddr_byte(hal_port port);

//! 現行のDDRの指定ビットの値を取得する。
/*!
  \param port 対象ポート
  \param bit 対象ビット番号(0~7)

  \return 対象ビットの値(0 or 1)
 */
unsigned char get_ddr_bit(hal_port port, unsigned char bit);

//! DRに1バイトデータを設定する。
/*!
  \param port 設定対象ポート
  \param dr 設定DR値。
 */
void set_dr_byte(hal_port port, unsigned char dr);

//! DRの指定ビットを1にする。
/*!
  \param port 設定対象ポート
  \param bit 設定対象ビット
 */
void set_dr_bit(hal_port port, unsigned char bit);

//! DRの指定ビットを0にする。
/*!
  \param port 設定対象ポート
  \param bit 設定対象ビット
 */
void clear_dr_bit(hal_port port, unsigned char bit);

//! 現行のDR情報を取得する。
/*!
  \param port 対象ポート

  \return 対象ポートのDR情報
 */
unsigned char get_dr_byte(hal_port port);

//! 現行のDRの指定ビット情報を取得する。
/*!
  \param port 対象ポート
  \param bit 対象ビット

  \return 対象ビットの情報(0 or 1)
 */
unsigned char get_dr_bit(hal_port port, unsigned char bit);

#endif
