/*!
  \file    locus.h
  \ingroup locus
  \author  Sota <jine0125@gmail.com>
  \date    10/09/24

  \brief   関数追従
 */

#include "install_option.h"
#ifndef __INSTALL_LOCUS__
#error locus is not installed
#endif

#ifndef __INCLUDED_LOCUS_H__
#define __INCLUDED_LOCUS_H__

#include "r_stdint.h"
#include "map.h"

typedef struct {
    p_map_t map;  //!< 利用するマップ
    p_map_t submap;  //!< 利用するサブマップ
    int32_t l;  //!< フィードバック項における最小曲率半径 (mm)
} go_locus_t;

//! 経路関数のセット
/*!
  目標経路(X(s),Y(s))と走行速度、終点のセット
  X,Yのs微分も関数で与える必要がある。
 */
typedef struct {
    int32_t (*X)(int32_t);
    int32_t (*Y)(int32_t);
    int32_t (*dX)(int32_t);  //!< d/ds X
    int32_t (*dY)(int32_t);  //!< d/ds Y
    int32_t (*ddX)(int32_t); //!< (d/ds)^2 X
    int32_t (*ddY)(int32_t); //!< (d/ds)^2 Y
    int32_t (*v)(int32_t);
    int32_t s_end; //!< 終点のパラメータ
} locus_t;

//! 関数追従の初期化
void init_go_locus(go_locus_t params);

//! 関数追従
/*!
  与えられた関数に沿って走行する
  関数上でマシンの初期位置は(0,0,0)。
 */
void go_locus_with(void (*block)(void), locus_t locus);
void go_locus(locus_t locus);

//! go_locusのバック版。
/*!
  \warning 関数の符号に注意すること。
 */
void go_back_locus_with(void (*block)(void), locus_t locus);
void go_back_locus(locus_t locus);

/******オレオレ関数追従指令関数******/
//関数の精度によるsubmapのづれを防止するためのもの

//nomalエディション
void go_locus_spline_with(void (*block)(void), locus_t locus,int32_t x,int32_t y,int32_t theta);
void go_locus_spline(locus_t locus,int32_t x,int32_t y,int32_t theta);
//backバージョン
void go_back_locus_spline_with(void (*block)(void), locus_t locus,int32_t x,int32_t y,int32_t theta);
void go_back_locus_spline(locus_t locus,int32_t x,int32_t y,int32_t theta);


void block_go_locus_spline(void);
void get_seed_from_locus(void);

#endif
