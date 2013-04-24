/*!
  \file    locus.c
  \ingroup locus
  \author  Sota <jine0125@gmail.com>
  \date    10/09/24

  \brief   関数追従
 */

#include "install_option.h"
#ifdef __INSTALL_LOCUS__

#include "i_math.h"
#include "drive_stop.h"
#include "drive_tw.h"
#include "function_list.h"
#include "kakitani.h"
#include "locus.h"
#include "map_calc.h"
#include "operate.h"
#include "r_stdint.h"
#include "unit_system.h"
#include "util.h"

#ifdef __INSTALL_CORDIC__
#include "cordic.h"
#endif

#define DEFAULT_L 500   //!< 最小曲率半径のデフォルト値(mm)

#define __DEBUG_LOCUS__

static go_locus_t _params;  //!< パラメータ
static locus_t _locus;    //!< 追従関数
static map_t funcmap = {0,0,0}; //!< 関数マップ。s=0を原点とする

static int32_t s = 0;    //!< 最近点パラメータ。単位 0.0001mm

static BOOLEAN run_back = FALSE;    //!< バック走行か否か

static int32_t calc_ds(void);
static int32_t calc_dest_omega(int32_t dest_v);
static void control_go_locus(void);
static void control_go_locus_spline(void);
static void block_go_locus(void);

extern int32_t kakitani_control(int32_t y, int32_t theta, int32_t v, int32_t l);   //!< 柿谷制御


//! 柿谷制御の初期化。必要なパラメータを設定
void init_go_locus(go_locus_t params)
{
    _params = params;
    if (!_params.l){
        _params.l = DEFAULT_L;
    }
}

//! 2変数arctan。
static int32_t my_atan2(int32_t x, int32_t y)
{
#ifndef __INSTALL_CORDIC__
    uint16_t r;
    int16_t theta;

    // 32bit->16bit
    while(ABS(x) > 0x7fff || ABS(y) > 0x7fff){
        x>>=1;
        y>>=1;
    }

    polar(x,y,&r,&theta);
    return theta;
#else
    int32_t r;
    int32_t theta;

    c_polar(x,y,&r,&theta);
    return theta;
#endif
}


//! go_locus用待機関数。バックグラウンドで最近点を更新しつづける。
static void wait_go_locus(void)
{
    int32_t ds;
    while(get_function(LABEL_BLOCK)!=NULL){
        ds = calc_ds();
        s+=ds;
    }
}


//! 関数追従
static void _go_locus_with(void (*block)(void), locus_t locus)
{
    position_t pos;
    s = 0;
    _locus = locus;
    // 関数マップの生成。
    // 開始時の理想的位置、すなわち前回のサブマップ原点を原点とする
    funcmap = *_params.submap;
    add_map(&funcmap, _params.map);
    // サブマップの移動。
    // ただし角度は2piの整数倍のずれが生じうる。
    // 関数追従内部ではこれが問題となる処理は存在しない。
    transfer_map(_params.submap, _params.submap, _locus.X(_locus.s_end), _locus.Y(_locus.s_end));
    rotate_map(_params.submap, _params.submap, 0, 0, my_atan2( _locus.dX(_locus.s_end), _locus.dY(_locus.s_end) ));
    printf("# s_end:%ld\n",_locus.s_end);
    printf("# submap:%ld %ld %ld\n",_params.submap->x,_params.submap->y,_params.submap->theta);
    // バック走行の場合終端でのマシン方向がpi異なる
    if (run_back){
        rotate_map(_params.submap, _params.submap, 0, 0, pi);
    }

    set_drive_function(control_go_locus);
    set_function(block, LABEL_BLOCK);
    wait_go_locus();
    // サブマップの周回分の角度ずれを修正
    // 走行中に前進後退が入れ替わる場合(経路中に特異点が含まれる場合)を考慮し、2piではなくpiの整数倍のずれを修正する
    pos = get_map_pos(_params.submap);
    rotate_map(_params.submap, _params.submap, 0, 0, pos.theta - (arrange_angle(pos.theta<<1)>>1) );

    remove_map(&funcmap);
}

//! 関数追従
/*!
  与えられた関数に沿って走行する
  関数上でマシンの初期位置は(0,0,0)。
 */
void go_locus_with(void (*block)(void), locus_t locus)
{
    run_back = FALSE;
    _go_locus_with(block, locus);
}

//! 関数追従
/*!
  与えられた関数に沿って走行する
  関数上でマシンの初期位置は(0,0,0)。
 */
void go_locus(locus_t locus)
{
    run_back = FALSE;
    _go_locus_with(block_go_locus, locus);
}

//! go_locusのバック版。
/*!
  \warning 関数の符号に注意すること。
 */
void go_back_locus_with(void (*block)(void), locus_t locus)
{
    run_back = TRUE;
    _go_locus_with(block, locus);
    run_back = FALSE;
}

//! go_locusのバック版。
/*!
  \warning 関数の符号に注意すること。
 */
void go_back_locus(locus_t locus)
{
    run_back = TRUE;
    _go_locus_with(block_go_locus, locus);
    run_back = FALSE;
}

//! 関数追従用終了条件
static void block_go_locus(void)
{
    position_t pos;
    pos = get_map_pos(_params.submap);

    if (( run_back && pos.x > 10*UNIT_MM ) || ( !run_back && pos.x < -10*UNIT_MM )){
        return;
    }

    if( (ABS(pos.y) < 100*UNIT_MM
         && ABS(arrange_angle(pos.theta<<1)>>1) < 5000
         && s > multiply_divide(_locus.s_end,9,10))
      || s > _locus.s_end){
        remove_function(block_go_locus);
        set_drive_function(drive_stop);
    }
}

//! 関数追従での割り込み処理
static void control_go_locus(void)
{
    position_t pos;
    int32_t dest_v;
    
    // 最近点更新
    s += calc_ds();
    pos = get_map_pos(&funcmap);
    if (_locus.v(s) - pos.v > 0){
        dest_v = pos.v + MIN(_locus.v(s) - pos.v, 1000000);
    }
    else {
        dest_v = pos.v + MAX(_locus.v(s) - pos.v, -1000000);
    }
    tw_set_v_omega(dest_v, calc_dest_omega(dest_v));
    #ifdef __DEBUG_LOCUS__
    //logger_locus();
    #endif
}

//! 最近点パラメータの更新
static int32_t calc_ds(void)
{
    position_t pos;
    int32_t eta;      // 接線角度
#ifdef USE_NEWTON_METHOD
    int32_t rev = 0;  // dsの二次補正値
#endif

    pos = get_map_pos(&funcmap);


#ifndef __INSTALL_CORDIC__
    eta = my_atan2( _locus.dX(s), _locus.dY(s) );

#ifdef USE_NEWTON_METHOD
    rev  =  multiply_divide(pos.x - _locus.X(s), _locus.ddX(s), UNIT_MM)
         +  multiply_divide(pos.y - _locus.Y(s), _locus.ddY(s), UNIT_MM);
    if (ABS(i_sin(eta)) < i_sin(pi/4)){
        rev  =  multiply_divide(rev, UNIT_RAD+i_cos(2*eta), 2*_locus.dX(s));
    }
    else {
        rev  =  multiply_divide(rev, UNIT_RAD-i_cos(2*eta), 2*_locus.dY(s));
    }
#endif

    if (ABS(i_sin(eta)) < i_sin(pi/4)){
        return 
            multiply_divide(pos.x - _locus.X(s), UNIT_RAD+i_cos(2*eta), 2*(_locus.dX(s)-rev))
          + multiply_divide(pos.y - _locus.Y(s),     i_sin(2*eta),      2*(_locus.dX(s)-rev));
    }
    else {
        return 
            multiply_divide(pos.x - _locus.X(s),     i_sin(2*eta),      2*(_locus.dY(s)-rev))
          + multiply_divide(pos.y - _locus.Y(s), UNIT_RAD-i_cos(2*eta), 2*(_locus.dY(s)-rev));
    }

#else
    int32_t dL;

    c_polar(_locus.dX(s), _locus.dY(s), &dL, &eta);

#ifdef USE_NEWTON_METHOD
    rev  =  multiply_divide(pos.x - _locus.X(s), _locus.ddX(s), dL)
         +  multiply_divide(pos.y - _locus.Y(s), _locus.ddY(s), dL);
#endif

    return 
        multiply_divide(pos.x - _locus.X(s), i_cos(eta), dL - rev)
      + multiply_divide(pos.y - _locus.Y(s), i_sin(eta), dL - rev);
#endif
}

//! 目標角速度導出
static int32_t calc_dest_omega(int32_t dest_v)
{
    position_t pos;
    int32_t eta;          // 接線角度
    int32_t d, phi, v_r;  // 最近距離、接線とのなす角、目標速度/理想半径

    pos = get_map_pos(&funcmap);

#ifndef __INSTALL_CORDIC__
    eta = my_atan2( _locus.dX(s), _locus.dY(s) );

    if (ABS(i_sin(eta)) < i_sin(pi/4)){
        v_r = 
            multiply_divide(
                ABS(dest_v),
                   multiply_divide(_locus.ddY(s), 3*i_cos(eta)+i_cos(3*eta), _locus.dX(s)*2)
                 - multiply_divide(_locus.ddX(s),   i_sin(eta)+i_sin(3*eta), _locus.dX(s)*2),
                _locus.dX(s)*2
            );
    }
    else {
        v_r =
            multiply_divide(
                ABS(dest_v),
                   multiply_divide(_locus.ddY(s),   i_cos(eta)-i_cos(3*eta), _locus.dY(s)*2)
                 - multiply_divide(_locus.ddX(s), 3*i_sin(eta)-i_sin(3*eta), _locus.dY(s)*2),
                _locus.dY(s)*2
            );
    }
#else
    int32_t dL;  // 孤長のパラメータ微分

    c_polar(_locus.dX(s), _locus.dY(s), &dL, &eta);

    v_r = 
        multiply_divide(
            ABS(dest_v),
               multiply_divide(_locus.ddY(s), i_cos(eta), dL)
             - multiply_divide(_locus.ddX(s), i_sin(eta), dL),
            dL
        );    
#endif
    d = multiply_divide(pos.y - _locus.Y(s), i_cos(eta), UNIT_RAD) - multiply_divide(pos.x - _locus.X(s), i_sin(eta), UNIT_RAD);
    phi = arrange_angle(pos.theta - eta + (dest_v < 0 ? pi : 0));

    return kakitani_control(d, phi, ABS(dest_v), _params.l) + v_r;
}

//! デバッグ用 ログ出力
void logger_locus(void)
{
    printf("%ld %ld %ld\n",_locus.X(s)/UNIT_MM, _locus.Y(s)/UNIT_MM, s);
}

/******オレオレ関数追従指令関数******/
//関数の精度によるsubmapのづれを防止するためのもの

// 杉本用
static void _get_seed_from_locus(void){
    if(get_function(LABEL_BLOCK)==NULL){return;}
    position_t pos;
    pos=get_map_pos(_params.map);
    printf("%ld %ld\n",pos.v,pos.omega);
    return;
}

// 関数追従
static void _go_locus_spline_with(void (*block)(void), locus_t locus,int32_t x,int32_t y,int32_t theta)
{
    position_t pos;

    s = 0;
    _locus = locus;

    // 関数マップの生成。
    // 開始時の理想的位置、すなわち前回のサブマップ原点を原点とする
    funcmap = *_params.submap;
    add_map(&funcmap, _params.map);

    // サブマップの移動。
    // 関数の終点ではなく入力された終端座標により移動させる。
    // ただし角度は2piの整数倍のずれが生じうる。
    // 関数追従内部ではこれが問題となる処理は存在しない。
    transfer_map(_params.submap, _params.submap, x*UNIT_MM, y*UNIT_MM);
    rotate_map(_params.submap, _params.submap, 0, 0, theta );
    #ifdef __DEBUG_LOCUS__
    printf("submap x:%ld y:%ld theta:%ld\n",_params.submap->x,_params.submap->y,_params.submap->theta);
    #endif
    // バック走行の場合終端でのマシン方向がpi異なる
    if (run_back){
        rotate_map(_params.submap, _params.submap, 0, 0, pi);
    }

    set_drive_function(control_go_locus);
    set_function(block, LABEL_BLOCK);
    wait_go_locus();

    // サブマップの周回分の角度ずれを修正
    // 走行中に前進後退が入れ替わる場合(経路中に特異点が含まれる場合)を考慮し、2piではなくpiの整数倍のずれを修正する
    pos = get_map_pos(_params.submap);
    rotate_map(_params.submap, _params.submap, 0, 0, pos.theta - (arrange_angle(pos.theta<<1)>>1) );
    #ifdef __DEBUG_LOCUS__
    printf("x:%ld y:%ld theta:%ld\n",pos.x,pos.y,pos.theta);
    pos=get_map_pos(_params.map);
    printf("x:%ld y:%ld theta:%ld\n",pos.x,pos.y,pos.theta);
    #endif
    remove_map(&funcmap);
}

//nomalエディション
void go_locus_spline_with(void (*block)(void), locus_t locus,int32_t x,int32_t y,int32_t theta){
    run_back = FALSE;
    _go_locus_spline_with(block, locus,x,y,theta);
}
void go_locus_spline(locus_t locus,int32_t x,int32_t y,int32_t theta){
    run_back = FALSE;
    _go_locus_spline_with(block_go_locus, locus,x,y,theta);
}
//backバージョン
void go_back_locus_spline_with(void (*block)(void), locus_t locus,int32_t x,int32_t y,int32_t theta){
    run_back = TRUE;
    _go_locus_spline_with(block, locus,x,y,theta);
    run_back = FALSE;
}

void go_back_locus_spline(locus_t locus,int32_t x,int32_t y,int32_t theta){
    run_back = TRUE;
    _go_locus_spline_with(block_go_locus, locus,x,y,theta);
    run_back = FALSE;
}

//関数追従用終了条件オレオレエディション
void block_go_locus_spline(void)
{
    position_t pos;
    pos = get_map_pos(_params.submap);

    if (( run_back && pos.x > 0 ) || ( !run_back && pos.x < 0)){
        return;
    }

    if( (ABS(pos.y) < 100*UNIT_MM
         && ABS(arrange_angle(pos.theta<<1)>>1) < 5000
         && s > multiply_divide(_locus.s_end,9,10))
      || s > _locus.s_end){
        remove_function(block_go_locus_spline);
        set_drive_function(drive_stop);
    }
    #ifdef __DEBUG_LOCUS__
    //printf("x:%ld y:%ld theta:%ld\n",pos.x,pos.y,pos.theta);
    #endif
}

void get_seed_from_locus(void){
    set_function(_get_seed_from_locus, LABEL_USER_POST);
    return;
}



#endif
