/*!
  \file    drive_tw.c
  \ingroup drive_tw
  \author  Sota <jine0125@gmail.com>
  \date    2011/02/22

  \brief   二輪マシン用駆動関数
  \todo    旧ライブラリ形式の制御器の実装

  v, omegaをDUTYに変換し、モーターを駆動する\n
  initを呼んだ制御器を用いてtw_set_v_omegaで駆動する。
  例えばgo_curveからは、計算したdest_v, dest_omegaがtw_set_v_omegaに渡される
 */

#include "install_option.h"
#ifdef __INSTALL_DRIVE_TW__

#include "dv_motor.h"
#include "drive_tw.h"
#include "drive_stop.h"
#include "i_math.h"
#include "map.h"
#include "r_stdint.h"
#include "unit_system.h"
//#include "stream.h"

#undef NULL
#define NULL ((void *)0)    //!< ヌルポインタ
#define OVER_NUM 0x3FFFFFFF //オーバーフローとする数

//#define __DEBUG_DRIVE_TW__

static void (*_controller)(int32_t v, int32_t omega) = NULL;    //!< 使用する制御器
static p_motor_t left_motor;  //!< 左タイヤ
static p_motor_t right_motor; //!< 右タイヤ
static int32_t k_a;         //!< フィードバックゲイン\@tdf
static int32_t k_v;         //!< フィードフォワードゲイン\@tdf
static int32_t k_p;         //!< pゲイン\@pid
static int32_t k_i;         //!< iゲイン\@pid
static int32_t k_d;         //!< dゲイン\@pid
static int32_t width;       //!< タイヤ間距離
static int32_t offset_l;    //!< 左タイヤのオフセットDUTY
static int32_t offset_r;    //!< 右タイヤのオフセットDUTY
static p_map_t map;         //!< 速度取得用マップ
static int32_t k_f_a;         // フィードバックゲイン\@fm
static int32_t k_f_v;         // フィードフォワードゲイン\@fm
static int32_t k_m_a;         // フィードバックゲイン\@fm
static int32_t k_m_v;         // フィードフォワードゲイン\@fm
static int32_t dt_f;
static int32_t dt_m;
static int32_t _v,_omega;
static int16_t _i_r_shift_num,_i_l_shift_num;

static void set_to_tw_tdf(int32_t v, int32_t omega);
static void set_to_tw_pid(int32_t v, int32_t omega);
static void set_to_tw_fm(int32_t v,int32_t omega);
static void set_to_tw_tdf_pi(int32_t v, int32_t omega);


//! 二輪用の制御器を設定
void set_tw_controller(void (*controller)(int32_t v, int32_t omega))
{
    /*!
     \todo
      二回目以降の登録時Warningを出力(dprintfの整備ができないとムリ)
     */

    _controller = controller;
}

//! 二輪マシンのタイヤを駆動
void tw_set_v_omega(int32_t v, int32_t omega)
{
    if (_controller == NULL){
        //log_message(stream_warn, "#! tw is NULL.\n");
        return;
    }
    (*_controller)(v, omega);
    _v = v; _omega = omega;
}


//! 二輪マシンの停止(FREE)
void tw_free(void)
{
    if (_controller == NULL){
        //log_message(stream_warn, "#! tw is NULL.\n");
        return;
    }

    dv_set_motor(right_motor, FREE, 0);
    dv_set_motor(left_motor, FREE, 0);
}

//! 二輪マシンの停止(BRAKE)
void tw_brake(void)
{
    if (_controller == NULL){
        //log_message(stream_warn, "#! tw is NULL.\n");
        return;
    }

    dv_set_motor(right_motor, BRAKE, 1000);
    dv_set_motor(left_motor, BRAKE, 1000);
}

//! 2自由度系の制御器による二輪の駆動
void init_tw_tdf(tw_tdf_t params)
{
    right_motor = params.right_motor;
    left_motor = params.left_motor;
    k_v = params.k_v;
    k_a = params.k_a;
    offset_l = params.offset_l;
    offset_r = params.offset_r;
    width = params.width;
    map = params.map;
    set_tw_controller(set_to_tw_tdf);
    if (get_drive_stop() == NULL){
        set_drive_stop(tw_free);
    }
}

p_motor_t get_tw_right(void)
{
	return right_motor;
}
p_motor_t get_tw_left(void)
{
	return left_motor;
}

// 目標速度を返す
int32_t get_dest_v(void)
{
	return _v;
}
// 目標角速度を返す
int32_t get_dest_omega(void){
	return _omega;
}

//! 2自由度系制御器
//! \todo DUTYの最大値での切り方
static void set_to_tw_tdf(int32_t v, int32_t omega)
{
    int32_t d_v_r, d_v_l;      // タイヤレベルでの目標速度
    int32_t v_r, v_l;          // 現在の推定タイヤ速度
    int right_duty, left_duty; // タイヤにかけるDuty
    position_t pos;            // 速度を取るための座標

    pos = get_map_pos(map);

    // タイヤごとの速度に分解
    // UNIT_RADを1余計に左シフトしているのは回転成分速度 = omega * width / 2だから
    d_v_r = v + multiply_divide(omega, width, UNIT_RAD<<1);
    d_v_l = v - multiply_divide(omega, width, UNIT_RAD<<1);
    v_r = pos.v + multiply_divide(pos.omega, width, UNIT_RAD<<1);
    v_l = pos.v - multiply_divide(pos.omega, width, UNIT_RAD<<1);
    #ifdef __DEBUG_DRIVE_TW__
    printf("#dest_v=%ld dest_omega=%ld\n",v,omega);
    #endif
    // それぞれのタイヤに対して二自由度制御
    // 過去のライブラリより一段浅いところでフィードバックをかけている
    right_duty = (d_v_r - v_r)/k_a + v_r / k_v;
    left_duty = (d_v_l - v_l)/k_a + v_l / k_v;

    // 摩擦分を足し込む
    right_duty += sgn(v_r)*offset_r;
    left_duty += sgn(v_l)*offset_l;

    // モーターを回す
    dv_set_motor(right_motor, FORWARD, right_duty);
    dv_set_motor(left_motor, FORWARD, left_duty);
}




//! PID制御での二輪の駆動
void init_tw_pid(tw_pid_t params)
{
    right_motor = params.right_motor;
    left_motor = params.left_motor;
    k_p = params.k_p;
    k_i = params.k_i;
    k_d = params.k_d;
    offset_l = params.offset_l;
    offset_r = params.offset_r;
    width = params.width;
    map = params.map;
    set_tw_controller(set_to_tw_pid);
    if (get_drive_stop() == NULL){
        set_drive_stop(tw_free);
    }
}

//! PID制御器
/*!
  \todo    I成分の桁溢れ
  \warning あまり真面目に実装していないので本気でPID制御器を使うつもりの人は加筆することを推奨
  \warning 1周期に二回呼ぶと変なことになる仕様。要修正
 */
static void set_to_tw_pid(int32_t v, int32_t omega)
{
    int32_t d_v_r, d_v_l;      // タイヤレベルでの目標速度
    int32_t v_r, v_l;          // 現在の推定タイヤ速度
    int right_duty, left_duty; // タイヤにかけるDuty
    position_t pos;            // 速度を取るための座標
    static int32_t p_r, p_l;    // P
    static int32_t i_r, i_l;    // I
    // ※64bitだとコンパイラ的に不具合が出るので32bitに修正されている。必要に応じて2変数用意するなどしてサイズを確保すること
    int32_t d_r, d_l;           // D

    pos = get_map_pos(map);

    // タイヤごとの速度に分解
    d_v_r = v + multiply_divide(omega, width, UNIT_RAD<<1);
    d_v_l = v - multiply_divide(omega, width, UNIT_RAD<<1);
    v_r = pos.v + multiply_divide(pos.omega, width, UNIT_RAD<<1);
    v_l = pos.v - multiply_divide(pos.omega, width, UNIT_RAD<<1);

    // それぞれのタイヤに対してPID制御
    // 過去のPをDに退避
    d_r = -p_r;
    d_l = -p_l;
    // 新規のPを取得、Dを差分値に書き換え
    d_r += p_r = d_v_r - v_r;
    d_l += p_l = d_v_l - v_l;
    // Iの足し込み
    i_r += p_r; i_l += p_l;

    right_duty = p_r/k_p + d_r/k_d + i_r/k_i;
    left_duty = p_l/k_p + d_l/k_d + i_l/k_i;

    // 摩擦分を足し込む
    right_duty += v_r > 0 ? offset_r:-offset_r;
    left_duty += v_l > 0 ? offset_l:-offset_l;

    // モーターを回す
    dv_set_motor(right_motor, FORWARD, right_duty);
    dv_set_motor(left_motor, FORWARD, left_duty);
}

// F/M制御器による二輪の駆動
void init_tw_fm(tw_fm_t params)
{
    right_motor = params.right_motor;
    left_motor = params.left_motor;
    k_f_v = params.k_f_v;
    k_f_a = params.k_f_a;
    k_m_v = params.k_m_v;
    k_m_a = params.k_m_a;
    dt_f = params.dt_f;
    dt_m = params.dt_m;
    offset_l = params.offset_l;
    offset_r = params.offset_r;
    map = params.map;
    set_tw_controller(set_to_tw_fm);
    if (get_drive_stop() == NULL){
        set_drive_stop(tw_free);
    }
}

// F/M制御器
static void set_to_tw_fm(int32_t v, int32_t omega)
{
    int left_duty, right_duty;
    int f_duty, m_duty;
    position_t pos;            // 速度を取るための座標 

    pos = get_map_pos(map);

    
    #ifdef __DEBUG_DRIVE_TW__
    printf("#dest_v=%ld dest_omega=%ld\n",v,omega);
    #endif
    f_duty = multiply_divide(v - pos.v,dt_f,k_f_a) + v / k_f_v;
    m_duty = multiply_divide(omega - pos.omega,dt_m,k_m_a) + omega / k_m_v;
    
    right_duty = f_duty + m_duty;
    left_duty = f_duty - m_duty;
    // 摩擦分を足し込む
    right_duty += sgn(right_duty)*offset_r;
    left_duty += sgn(left_duty)*offset_l;

    // モーターを回す
    dv_set_motor(right_motor, FORWARD, right_duty);
    dv_set_motor(left_motor, FORWARD, left_duty);
}

// tdf+I
void init_tw_tdf_pi(tw_tdf_pi_t params){
    right_motor = params.right_motor;
    left_motor = params.left_motor;
    k_v = params.k_v;
    k_a = params.k_a;
    k_i = params.k_i;
    offset_l = params.offset_l;
    offset_r = params.offset_r;
    width = params.width;
    map = params.map;
    _i_r_shift_num = params.default_shift_num;
    _i_l_shift_num = params.default_shift_num;
    set_tw_controller(set_to_tw_tdf_pi);
    if (get_drive_stop() == NULL){
        set_drive_stop(tw_free);
    }
}

static void set_to_tw_tdf_pi(int32_t v, int32_t omega)
{
    int32_t d_v_r, d_v_l;      // タイヤレベルでの目標速度
    int32_t v_r, v_l;          // 現在の推定タイヤ速度
    int right_duty, left_duty; // タイヤにかけるDuty
    position_t pos;            // 速度を取るための座標
    int32_t p_r,p_l;
    static int32_t i_r,i_l;     //I成分

    pos = get_map_pos(map);

    // タイヤごとの速度に分解
    // UNIT_RADを1余計に左シフトしているのは回転成分速度 = omega * width / 2だから
    d_v_r = v + multiply_divide(omega, width, UNIT_RAD<<1);
    d_v_l = v - multiply_divide(omega, width, UNIT_RAD<<1);
    v_r = pos.v + multiply_divide(pos.omega, width, UNIT_RAD<<1);
    v_l = pos.v - multiply_divide(pos.omega, width, UNIT_RAD<<1);
    #ifdef __DEBUG_DRIVE_TW__
    printf("#dest_v=%ld dest_omega=%ld\n",v,omega);
    #endif
    
    //P成分更新
    p_r = d_v_r - v_r;
    p_l = d_v_l - v_l;
    
    //I成分のオーバーフローチェック。ヤバくなったらシフトする。
    if(ABS(sgn(i_r)*(ABS(i_r)>>1) + sgn(p_r)*(ABS(p_r)>>(_i_r_shift_num+1))) >= OVER_NUM ){ _i_r_shift_num++; }
    if(sgn(i_l)*(ABS(i_l)>>1) + sgn(p_l)*(ABS(p_l)>>(_i_l_shift_num+1)) >= OVER_NUM ){ _i_l_shift_num++; }
    //I成分更新
    i_r += sgn(p_r)*(ABS(p_r)>>_i_r_shift_num);
    i_l += sgn(p_l)*(ABS(p_l)>>_i_l_shift_num);
    
    // それぞれのタイヤに対して二自由度制御
    // 過去のライブラリより一段浅いところでフィードバックをかけている
    right_duty = p_r/k_a + v_r / k_v + (sgn(i_r)*(ABS(i_r/k_i)<<_i_r_shift_num));
    left_duty = p_l/k_a + v_l / k_v + (sgn(i_l)*(ABS(i_l/k_i)<<_i_l_shift_num));

    // 摩擦分を足し込む
    right_duty += sgn(v_r)*offset_r;
    left_duty += sgn(v_l)*offset_l;

    // モーターを回す
    dv_set_motor(right_motor, FORWARD, right_duty);
    dv_set_motor(left_motor, FORWARD, left_duty);
}




#endif
