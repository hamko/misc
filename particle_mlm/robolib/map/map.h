/*!
  \file    map.h
  \ingroup map
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   マップの基本関数、依存関係の定義

  マップは大別して、独自に座標を計算する能力を持つrootマップと、その座標変換で得られる子マップ群に分かれる。
  マップリストでは親子の依存関係を保持し、子マップの座標を生成する。
 */

#include "install_option.h"
#ifndef __INSTALL_MAP__
#error map is not installed
#endif

#ifndef __INCLUDED_MAP_H__
#define __INCLUDED_MAP_H__

#include "r_stdint.h"
#include "unit_system.h"
#include "i_math.h"

//! 座標のセット
typedef struct {
    int32_t x;
    int32_t y;
    int32_t theta;
    int32_t v;
    int32_t omega;
} position_t;

//! マップ構造体
/*!
  原点情報を保持する。
 */
typedef struct {
    int32_t x;
    int32_t y;
    int32_t theta;
} map_t, *p_map_t;


//! マップライブラリの初期化
/*!
  マップリストをクリアする
 */
void init_map(void);

//! マップリストにマップを追加
/*!
  \param daughter 追加するマップ
  \param parent 親マップ rootの場合、NULLを渡す
 */
/* for implementer
  世代情報を付加し、世代順になるようリストに追加
 */
int add_map(p_map_t map, p_map_t parent);

//! マップリストからマップを削除
/*!
  子マップもすべて削除する
 */
int remove_map(p_map_t map);

//! 子マップの座標計算
/*!
  指定したマップおよびその子マップ全ての座標を更新する
 */
void renew_map(p_map_t map);

//! 指定したマップの座標を取得
/*!
  マップがリストに登録されていない場合はすべて0の座標が返される
 */
position_t get_map_pos(p_map_t map);

//! 指定したマップの座標を変更
/*!
  rootマップに対してのみ有効
  \retval 0 成功
  \retval -1 失敗
 */
int set_root_map_pos(p_map_t root_map, position_t pos);

//! 元の座標系でのベクトルが、他の座標系ではどう表されるか
/*!
  \param old_map 元の座標系
  \param new_map 対象とする座標系 元の座標系と同じrootマップを持たなければならない
  \param x       元の座標系でのx
  \param y       元の座標系でのy
 */
map_t get_vector_transform(p_map_t old_map, p_map_t new_map, int32_t x, int32_t y);

//! 元の座標系での座標が、他の座標系ではどう表されるか
/*!
  \param old_map 元の座標系
  \param new_map 対象とする座標系 元の座標系と同じrootマップを持たなければならない
 */
map_t get_pos_transform(p_map_t old_map, p_map_t new_map, int32_t x, int32_t y, int32_t theta);

//! 親マップを取得する
p_map_t get_parent(p_map_t map);

#endif
