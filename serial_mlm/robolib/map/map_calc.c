/*!
  \file    map_calc.c
  \ingroup map
  \author  Oka <tetramorium.tsushimae@gmail.com>
  \date    2010/07/18

  \brief   マップ操作に関する演算
 */

#include "install_option.h"
#ifdef __INSTALL_MAP__

#include "map_calc.h"
#include "map.h"
#include "r_stdint.h"
#include "i_math.h"

//! マップを平行移動させる
void transfer_map(p_map_t map, p_map_t origin, int32_t x, int32_t y)
{
    map_t dif; // 指定座標系におけるマップ移動ベクトル

    // originでの(x, y)をmapの親での表現に変更
    dif = get_vector_transform(origin, get_parent(map), x, y);

    // 親座標系でmap原点を指定移動ベクトル分を平行移動
    map->x += dif.x;
    map->y += dif.y;

    // map原点の変更をmapに反映
    renew_map(map);
}

//! マップ・サブマップを現在位置中心に回転
void rotate_map_here(p_map_t map, int32_t theta)
{
    position_t pos = get_map_pos(map);
    // mapの現在座標を中心にtheta回転
    rotate_map(map, map, pos.x, pos.y, theta);
}

//! マップを指定座標中心に回転
void rotate_map(p_map_t map, p_map_t origin, int32_t x, int32_t y, int32_t theta)
{
    map_t center; // 指定座標系における回転中心座標
    int32_t x1, y1; // 回転するマップの原点から指定座標系における回転中心座標への変位
    int32_t x2, y2; // 指定座標系における移動ベクトル

    // originで表された回転中心(x, y)をmapの親における表現で書き直す
    center = get_pos_transform(origin, get_parent(map), x, y, 0);

    // 親座標におけるmap原点から回転中心までの変位ベクトルを算出
    x1 = center.x - map->x;
    y1 = center.y - map->y;

    // 変位ベクトル(x1, y1)をtheta回転すると
    // 新しい変位ベクトル - 元の変位ベクトル = 原点の変化量
    x2 = x1 - multiply_divide(x1, i_cos(theta), UNIT_MM)
        + multiply_divide(y1, i_sin(theta), UNIT_MM);
    y2 = y1 - multiply_divide(x1, i_sin(theta), UNIT_MM)
        - multiply_divide(y1, i_cos(theta), UNIT_MM);

    // 原点を求めた変化量分平行移動し、移動後に新たな原点を中心にtheta回転
    map->x += x2;
    map->y += y2;
    map->theta += theta;

    // 原点及び角度の変更をmapに反映
    renew_map(map);
}


//! 現在位置・角度が指定位置・角度になるようにマップを修正
void set_map_pos(p_map_t map, p_map_t origin, int32_t x, int32_t y, int32_t theta)
{
    position_t old;
    p_map_t parent;
    map_t new;

    // originで表記された指定座標の親マップ上での表現を取得
    parent = get_parent(map);
    new = get_pos_transform(origin, parent, x, y, theta);

    // 親マップ上における現在座標を取得
    old = get_map_pos(parent);

    // newが新しいmapでの現在座標になるようにmap原点を指定
    // 子マップでの角度 = 親マップでの角度 - 原点角度
    map->theta = old.theta - new.theta;
    // 子マップでの座標ベクトル = (親マップでの座標ベクトル - 原点ベクトル)を-theta回転
    map->x = old.x - multiply_divide(new.x, i_cos(map->theta), UNIT_MM)
         + multiply_divide(new.y, i_sin(map->theta), UNIT_MM);
    map->y = old.y - multiply_divide(new.x, i_sin(map->theta), UNIT_MM)
         - multiply_divide(new.y, i_cos(map->theta), UNIT_MM);

    // map原点の変更をmapに反映
    renew_map(map);

    return;
}


#endif
