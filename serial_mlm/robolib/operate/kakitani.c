/*!
  \file    kakitani.c
  \ingroup kakitani
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/12

  \brief   柿谷制御
  \todo    マップとの単位系の摺り合わせ
 */

#include "install_option.h"
#ifdef __INSTALL_KAKITANI__

#include "dv_motor.h"
#include "drive_stop.h"
#include "drive_tw.h"
#include "function_list.h"
#include "i_math.h"
#include "kakitani.h"
#include "labels.h"
#include "map_calc.h"
#include "operate.h"
#include "timer_utility.h"
#include "unit_system.h"

#define CYCLES_PER_SEC 100

static kakitani_t _params;  //!< パラメータ群
static int32_t _v;  //!< 目標速度 (単位:0.0001mm/s)
static int32_t _r;  //!< 最小曲率半径 (単位:mm)
static int32_t _a;  //!< 停止加速度 (単位:mm/s^2)
static int32_t t_s; //!< 制御開始時刻(go_const用)
static int32_t t_c; //!< 制御時間(go_const用)
static int32_t _term_v;		//!< 終端速度の二乗(go_accele用)

int32_t kakitani_control(int32_t y, int32_t theta, int32_t v, int32_t l);
static void control_go_curve(void);
static void control_go_stop(void);
static void control_turn(void);
static void control_go_back_stop(void);
static void control_go_accele(void);
static void control_go_const(void);
static void block_go_curve(void);
static void block_go_back_curve(void);
static void block_turn(void);
static void block_go_const(void);
static void block_go_accele(void);
static void block_turn_stay(void);


//! 柿谷制御の初期化。必要なパラメータを設定
void init_kakitani(kakitani_t params)
{
    _params = params;
}


//! 目標角速度の導出。柿谷制御の神髄。
/*!
  \return 目標角速度(0.0001rad/s)
  \param y 目標直線からの距離(0.0001mm)
  \param theta 目標直線とマシンの成す角(0.0001rad)
  \param v 目標速度(0.0001mm/s)
  \param l 最小曲率半径(mm)
 */
int32_t kakitani_control(int32_t y, int32_t theta, int32_t v, int32_t l)
{
    int32_t alpha, dest_omega;
    int32_t v_l;

    // バックするとき
    // 進む方向がマシン方向と逆なので目標点を伸ばす方向も逆
    if(v < 0){
        l = -l;
    }

    v_l = v/l;

    alpha = y/l;
    if(alpha > pi/2){
        alpha = pi/2;
    }else if(alpha < -pi/2){
        alpha = -pi/2;
    }

    dest_omega = -multiply_divide(v_l*_params.k, alpha+theta, UNIT_MM);
    if(dest_omega > v_l){
        dest_omega = v_l;
    }
    if(dest_omega < -v_l){
        dest_omega = -v_l;
    }

    return dest_omega;
}



/***************************************************************************************************/
// 制御指令関数(blockパッケージ済)

//! カーブを描きながら走行する
void go_curve(int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r)
{
    go_curve_with(block_go_curve,x,y,theta,v,r);
}

//! カーブを描きながら走行し、目標地点で止まる
void go_stop(int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r, int32_t a)
{
    go_stop_with(block_go_curve,x,y,theta,v,r,a);   // ブロック関数はgo_curveと同じ。
}

//! 心地旋回する
void turn(int32_t theta)
{
    turn_with(block_turn, theta);
}

//! go_curveのバック版。
/*!
  \warning xの符号に注意すること。
 */
void go_back_curve(int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r)
{
    go_back_curve_with(block_go_back_curve,x,y,theta,v,r);
}

//! go_stopのバック版。
/*!
  \warning xの符号に注意すること。
 */
void go_back_stop(int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r, int32_t a)
{
    go_back_stop_with(block_go_back_curve,x,y,theta,v,r,a);   // ブロック関数はgo_back_curveと同じ。
}

//! 指定した角度になるように心地旋回する
void turn_to(int32_t theta)
{
    turn_to_with(block_turn, theta);
}

//! 指定した角度になるように心地旋回する
/*!
  通常時はブレーキ、外部から回転させられると戻す。
  一度呼ぶと帰ってこない子。
 */
void turn_stay(int32_t theta){
    turn_to_with(block_turn_stay, theta);
}

//! go_curveに加え、終端付近で目標終端速度へ向けて加減速する
void go_accele(int32_t x, int32_t y, int32_t theta, int32_t v, int32_t term_v, int32_t r, int32_t a)
{
    go_accele_with(block_go_accele,x,y,theta,v,term_v,r,a);
}

//! 直進性を確保しつつ、推進DUTYを一定値かける
void go_const(int32_t c_F, int32_t r, int32_t control_time)
{
    go_const_with(block_go_const,c_F,r,control_time);
}

/***************************************************************************************************/
// 制御指令関数

//! カーブを描きながら走行する
void go_curve_with(void (*block)(void), int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r)
{
    // 目標地点をサブマップ原点に設定する
    transfer_map(_params.submap, _params.submap, x*UNIT_MM, y*UNIT_MM);
    rotate_map(_params.submap, _params.submap, 0, 0, theta);

    // パラメータ設定
    _v = ABS(v)*UNIT_MM;
    _r = ABS(r);

    // 関数登録、制御終了まで待機
    set_drive_function(control_go_curve);
    set_function(block, LABEL_BLOCK);
    wait_for_control();
}

//! カーブを描きながら走行し、目標地点で止まる
void go_stop_with(void (*block)(void), int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r, int32_t a)
{
    // 目標地点をサブマップ原点に設定する
    transfer_map(_params.submap, _params.submap, x*UNIT_MM, y*UNIT_MM);
    rotate_map(_params.submap, _params.submap, 0, 0, theta);

    // パラメータ設定
    _v = ABS(v)*UNIT_MM;
    _r = ABS(r);
    _a = -ABS(a);

    // 関数登録、制御終了まで待機
    set_drive_function(control_go_stop);
    set_function(block, LABEL_BLOCK);
    wait_for_control();
}

//! 心地旋回する
void turn_with(void (*block)(void), int32_t theta)
{
    // 目標地点をサブマップ原点に設定する
    rotate_map_here(_params.submap, theta);

    // 関数登録、制御終了まで待機
    set_drive_function(control_turn);
    set_function(block, LABEL_BLOCK);
    wait_for_control();
}

//! go_curveのバック版。
/*!
  \warning xの符号に注意すること。
 */
void go_back_curve_with(void (*block)(void), int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r)
{
    // 目標地点をサブマップ原点に設定する
    transfer_map(_params.submap, _params.submap, x*UNIT_MM, y*UNIT_MM);
    rotate_map(_params.submap, _params.submap, 0, 0, theta);

    // パラメータ設定
    _v = -ABS(v)*UNIT_MM;
    _r = ABS(r);

    // 関数登録、制御終了まで待機
    set_drive_function(control_go_curve);    // go_curveと同じ
    set_function(block, LABEL_BLOCK);
    wait_for_control();

}

//! go_stopのバック版。
/*!
  \warning xの符号に注意すること。
 */
void go_back_stop_with(void (*block)(void), int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r, int32_t a)
{
    // 目標地点をサブマップ原点に設定する
    transfer_map(_params.submap, _params.submap, x*UNIT_MM, y*UNIT_MM);
    rotate_map(_params.submap, _params.submap, 0, 0, theta);

    // パラメータ設定
    _v = -ABS(v)*UNIT_MM;
    _r = ABS(r);
    _a = ABS(a);

    // 関数登録、制御終了まで待機
    set_drive_function(control_go_back_stop);
    set_function(block, LABEL_BLOCK);
    wait_for_control();
}

//! 指定した角度になるように心地旋回する
void turn_to_with(void (*block)(void), int32_t theta)
{
    position_t pos;
    pos = get_map_pos(_params.submap);
    set_map_pos(_params.submap,_params.submap,0,0,0);//ここはもうちょっとまともに書いたほうがいいかも。適当すぎる。
    rotate_map_here(_params.submap,theta-pos.theta);

    // 関数登録、制御終了まで待機
    set_drive_function(control_turn);
    set_function(block, LABEL_BLOCK);
    wait_for_control();
}

//! 終端付近で目標終端速度へ向けて加減速する
void go_accele_with(void (*block)(void), int32_t x, int32_t y, int32_t theta, int32_t v, int32_t term_v, int32_t r, int32_t a){
    // 目標地点をサブマップ原点に設定する
    transfer_map(_params.submap, _params.submap, x*UNIT_MM, y*UNIT_MM);
    rotate_map(_params.submap, _params.submap, 0, 0, theta);

    _v = ABS(v)*UNIT_MM;
    _term_v = term_v*term_v;
    _r = ABS(r);
    _a = ABS(a);

    // 関数登録、制御終了まで待機
    set_drive_function(control_go_accele);
    set_function(block, LABEL_BLOCK);
    wait_for_control();

    return;
}

//! 直進性を確保しつつ、推進DUTYを一定値かける
void go_const_with(void (*block)(void), int32_t c_F, int32_t r, int32_t control_time)
{
    set_map_pos(_params.submap, _params.submap, 0, 0, 0);
    _v = c_F;	// 擬似的に目標速度にDutyを格納
    _r = ABS(r);
    t_s = get_time();
    t_c = control_time;

    // 関数登録、制御終了まで待機
    set_drive_function(control_go_const);
    set_function(block, LABEL_BLOCK);
    wait_for_control();
}

/***************************************************************************************************/
// control関数

//! go_curveにおける速度、角速度制御
static void control_go_curve(void)
{
    position_t pos;
    int32_t v, omega;

    pos = get_map_pos(_params.submap);

    // 加速度制限
    if (_v - pos.v > 0){
        v = pos.v + MIN(_v - pos.v, _params.a_max/CYCLES_PER_SEC);
    }
    else {
        v = pos.v + MAX(_v - pos.v, -_params.a_max/CYCLES_PER_SEC);
    }

    omega = kakitani_control(pos.y,pos.theta,v,_r);
    tw_set_v_omega(v, omega);
}

//! go_stopにおける速度、角速度制御
static void control_go_stop(void)
{
    position_t pos;
    int32_t dest_v, v, omega;

    pos = get_map_pos(_params.submap);

    // 目標現在速度を計算
    if( pos.x < 0 ){
        // 加速度_aで減速するとき、今の位置での限界目標速度を求める
        dest_v = i_sqrt(2*_a*(pos.x/UNIT_MM))*UNIT_MM;
        if(dest_v > _v){
            dest_v = _v;
        }
    }else{
        dest_v = 0;
    }

    // 加速度制限
    if (dest_v - pos.v > 0){
        v = pos.v + MIN(dest_v - pos.v, _params.a_max/CYCLES_PER_SEC);
    }
    else {
        v = pos.v + MAX(dest_v - pos.v, -_params.a_max/CYCLES_PER_SEC);
    }

    omega = kakitani_control(pos.y,pos.theta,v,_r);
    tw_set_v_omega(v, omega);
}

//! turnにおける速度、角速度制御
static void control_turn(void)
{
    position_t pos;
    int32_t dest_omega, omega;

    pos = get_map_pos(_params.submap);

    // 目標現在角速度を計算
    if(pos.theta <= 0){
        dest_omega = i_sqrt(-2*_params.turn_a*pos.theta)*100;
        // 角速度制限
        if(dest_omega > _params.max_omega){
            dest_omega = _params.max_omega;
        }
    }else{
        dest_omega = -i_sqrt(2*_params.turn_a*pos.theta)*100;
        // 角速度制限
        if(dest_omega < -_params.max_omega){
            dest_omega = -_params.max_omega;
        }
    }

    // 角加速度制限
    if (dest_omega - pos.omega > 0){
        omega = pos.omega + MIN(dest_omega - pos.omega, _params.alpha_max*(UNIT_RAD/CYCLES_PER_SEC));
    }
    else {
        omega = pos.omega + MAX(dest_omega - pos.omega, -_params.alpha_max*(UNIT_RAD/CYCLES_PER_SEC));
    }

    tw_set_v_omega(0, omega);
}

//! go_back_stopにおける速度、角速度制御
static void control_go_back_stop(void)
{
    position_t pos;
    int32_t dest_v, v, omega;

    pos = get_map_pos(_params.submap);

    // 目標現在速度を計算
    if( pos.x > 0 ){
        // 加速度_aで減速するとき、今の位置での目標速度を求める
        dest_v = -i_sqrt(2*_a*(pos.x/UNIT_MM))*UNIT_MM;
        if(dest_v < _v){
            dest_v = _v;
        }
    }else{
        dest_v = 0;
    }

    // 加速度制限
    if (dest_v - pos.v > 0){
        v = pos.v + MIN(dest_v - pos.v, _params.a_max/CYCLES_PER_SEC);
    }
    else {
        v = pos.v + MAX(dest_v - pos.v, -_params.a_max/CYCLES_PER_SEC);
    }

    omega = kakitani_control(pos.y,pos.theta,v,_r);
    tw_set_v_omega(v, omega);
}

//! go_acceleにおける速度、角速度制御
static void control_go_accele(void)
{
    position_t pos;  
    int32_t v, // 採用する目標速度
    temp_double_dest_v; // 目標速度の二乗値

    pos = get_map_pos(_params.submap);

    int32_t mx = pos.x/UNIT_MM, my = pos.y/UNIT_MM;
	  		  	  
    if(pos.x < 0){
        // 加速度_aで減速して目標速度に達するとき、今の位置での限界目標速度を求める
        temp_double_dest_v = -2*_a*i_sqrt(mx*mx+my*my) + _term_v; // _aで減速して終端速度sqrt(_term_v)になるための限界速度
        if(temp_double_dest_v <= 0){
            v = _v; // 例外処理
        }
        else{
            v = i_sqrt(temp_double_dest_v)*UNIT_MM;
        }

        // 最初に指定した目標速度が限界より小さければ最初の目標速度を採用
        if(v > _v && _a < 0){
            v = _v;
        }
        else if(v < _v && _a > 0){
            v = _v;
        }
    }
    else{
        v = 0;
    }

    // 加速度制限
    if (_v - pos.v > 0){
        v = pos.v + MIN(_v - pos.v, _params.a_max/CYCLES_PER_SEC);
    }
    else {
        v = pos.v + MAX(_v - pos.v, -_params.a_max/CYCLES_PER_SEC);
    }

    tw_set_v_omega(v, kakitani_control(pos.y,pos.theta,v,_r));
}

//! go_constにおける速度、角速度制御
static void control_go_const(void)
{
    position_t pos;  

    pos = get_map_pos(_params.submap);

    int16_t omega;

    omega = kakitani_control(pos.y, pos.theta, pos.v, _r);
    tw_set_v_omega(0, omega);	// 試験的にDutyを設定
    omega = dv_get_signed_duty(get_tw_right())*2;	// 設定したDutyを取り出す
    if(omega > 0){
        dv_set_motor(get_tw_left(), FORWARD, _v - omega);
        dv_set_motor(get_tw_right(), FORWARD, _v);
    }
    else{
        dv_set_motor(get_tw_left(), FORWARD, _v);
        dv_set_motor(get_tw_right(), FORWARD, _v + omega);
    }
}

/***************************************************************************************************/
// block関数

//! go_curveの終了判定
static void block_go_curve(void)
{
    position_t pos;
    pos = get_map_pos(_params.submap);

    if( pos.x > -_params.stop_go_x*UNIT_MM && ABS(pos.y) < _params.stop_go_y*UNIT_MM && ABS(pos.theta) < _params.stop_go_theta ){
        remove_function(block_go_curve);
        set_drive_function(drive_stop);
    }
}

//! turnの終了判定
static void block_turn(void)
{
    position_t pos;
    pos = get_map_pos(_params.submap);

    if(ABS(pos.theta) < _params.stop_turn_theta && ABS(pos.omega) < _params.stop_turn_theta ){
        remove_function(block_turn);
        set_drive_function(drive_stop);
    }
}

//! go_back_curveの終了判定
static void block_go_back_curve(void)
{
    position_t pos;
    pos = get_map_pos(_params.submap);

    if( pos.x < _params.stop_go_x*UNIT_MM && ABS(pos.y) < _params.stop_go_y*UNIT_MM && ABS(pos.theta) < _params.stop_go_theta ){
        remove_function(block_go_back_curve);
        set_drive_function(drive_stop);
    }
}

//! turn_stay用block関数
/*!
  終了はしない。マシンの状態に応じてモードを切り替える
 */
static void block_turn_stay(void)
{
    position_t pos;
    pos = get_map_pos(_params.submap);
    if(ABS(pos.theta) < 1000 && ABS(pos.omega) < 500){
        set_drive_function(drive_stop); // 角度が所定の範囲内なら止まる
    }else{
        set_drive_function(control_turn); // それ以外なら回る
    }
}

//! go_acceleの終了判定
static void block_go_accele(void)
{
    position_t pos;
    pos = get_map_pos(_params.submap);

    if(pos.x > 0){
        remove_function(block_go_accele);
        set_drive_function(drive_stop); // 安全装置
    }
}

//! go_constの終了判定
static void block_go_const(void)
{
    if(t_s + t_c > get_time()){
        remove_function(block_go_const);
        set_drive_function(drive_stop);
    }
}

#endif
