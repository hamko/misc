/*!
 \file    mlm.h
 \ingroup mlm
 \author  Iori <ik0313@gmail.com>
 \date    2010/08/12
 
 \brief   mlmによるマップ補正を行うための関数群
 
 詳細説明
 */

#include "install_option.h"
#ifndef __INSTALL_MLM__
#error mlm is not installed
#endif

#ifndef __INCLUDED_MLM_H__
#define __INCLUDED_MLM_H__

#include "map.h"
#include "map_calc.h"
#include "dv_linesensor.h"

//! mlm初期化関数
/*!
 \param target_map            対象のマップ(フィールドマップ系を指定)
 \param mlm_sources           使用するラインセンサリスト構造体
 \param calc_line_to_sample   サンプル-ライン間距離導出関数へのポインタ
 !! 返り値が0以外の場合control_mlmは該当サンプルを捨てる !!
 \param v_threshold           サンプリングを許可する最低速度(単位1mm/s)
 \param init_exec_sample_num  補正を許可するサンプル数の初期値
 \param exec_sample_increment 補正を許可するサンプル数の補正一回あたり増分(0なら一定)
 \param max_exec_sample_num   補正を許可するサンプル数の最大値
 \param shift                 サンプルの桁溢れ防止のためのシフト数(x, y用)
 \param shift_d               サンプルの桁溢れ防止のためのシフト数(xd, yd用)
 \param x_offset              補正対象マップ原点のライン格子点からのx方向ずれ(単位1/UNIT_MMmm)
 \param y_offset              補正対象マップ原点のライン格子点からのy方向ずれ(単位1/UNIT_MMmm)
 \param theta_offset          補正対象マップのフィールドに対する角度ずれ(単位1/UNIT_RADrad)
 
 mlmに必要な定数群を初期化する。
 xd, ydはそれぞれ最も近いy, x軸に平行なラインから反応点までの変位
 内部関数control_mlmをset_functionしてmlm関連を初期化するところまでやる
 (同名関数がfunction_list中にある場合はset_functionしない)。
 offset群は補正対象の元マップを平行・回転移動して
 格子点を原点とする新しいサンプリング対象マップを作るために利用する。
 以降サンプリングはこの新しいマップ上で行う。
 (-x_offset, -y_offset, -theta_offset)がサンプリング用のマップ原点になる。
 サンプル-ライン間距離導出関数へのポインタはサンプル座標(x, y)を元に
 そのサンプルから直近のラインまでの距離(xd, yd)を導出する関数を指定する。(xd, yd)についての詳細はドキュメント参照
 
 */
void init_mlm_with(p_map_t original_map, p_lsl_package_t mlm_sources,
                   int16_t (*calc_line_to_sample)(int64_t x, int64_t y, int64_t *xd, int64_t *yd),
                   int64_t v_threshold,
                   int64_t init_exec_sample_num, int64_t exec_sample_increment, int64_t max_exec_sample_num,
                   int16_t shift, int16_t shift_d,
                   int64_t x_offset, int64_t y_offset, int64_t theta_offset);

//! mlm初期化関数
/*!
 \param target_map            対象のマップ(フィールドマップ系を指定)
 \param mlm_sources           使用するラインセンサリスト構造体
 \param line_interval         ライン間隔(単位1mm)
 \param v_threshold           サンプリングを許可する最低速度(単位1mm/s)
 \param init_exec_sample_num  補正を許可するサンプル数の初期値
 \param exec_sample_increment 補正を許可するサンプル数の補正一回あたり増分(0なら一定)
 \param max_exec_sample_num   補正を許可するサンプル数の最大値
 \param shift                 サンプルの桁溢れ防止のためのシフト数(x, y用)
 \param shift_d               サンプルの桁溢れ防止のためのシフト数(xd, yd用)
 \param x_offset              補正対象マップ原点のライン格子点からのx方向ずれ(単位1/UNIT_MMmm)
 \param y_offset              補正対象マップ原点のライン格子点からのy方向ずれ(単位1/UNIT_MMmm)
 \param theta_offset          補正対象マップのフィールドに対する角度ずれ(単位1/UNIT_RADrad)
 
 mlmに必要な定数群を初期化する。
 基本的にinit_mlm_with関数と同じだが、サンプル-ライン間距離導出関数はデフォルトの関数を利用する。
 デフォルトのサンプル-ライン間距離導出関数はline_intervalでサンプルをline_intervalで丸めた余りを
 サンプル-ライン間距離とする
 */
void init_mlm(p_map_t original_map, p_lsl_package_t mlm_sources, int64_t line_interval, int64_t v_threshold,
              int64_t init_exec_sample_num, int64_t exec_sample_increment, int64_t max_exec_sample_num,
              int16_t shift, int16_t shift_d,
              int64_t x_offset, int64_t y_offset, int64_t theta_offset);

//! mlm実行開始フラグを操作する関数
/*!
 \param flag 設定フラグ
 
 TRUEにするとmlmを開始する
 */
void set_execute_mlm_flag(BOOLEAN flag);

#endif
