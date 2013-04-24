/*!
  \file    duty_odometry.c
  \ingroup duty_odometry
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/09

  \brief   Dutyの積算で簡易的なマップを生成
  \see     duty_odometry.h map.h

  このマップを利用することで、エンコーダ、およびマップの利用を前提とした機能へアクセスが可能になる。
  ただし精度はまったく期待できない。

  エンコーダから生成したマップとこのマップを比較することで、モーターのパラメータを補正することができないだろうか?というアイディアもなくはない。

  \par 原理:
  モデル \f$Duty = \frac{aDT}{k_a} +\frac{v}{k_v} + r \f$を用いると、
  \f[
  \left( \begin{array}{c} v\\ dv \end{array} \right)_{new}
  =
  \left(\begin{array}{cc}
      1 & 1 \\
    -2\frac{k_{a}}{k_{v}} & -1
  \end{array}\right)
  \left(\begin{array}{c} v\\ dv \end{array} \right)
  +
  2k_{a}(Duty-r)
  \left(\begin{array}{c} 0\\ 1 \end{array} \right)
  \f]
  として1周期前の情報から\f$v, dv\f$を更新できる。\n
  但し、加速度を
  \f[2a(t)DT = v(t+2DT) - v(t)\f]
  と置き換えるやや特殊な離散化を取っている(推定が行える離散化の中で最も奇抜でないものを選ぶとこうなる)。
 */

#include "install_option.h"
#ifdef __INSTALL_DUTY_ODOMETRY__

#include "device.h"
#include "duty_odometry.h"
#include "dv_motor.h"
#include "function_list.h"
#include "i_math.h"
#include "labels.h"
#include "map.h"
#include "r_stdint.h"
#include "unit_system.h"

static map_t root = {0,0,0};   //!< ルートマップ
static map_t map = {0,0,0};    //!< マップ
static map_t submap = {0,0,0}; //!< サブマップ
static duty_odometry_t p;       //!< パラメータ

static void renew_duty_odometry(void);


//! Dutyオドメトリの初期化
void init_duty_odometry(duty_odometry_t params)
{
    set_function(renew_duty_odometry, LABEL_MAP);
    add_map(&root, NULL);
    add_map(&map, &root);
    add_map(&submap, &map);
    p = params;
}

//! Dutyオドメトリの更新
static void renew_duty_odometry(void)
{
    position_t pos;
    static int32_t dv, domega;
    int32_t old_v, old_omega;

    pos = get_map_pos(&root);

    // x,yの更新
    pos.x += multiply_divide(pos.v, i_cos(pos.theta), (int32_t)UNIT_RAD*100);
    pos.y += multiply_divide(pos.v, i_sin(pos.theta), (int32_t)UNIT_RAD*100);
    pos.theta += pos.omega / 100;

    old_v = pos.v;
    old_omega = pos.omega;

    // v, omegaの更新
    pos.v += dv;
    dv = -dv - multiply_divide(2*old_v, p.k_a, p.k_v)
         + p.k_a*(dv_get_signed_duty(p.right_motor)+dv_get_signed_duty(p.left_motor) - 2*p.r);

    pos.omega += domega;
    domega = -domega - multiply_divide(2*old_omega, p.k_a, p.k_v)
         + multiply_divide(p.k_a,dv_get_signed_duty(p.right_motor)-dv_get_signed_duty(p.left_motor),p.width) - 2*p.r*p.k_a;

    set_root_map_pos(&root, pos);
}

//! ルートマップへのポインタを取得
p_map_t get_duty_odometry_root(void)
{
    return &root;
}

//! マップへのポインタを取得
p_map_t get_duty_odometry_map(void)
{
    return &map;
}

//! サブマップへのポインタを取得
p_map_t get_duty_odometry_submap(void)
{
    return &submap;
}

#endif
