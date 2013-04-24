 /*!
  \file    odometry.c
  \ingroup map
  \author  Murooka Masaki <m117120@yahoo.co.jp>
  \date    2010/07/15

  \brief   オドメトリによるマップ生成
 */

#include "install_option.h"
#ifdef __INSTALL_MAP__

#include "odometry.h"
#include "function_list.h"
#include "i_math.h"
#include "labels.h"
#include "unit_system.h"

static map_t root = {0,0,0};   //!< ルートマップ
static map_t map = {0,0,0};    //!< マップ
static map_t submap = {0,0,0}; //!< サブマップ
static odometry_t p;       //!< パラメータ

static int32_t old_theta = 0; //!< 一周期前の角度。set_root_map_posでの変更を反映するためのもの。

static void renew_odometry(void);

//! オドメトリの初期化
void init_odometry(odometry_t params)
{
    set_function(renew_odometry, LABEL_MAP);
    p = params;

    // 各種原点情報を初期化
    root.x = root.y = 0;
    root.theta = multiply_divide(dv_get_encoder_total(p.right_encoder),
                                 p.step_right, p.encoder_width)
        - multiply_divide(dv_get_encoder_total(p.left_encoder),
                          p.step_left, p.encoder_width); // 基準角度の設定
    map.x = map.y = map.theta = 0;
    submap.x = submap.y = submap.theta = 0;
    old_theta = 0;
    
    add_map(&root, NULL);
    add_map(&map, &root);
    add_map(&submap, &map);
}

//! オドメトリの更新
static void renew_odometry(void)
{
    position_t pos;

    int32_t left_encoder_total, right_encoder_total; // エンコーダの積算値
    static int32_t old_left_encoder_total, old_right_encoder_total; // 1周期前のエンコーダの積算値
    int32_t d_left_encoder, d_right_encoder; // エンコーダの増分
    int32_t dx, dy, ds; // x,yと道のりの変位(0.0001mm, 0.0001mm)
    int32_t dtheta; // 角度の変位(0.0001rad)

    pos = get_map_pos(&root);

    // エンコーダの積算値を取得
    left_encoder_total = dv_get_encoder_total(p.left_encoder);
    right_encoder_total = dv_get_encoder_total(p.right_encoder);

    // エンコーダの増分を求める
    d_left_encoder = left_encoder_total - old_left_encoder_total;
    d_right_encoder = right_encoder_total - old_right_encoder_total;
    /* d_left_encoder = dv_get_encoder_vel(p.left_encoder); */
    /* d_right_encoder = dv_get_encoder_vel(p.right_encoder); */

    // 角度変化を求める。pos.thetaは今までのdthetaの総和なのでencoder_totalで求められる
    // エンコーダの精度はencoder_width(0.1mm)で直す
    //pos.theta = (right_encoder_total * p.step_right - left_encoder_total * p.step_left) / p.encoder_width;
    //dtheta = (d_right_encoder * p.step_right - d_left_encoder * p.step_left) / p.encoder_width;
    root.theta += pos.theta - old_theta;
    pos.theta = multiply_divide(right_encoder_total, p.step_right, p.encoder_width)
         - multiply_divide(left_encoder_total, p.step_left, p.encoder_width);
    printf("#theta %ld\n", pos.theta);
    dtheta = multiply_divide(d_right_encoder, p.step_right, p.encoder_width)
         - multiply_divide(d_left_encoder, p.step_left, p.encoder_width);

    pos.theta -= root.theta; // 基準角度から見たthetaの値
    old_theta = pos.theta;

    // 道のりの変位を求める (エンコーダの精度はマップの10倍なので、ここで直す)
    ds = (d_left_encoder * p.step_left + d_right_encoder * p.step_right) / 20;

    // 以上の値からdx, dyを計算
    dx = multiply_divide(ds, i_cos(pos.theta + dtheta/2), UNIT_MM);
    dy = multiply_divide(ds, i_sin(pos.theta + dtheta/2), UNIT_MM);

    // x,y,v,omega更新
    pos.x += dx;
    pos.y += dy;
    pos.v = ds * CYCLES_PER_SEC;
    pos.omega = dtheta * CYCLES_PER_SEC;

    old_left_encoder_total = left_encoder_total;
    old_right_encoder_total = right_encoder_total;

    set_root_map_pos(&root, pos);
}

//! ルートマップへのポインタを取得
p_map_t get_odometry_root(void)
{
    return &root;
}

//! マップへのポインタを取得
p_map_t get_odometry_map(void)
{
    return &map;
}

//! サブマップへのポインタを取得
p_map_t get_odometry_submap(void)
{
    return &submap;
}

#endif
