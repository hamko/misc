/*!
 \file    mlm.c
 \ingroup mlm
 \author  Iori <ik0313@gmail.com>
 \date    2010/08/12
 
 \brief   mlmによるマップ補正を行うための関数群
 
 mlm = maximum likelihood method(最尤推定法)
 
 */

#include "install_option.h"
#ifdef __INSTALL_MLM__

#include "mlm.h"
#include "function_list.h"
#include "labels.h"
// SIM
#include <stdio.h>


// for debug
#define _MLM_DEBUG
//#undef _MLM_DEBUG

// 定数群
#define INT64_EMERGENCY ((int64_t)0x3FFFFFFFFFFFFFFFll) //!< int64_tが桁溢れする危険水域
#define SHIFT_DIFF (_shift - _shift_d) //!< (x, y)用シフトと(xd, yd)用シフトの差
// 補正を許可するマップ偏差の絶対値
#define MAX_CORRECT_DX ((int64_t)2500000) // x(250mm)
#define MAX_CORRECT_DY ((int64_t)2500000) // y(250mm)
#define MAX_CORRECT_DTHETA ((int64_t)75854) // theta(pi/4)

// mlmで使用するフィールドパラメータ
// init_mlm()関数で初期化
static p_map_t _original_map; //!< mlmの補正対象となる元マップ
static map_t _mlm_map_substance; //!< mlm用サンプリングマップの実体
static p_map_t _mlm_map = &_mlm_map_substance; //!< mlmのサンプリングを行うためのマップへのポインタ。原点は格子点上
static p_lsl_package_t _mlm_sources; //!< mlmに使うラインセンサリスト群
static int64_t _line_interval; //!< ライン間隔。デフォルトのサンプル-ライン最短距離導出関数を使用する場合に指定(単位 1/UINT_MM mm)
static int64_t _v_threshold; //!< サンプル取得を許す最低速度(単位 1/UNIT_MM mm) 
static int64_t _executable_sample_num; //!< 補正を実行できる最低サンプル数
static int64_t _exec_sample_increment; //!< 補正閾値の一回あたり増分
static int64_t _max_exec_sample_num; //!< 補正閾値の最大値
static int16_t _shift; //!< サンプル値の桁溢れを防ぐ為のシフト数(x, y用)
static int16_t _shift_d; //!< サンプル値の桁溢れを防ぐ為のシフト数(xd, yd用)
//! サンプルの(x, y)から(xd, yd)を導出するサンプル-ライン間距離導出関数へのポインタ
static int16_t (*_calc_line_to_sample)(int64_t x, int64_t y, int64_t *xd, int64_t *yd);


//! mlm実行フラグ
static volatile BOOLEAN _execute_mlm_flag = FALSE;

// 補正に使用する更新値
static volatile int64_t _sum_x = 0; //!< 反応点x座標の総和(平行移動済み)
static volatile int64_t _sum_y = 0; //!< 反応点y座標の総和(平行移動済み)
static volatile int64_t _sum_xd = 0; //!< 最も近いy軸に平行なラインからの変位の総和
static volatile int64_t _sum_yd = 0; //!< 最も近いx軸に平行なラインからの変位の総和
static volatile int64_t _sum_x2 = 0; //!< 平行移動した反応点x座標の二乗の総和
static volatile int64_t _sum_y2 = 0; //!< 平行移動した反応点y座標の二乗の総和
static volatile int64_t _sum_xd2 = 0; //!< 最も近いy軸に平行なラインからの変位の二乗の総和
static volatile int64_t _sum_yd2 = 0; //!< 最も近いx軸に平行なラインからの変位の二乗の総和
static volatile int64_t _sum_xyd = 0; //!< 平行移動したx * ydの総和
static volatile int64_t _sum_xdy = 0; //!< xd * 平行移動したyの総和
static volatile int16_t _x_sample_num = 0; //!< x軸方向のラインに近いサンプル数
static volatile int16_t _y_sample_num = 0; //!< y軸方向のラインに近いサンプル数

static volatile int64_t _x0 = 0; //!< x平行移動量
static volatile int64_t _y0 = 0; //!< y平行移動量

// 内部関数のプロトタイプ
//! 割り込み関数からmlmを実行する関数。function_listから呼ばれるmlmの実体
void control_mlm(void);
//! 絶対値シフト関数。shift>0なら左、shift<0なら右にシフトする
static int64_t abs_shift(int64_t target, int16_t shift);
//! mlmの更新値を初期化
static void reset_mlm(void);
//! ラインセンサリストについてサンプリング可能かを判別
static BOOLEAN is_sampling_possible(const p_ls_list_t p_lsl, int16_t bit);
//! サンプルの取得と計算
static void calc_sample(const p_ls_list_t p_lsl, int16_t bit);
//! 蓄積したサンプルから実際のフィールドとマップのずれを計算
static position_t calc_difference(void);
//! フィールドからのずれを受け取り、マップに反映
static void correct_map(p_map_t original_map, p_map_t mlm_map, const position_t *diff);
//! mlmを実行する
static void execute_mlm(void);
//! デフォルトのサンプル-ライン間距離導出関数。line_intervalで(x, y)を丸めた余りを求める。返り値は常に0
static int16_t default_calc_line_to_sample(int64_t x, int64_t y, int64_t *xd, int64_t *yd);

// 制御関数
void init_mlm_with(p_map_t original_map, p_lsl_package_t mlm_sources,
                   int16_t (*calc_line_to_sample)(int64_t x, int64_t y, int64_t *xd, int64_t *yd),
                   int64_t v_threshold,
                   int64_t init_exec_sample_num, int64_t exec_sample_increment, int64_t max_exec_sample_num,
                   int16_t shift, int16_t shift_d,
                   int64_t x_offset, int64_t y_offset, int64_t theta_offset)
{
    // 元マップに対して-offset分だけずらしたマップ(格子点上に原点がある)をサンプリング対象に指定
    // サンプリング対象のマップは元のマップを親としてマップツリーに登録
    _original_map = original_map;
    _mlm_map->x = -x_offset;
    _mlm_map->y = -y_offset;
    _mlm_map->theta = -theta_offset;
    add_map(_mlm_map, _original_map);
    
    _mlm_sources = mlm_sources;
    _calc_line_to_sample = calc_line_to_sample;
    _v_threshold = v_threshold * UNIT_MM;
    _executable_sample_num = init_exec_sample_num;
    _exec_sample_increment = exec_sample_increment;
    _max_exec_sample_num = max_exec_sample_num;
    _shift = shift;
    _shift_d = shift_d;
    if(!search_function(control_mlm)){
        set_function(control_mlm, LABEL_USER_PRE);
    }
    reset_mlm();
	
}
void init_mlm(p_map_t original_map, p_lsl_package_t mlm_sources, int64_t line_interval, int64_t v_threshold,
              int64_t init_exec_sample_num, int64_t exec_sample_increment, int64_t max_exec_sample_num,
              int16_t shift, int16_t shift_d,
              int64_t x_offset, int64_t y_offset, int64_t theta_offset)
{
    // default_calc_line_to_sample用
    _line_interval = line_interval * UNIT_MM;
    // サンプル-ライン最短距離導出関数としてdefault_calc_line_to_sampleを使用する
    init_mlm_with(original_map, mlm_sources,
                  default_calc_line_to_sample, v_threshold,
                  init_exec_sample_num, exec_sample_increment, max_exec_sample_num,
                  shift, shift_d,
                  x_offset, y_offset, theta_offset);
    return;
}

void set_execute_mlm_flag(BOOLEAN flag)
{
    _execute_mlm_flag = flag;
    return;
}

// 内部関数
void control_mlm(void){
    switch(_execute_mlm_flag){
		case TRUE:
			// mlmを実行
			execute_mlm();
			break;
		case FALSE:
		default:
			// mlmしない場合はサンプルをリセット
			reset_mlm();
			break;
    }
    return;
}

static int64_t abs_shift(int64_t target, int16_t shift)
{
	// shiftが正であれば絶対値分だけ左シフトを行う
	if(shift > 0){
		if(target < 0){
			// 負の値はtargetの絶対値をシフトしてマイナスをつける
			target = ABS(target) << shift;
			return -target;
		}
		else
			// 正の値はそのままシフト
			return target << shift;
	}
	// shiftが負であれば絶対値分だけ右シフトを行う
	else{
		if(target < 0){
			//負の値はtargetの絶対値をシフトしてマイナスをつける
			target = ABS(target) >> (-shift);
			return -target;
		}
		else{
			// 正の値はそのままシフト
			return target >> (-shift);
		}
	}
}

static void reset_mlm(void){
    // 更新値を初期化
    _sum_x = 0;
    _sum_y = 0;
    _sum_xd = 0;
    _sum_yd = 0;
    _sum_x2 = 0;
    _sum_y2 = 0;
    _sum_xd2 = 0;
    _sum_yd2 = 0;
    _sum_xyd = 0;
    _sum_xdy = 0;
    _x_sample_num = 0;
    _y_sample_num = 0;
    _x0 = 0;
    _y0 = 0;
}

static BOOLEAN is_sampling_possible(const p_ls_list_t p_lsl, int16_t bit){
    if (!dv_is_linesensor_list(p_lsl, bit)) {
#ifdef _MLM_DEBUG
        printf("# not on a line #\n");
#endif
        return FALSE;
    } 
    position_t pos = get_map_pos(_mlm_map);
	
    // ラインセンサエラーがあったら不可
    if(p_lsl->error == TRUE){
#ifdef _MLM_DEBUG
        printf("# lsl error #\n");
#endif
        return FALSE;
    }
	
    // 速度が指定値以下だとサンプル値が密集するので不可
    if(ABS(pos.v) < ABS(_v_threshold)){
#ifdef _MLM_DEBUG        
        printf("# too slow #\n");
#endif
        return FALSE;
    }
	
    // 桁溢れチェック。桁が溢れ危険域に入っていたら強制初期化
    // 最初に桁溢れするのは間違いなくx2かy2かxd2かyd2
    if((_sum_x2 > INT64_EMERGENCY) || (_sum_y2 > INT64_EMERGENCY)
       || (_sum_x2 < 0) || (_sum_y2 < 0)){
#ifdef _MLM_DEBUG
        printf("#sum_x2 or sum_y2 in mlm overflow#\n");
#endif
        reset_mlm();
    }
    if((_sum_xd2 > INT64_EMERGENCY) || (_sum_yd2 > INT64_EMERGENCY)
       || (_sum_xd2 < 0) || (_sum_yd2 < 0)){
#ifdef _MLM_DEBUG
        printf("#sum_xd2 or sum_yd2 in mlm overflow#\n");
#endif
        reset_mlm();
    }
	
    return TRUE;
}

static void calc_sample(const p_ls_list_t p_lsl, int16_t bit)
{
    position_t lsl_pos = get_lslbit_coord_avg_in_map(p_lsl, _mlm_map, bit); // サンプリングマップ上でのライン座標
    int64_t tmp_xd, tmp_yd; // 直近ラインからの変位の一時保存変数
    int64_t x_trans, y_trans; // 取得したサンプルを_x0, _y0だけ平行移動した値
    int64_t shift_x, shift_y, shift_xd, shift_yd; // シフトした後のサンプル
	
	
    // まずサンプリング可能かをチェックし、不可なら即返る
    if(is_sampling_possible(p_lsl, bit) == FALSE){
        return;
    }
	
    // 直近のx, y方向に平行なラインからサンプルまでの距離を求める
    /* default                                                  */
    /* tmp_xd = lsl_pos.x - i_round(lsl_pos.x, _line_interval); */
    /* tmp_yd = lsl_pos.y - i_round(lsl_pos.y, _line_interval); */
    if((*_calc_line_to_sample)(lsl_pos.x, lsl_pos.y, &tmp_xd, &tmp_yd)){
        // _calc_line_to_sampleが不正な値(0以外)を吐いた場合はサンプルを計算せずに捨てる
        return;
    }
    
#ifdef _MLM_DEBUG
    printf("sample_val: %d %d %lld %lld\n", lsl_pos.x, lsl_pos.y, tmp_xd, tmp_yd);
#endif
	
    if((!_x_sample_num) && (!_y_sample_num)){
        // サンプリング開始時に平行移動量を決定
        // 平行移動量は_sum_x2, _sum_y2を小さくするためサンプリング開始点とする
        _x0 = lsl_pos.x;
        _y0 = lsl_pos.y;
    }
    
    // 取得サンプルを平行移動, 以下この平行移動値を使用
    x_trans = lsl_pos.x - _x0;
    y_trans = lsl_pos.y - _y0;
	
    // 反応点がx方向とy方向のラインのどちらに近いかを判定
    if(ABS(tmp_xd) > ABS(tmp_yd)){
        // x軸方向のラインに近い時
		
        // 桁溢れ防止のシフト
        shift_x = abs_shift(x_trans, -_shift);
        shift_yd = abs_shift(tmp_yd, -_shift_d);
		
        // サンプル値の更新
        _sum_x += shift_x;
        _sum_yd += shift_yd;
        _sum_x2 += shift_x * shift_x;
        _sum_yd2 += shift_yd * shift_yd;
        _sum_xyd += shift_x * shift_yd;
        _x_sample_num++;
    }else{
        // y方向のラインに近い時
		
        // 桁溢れ防止のシフト
        shift_y = abs_shift(y_trans, -_shift);
        shift_xd = abs_shift(tmp_xd, -_shift_d);
        // サンプル値の更新
        _sum_y += shift_y;
        _sum_xd += shift_xd;
        _sum_y2 += shift_y * shift_y;
        _sum_xd2 += shift_xd * shift_xd;
        _sum_xdy += shift_xd * shift_y;
        _y_sample_num++;
    }
	
#ifdef _MLM_DEBUG
    printf("sum2: %lld %lld %lld %lld\n", _sum_x2, _sum_y2, _sum_xd2, _sum_yd2);
#endif 
    
    return;
}

static position_t calc_difference(void)
{
    position_t diff = {0, 0, 0, 0, 0}; // マップの誤差
    int64_t avg_x, avg_y, avg_xd, avg_yd; // x, y, xd, ydの平均
    int64_t dt_denom, dt_numer; // 角度誤差の分母・分子
	
    // 平均値を計算
    if(_x_sample_num > 0){
        avg_x = _sum_x / _x_sample_num;
        avg_yd = _sum_yd / _x_sample_num;
    }else{
        avg_x = avg_yd = 0;
    }
    if(_y_sample_num > 0){
        avg_y = _sum_y / _y_sample_num;
        avg_xd = _sum_xd / _y_sample_num;
    }else{
        avg_y = avg_xd = 0;
    }
	
    // 角度誤差を計算。シフトは(x, y)のシフト量に合わせる
    dt_denom = -(_sum_y * avg_y) - (abs_shift(_sum_xd2, -2 * SHIFT_DIFF) - _sum_y2)
	- (_sum_x * avg_x) - (-_sum_x2 + abs_shift(_sum_yd2, -2 * SHIFT_DIFF));
    dt_numer = -(_sum_y * abs_shift(avg_xd, -SHIFT_DIFF)) + abs_shift(_sum_xdy, -SHIFT_DIFF)
	+ (_sum_x * abs_shift(avg_yd, -SHIFT_DIFF)) - abs_shift(_sum_xyd, -SHIFT_DIFF);
    if(!dt_denom){
        // 分母0の場合はエラーとして補正項はall 0で返す
#ifdef _MLM_DEBUG
        printf("# denom is 0 #\n");
#endif
        return diff;
    }else{
        diff.theta = multiply_divide64(dt_numer, UNIT_RAD, dt_denom); // シフト量は分母・分子ともに2 * _shiftで相殺
        diff.x = abs_shift(-avg_xd, _shift_d) + multiply_divide64(diff.theta, abs_shift(avg_y, _shift) + _y0, UNIT_RAD); // シフトを元に戻す
        diff.y = -multiply_divide64(diff.theta, abs_shift(avg_x, _shift) + _x0, UNIT_RAD) - abs_shift(avg_yd, _shift_d); // シフトを元に戻す
        return diff;
    }
}

static void correct_map(p_map_t original_map, p_map_t mlm_map, const position_t *diff)
{
    // diffはサンプリング対象のマップ系のフィールドに対する変位
    // これを元にoriginal_mapを補正する
	
    // diffの値が補正許容値か検査、引っかかったら補正はしない
    if(ABS(diff->x) > MAX_CORRECT_DX){
#ifdef _MLM_DEBUG
        printf("# diff x = %d is too large #\n", ABS(diff->x));
#endif
    }else if(ABS(diff->y) > MAX_CORRECT_DY){
#ifdef _MLM_DEBUG
        printf("# diff y = %d is too large #\n", ABS(diff->y));
#endif
    }else if(ABS(diff->theta) > MAX_CORRECT_DTHETA){
#ifdef _MLM_DEBUG
        printf("# diff theta is too large #\n");
#endif
    }else{
#ifdef _MLM_DEBUG
        printf("correct: %d %d %d\n", diff->x, diff->y, diff->theta);
#endif
        // まずoriginal_mapの原点を合わせる
        // 平行移動量はmlm_map表記
        transfer_map(original_map, mlm_map, -(diff->x), -(diff->y));
        // 次に角度を合わせるためにoriginal_mapの原点周りに回転する
        // 回転角度はoriginal_mapもmlm_mapも同じ
        rotate_map(original_map, original_map, 0, 0, -(diff->theta));
    }
    // 最後に更新値を初期化
    reset_mlm();
    return;
}

static void execute_mlm(void)
{
    int16_t i, j;
    position_t diff; // フィールドとマップの誤差
    p_ls_list_t target_lsl_p; // サンプル対象のラインセンサリスト
	
#ifdef _MLM_DEBUG
    printf("sample_num: %d %d\n",_x_sample_num,_y_sample_num); //for debug
#endif
	
    // サンプリング
    for(i = 0; i < _mlm_sources->num; i++){
        target_lsl_p = *(_mlm_sources->device + i);
        for (j = 0; j < get_sensor_num_in_lsl(target_lsl_p); j++) {
            calc_sample(target_lsl_p, j);
        }
    }
	
    // サンプル数が閾値を超えたら補正開始
    if(_x_sample_num + _y_sample_num > _executable_sample_num){
        diff = calc_difference();
        correct_map(_original_map, _mlm_map, &diff); // _mlm_mapは_original_mapの子マップなので_original_mapを変えれば更新される
        if(_executable_sample_num < _max_exec_sample_num){
            // 補正一回ごとにサンプル数閾値を指定分増加
            _executable_sample_num += _exec_sample_increment;
        }
        if(_executable_sample_num > _max_exec_sample_num){
            // 補正閾値最大値で抑える
            _executable_sample_num = _max_exec_sample_num;
        }
    }
    return;
}

static int16_t default_calc_line_to_sample(int64_t x, int64_t y, int64_t *xd, int64_t *yd)
{
    // x, y座標を_line_intervalで丸めた余りとして直近のラインからの変位を導出
    *xd = x - i_round(x, _line_interval);
    *yd = y - i_round(y, _line_interval);
    return 0;
}

#endif
