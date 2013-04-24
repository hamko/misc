 /*!
  \file    odometry.c
  \ingroup map
  \author  Murooka Masaki <m117120@yahoo.co.jp>
  \date    2010/07/15

  \brief   オドメトリによるマップ生成
 */

#include "install_option.h"
#ifdef __INSTALL_MAP__

#define USE_PARTICLE_ODOMETRY
#define USE_DETERMIN_MODE

#include <stdio.h>
#include <math.h>
#include "odometry.h"
#include "function_list.h"
#include "i_math.h"
#include "labels.h"
#include "unit_system.h"
#include "pf_sys.h"
#include "pf_sys_util.h"
#include "dv_linesensor.h"

static map_t root = {0,0,0};   //!< ルートマップ
static map_t map = {0,0,0};    //!< マップ
static map_t submap = {0,0,0}; //!< サブマップ
static odometry_t p;            //!< パラメータ

static int32_t old_theta = 0; //!< 一周期前の角度。set_root_map_posでの変更を反映するためのもの。

static void renew_odometry(void);

#ifdef USE_PARTICLE_ODOMETRY
// pdat[mod 0] = x [mm/UNIT_MM]
// pdat[mod 1] = y [mm/UNIT_MM]
// pdat[mod 2] = theta [mm/UNIT_MM]
static pfsys_t pfsys;
static float drot1, dtrans, drot2;
static int32_t dx_glob, dy_glob, dtheta_glob;

// ラインからmだけ離れた場所の、分散vのガウス型確率を返す
#define DIV_ROOT_TWO_PI 0.39894228f
static float get_gauss(float m, float v)
{
    return DIV_ROOT_TWO_PI * exp(-m*m/2/v/v) / v;
}

//! ライン格子の状態([mm/UNIT_MM])
#define LINE_SIDE_LENGTH (500*UNIT_MM)
#define LINE_OFFSET_X (0*UNIT_MM)
#define LINE_OFFSET_Y (0*UNIT_MM)

//! 最近ラインへの距離([mm/UNIT_MM])を求める
//! 暫定的に以下のことを前提とする
//!  ラインは500mm*500mmの格子状であり, ルートマップ原点は格子の交点に一致する
static float dist_to_nearest_line(float* pdat, int index)
{   
    position_t pos_sensor, pos_machine;
    int32_t dist_x, dist_y; //Y(X)軸に平行な直線までの距離
    //pos_machineはルートマップ上でのマシンの中心座標
    pos_machine.x = (int32_t)pdat[0];
    pos_machine.y = (int32_t)pdat[1];
    pos_machine.theta = (int32_t)pdat[2];
    pos_sensor = get_lslpackbit_coord_avg_in_pos(p.lsl_pack,pos_machine,index);
    //マップ原点にライン交点がくるようにする
    pos_sensor.x -= LINE_OFFSET_X;
    pos_sensor.y -= LINE_OFFSET_Y;
    //一辺の長さで割った剰余を求める
    dist_x = ABS(pos_sensor.x) % LINE_SIDE_LENGTH;
    dist_x = MIN(dist_x, LINE_SIDE_LENGTH-dist_x);
    dist_y = ABS(pos_sensor.y) % LINE_SIDE_LENGTH;
    dist_y = MIN(dist_y, LINE_SIDE_LENGTH-dist_y);
    //短い方が最小距離
    return (float)MIN(dist_x, dist_y);
}

static float particle_odometry_likelyhood_func(float* pdat, float* mdat)
{
    /*
    int i;
    float prob = 1.0f;
    for (i = 0; i < PHOTOSENSOR_NUM; i++) {
        // dist_to_nearest_line: i番目の素子の最近ラインを求める
        int singleprob = get_gauss(dist_to_nearest_line(pdat, i), HALF_LINE_WIDTH);
        prob *= (mdat[i] ? singleprob : 1 - singleprob);
    }
    return prob;
    */
    return 1;
}

// 確率的な発展を記述
static void particle_odometry_predict_func(float* pdat)
{
    /*
    printf("P_PDAT %f %f %f\n", pdat[0], pdat[1], pdat[2]);
    */
    /*
    float drot1_samp = drot1 + box_muller(0, p.var_coeff[0] * drot1 * drot1 / UNIT_RAD / UNIT_RAD + (float)p.var_coeff[1] * dtrans * dtrans / UNIT_MM / UNIT_MM);
    float drot2_samp = drot2 + box_muller(0, p.var_coeff[0] * drot2 * drot2 / UNIT_RAD / UNIT_RAD + (float)p.var_coeff[1] * dtrans * dtrans / UNIT_MM / UNIT_MM);
    float dtrans_samp = dtrans + box_muller(0, p.var_coeff[2] * dtrans * dtrans / UNIT_MM / UNIT_MM + p.var_coeff[3] * (drot1 * drot1 + drot2 * drot2) / UNIT_RAD / UNIT_RAD);
    */
    float drot1_samp = drot1;
    float drot2_samp = drot2;
    float dtrans_samp = dtrans;

    pdat[0] += dtrans_samp * cos(pdat[2] + drot1_samp); // TODO cosの中本当に？
    pdat[1] += dtrans_samp * sin(pdat[2] + drot1_samp);
    pdat[2] += (drot1_samp + drot2_samp);
    printf("#pdat %f %f %f\n", pdat[0], pdat[1], pdat[2]);
    /*
    printf("DXY_GLOB %d %d %d\n", dx_glob, dy_glob, dtheta_glob);
    printf("DXY %f %f %f\n", dtrans_samp * cos(pdat[2] + drot1_samp), dtrans_samp * sin(pdat[2] + drot1_samp), drot1_samp + drot2_samp);
    printf("P_PDATAFT %f %f %f\n", pdat[0], pdat[1], pdat[2]);
    */
}

static void particle_odometry_init_func(pfsys_t* _pfsys)
{
    int i;
    // rootマップを見て、x, y, thetaに関してガウスに足す
    for (i = 0; i < PARTICLE_NUM; i++) {
#ifdef USE_DETERMIN_MODE
#else
        _pfsys->particle[i*3+0] += randf_pfsys() * PARTICLE_INIT_X_RANGE;
        _pfsys->particle[i*3+1] += randf_pfsys() * PARTICLE_INIT_Y_RANGE;
        _pfsys->particle[i*3+2] += randf_pfsys() * PARTICLE_INIT_THETA_RANGE;
#endif
        printf("#init %f %f %f\n", _pfsys->particle[i*3+0], _pfsys->particle[i*3+1], _pfsys->particle[i*3+2]);
    }
}

#endif

//! オドメトリの初期化
void init_odometry(odometry_t params)
{
    set_function(renew_odometry, LABEL_MAP);
    p = params;
    p.var_coeff[0] = 0.0001;
    p.var_coeff[1] = 0.0001;
    p.var_coeff[2] = 0.0001;
    p.var_coeff[3] = 0.0001;

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

    // dat[0]: x, dat[1]: y, dat[2]: theta
#ifdef USE_PARTICLE_ODOMETRY
    init_pfsys(&pfsys, PARTICLE_NUM, 3, particle_odometry_init_func, particle_odometry_likelyhood_func, particle_odometry_predict_func);
#endif
}

#ifdef USE_PARTICLE_ODOMETRY
static void copy_lsdat_to_mdat(float* mdat)
{
    int i; 
    for (i = 0; i < PHOTOSENSOR_NUM; i++) 
        mdat[i] = dv_is_linesensor_in_package(p.lsl_pack, i);

    printf("PHOTO ");
    for (i = 0; i < PHOTOSENSOR_NUM; i++) 
        printf("%d", mdat[i] ? 1 : 0);
    printf("\n");
}

static void renew_odometry(void)
{
    position_t pos;

    int32_t left_encoder_total, right_encoder_total; // エンコーダの積算値
    static int32_t old_left_encoder_total, old_right_encoder_total; // 1周期前のエンコーダの積算値
    int32_t d_left_encoder, d_right_encoder; // エンコーダの増分
    int32_t dx, dy, ds; // x,yと道のりの変位(0.0001mm, 0.0001mm)
    int32_t dtheta; // 角度の変位(0.0001rad)

    /* drot1, dtrans, drot2 の計算 */
    // エンコーダの積算値を取得
    left_encoder_total = dv_get_encoder_total(p.left_encoder);
    right_encoder_total = dv_get_encoder_total(p.right_encoder);

    // エンコーダの増分を求める
    d_left_encoder = left_encoder_total - old_left_encoder_total;
    d_right_encoder = right_encoder_total - old_right_encoder_total;

    // 角度変化を求める。エンコーダの精度はencoder_width(0.1mm)で直す
    dtheta = multiply_divide(d_right_encoder, p.step_right, p.encoder_width)
         - multiply_divide(d_left_encoder, p.step_left, p.encoder_width);

    // 道のりの変位を求める (エンコーダの精度はマップの10倍なので、ここで直す)
    ds = (d_left_encoder * p.step_left + d_right_encoder * p.step_right) / 20;

    // 以上の値からdx, dyを計算
    dx = ds * i_cos(/*pos.theta + */dtheta/2) / UNIT_MM;
    dy = ds * i_sin(/*pos.theta + */dtheta/2) / UNIT_MM;

    printf("DXY_ORG %d %d %d\n", dx, dy, dtheta);
    dx_glob = dx, dy_glob = dy, dtheta_glob = dtheta;

    // (dx, dy, dtheta) -> (drot1, dtrans, drot2)の変換
    if (!dx)
        drot1 = dy > 0 ? pi / 2 : -pi / 2;
    else 
        drot1 = atan2(dy, dx) * UNIT_RAD; // [rad / UNIT_RAD]
    dtrans = sqrt((float)((int64_t)dx * dx + (int64_t)dy * dy)); // [mm / UNIT_MM]
    drot2 = (float)dtheta - drot1; // [rad / UNIT_RAD]
    if (drot2 < -pi)
        drot2 += 2 * pi;
    if (drot2 > +pi)
        drot2 -= 2 * pi;

    pos.v = ds * CYCLES_PER_SEC;
    pos.omega = dtheta * CYCLES_PER_SEC;

    old_left_encoder_total = left_encoder_total;
    old_right_encoder_total = right_encoder_total;

    /* パーティクルフィルタの更新 */
    float mdat[PHOTOSENSOR_NUM];
    // TODO ラインセンサ情報のコピー
    copy_lsdat_to_mdat(mdat);
    revise_pfsys(&pfsys, mdat);

    /* パーティクルフィルタの粒子の平均を代入 */
    float nmax_state[3];
    printf("\n");
    get_particle_nmax_pfsys(5, nmax_state, &pfsys);
    pos.x = nmax_state[0]; pos.y = nmax_state[1]; pos.theta = nmax_state[2];
    printf("#rottrans %f %f %f\n", drot2, drot1, dtrans);
    printf("#map %d %d %d\n", pos.x, pos.y, pos.theta);
    
    set_root_map_pos(&root, pos);
}
#else 
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
#endif

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
