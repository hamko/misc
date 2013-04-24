/*!
  \file    dv_linesensor.c
  \ingroup device
  \author  Iori <ik0313@gmail.com>
  \date    2010/08/12

  \brief   複数のラインセンサを統一的に扱うための構造体・関数定義
  詳細説明
 */

#include "install_option.h"
#ifdef __INSTALL_DEVICE__

#include "dv_linesensor.h"
#include "unit_system.h"
#include "i_math.h"

static uint8_t _min_sensor_num_for_line = 3; //!< ラインとみなす反応点の最低連続数(デフォルト3)

void init_line_info(uint8_t min_sensor_num_for_line)
{
    _min_sensor_num_for_line = min_sensor_num_for_line;
    return;
}

uint8_t dv_is_linesensor(const p_ls_material_t p_lsm, uint8_t pos)
{
    uint8_t reference; // 実際の参照先センサ位置

    if(p_lsm->num < pos){
        // 不正な参照には0を返す
        return 0;
    }
    
    if(p_lsm->direction == NEGATIVE){
        reference = (p_lsm->num - 1) - pos;
    }else{
        reference = pos;
    }

    return is_linesensor(p_lsm->hal_ls, reference);
}

uint8_t dv_is_linesensor_list(p_ls_list_t p_lsl, uint8_t pos)
{
    int16_t lsl_index = 0; // 実際の参照ラインセンサのインデックス
    uint8_t reference = pos; // 実際の参照先センサ位置
    ls_material_t *target_lsm_p = *(p_lsl->lsl_head); // 参照ラインセンサ構造体へのポインタ
    
    // 指定の参照センサがどのラインセンサにあるか探す
    while((lsl_index < p_lsl->num)
          && (reference >= target_lsm_p->num)){
        reference -= target_lsm_p->num;
        lsl_index++;
        target_lsm_p = *(p_lsl->lsl_head + lsl_index);
    }

    if((lsl_index < p_lsl->num) && (reference < target_lsm_p->num)){
        return dv_is_linesensor(target_lsm_p, reference);
    }else{
        // 不正な値には0を返す
        return 0;
    }
}

uint8_t get_sensor_num_in_lsl(const p_ls_list_t p_lsl)
{
    int16_t i;
    uint8_t sensor_num = 0;

    for(i = 0; i < p_lsl->num; i++){
        sensor_num += (*(p_lsl->lsl_head))->num;
    }

    return sensor_num;
}

int32_t get_sensor_coord_in_lsl(const p_ls_list_t p_lsl, uint8_t pos)
{
    int16_t i;
    int32_t origin = 0; // ラインセンサ座標原点
    int32_t sensor_coord = 0; // センサ0を基準とした指定センサ位置
    int16_t lsl_index = 0; // 実際の参照ラインセンサのインデックス
    uint8_t reference = pos; // 実際の参照先センサ位置
    ls_material_t *target_lsm_p; // 参照ラインセンサ構造体へのポインタ

    // 原点の計算
    for(i = 0; i < p_lsl->num; i++){
        target_lsm_p = *(p_lsl->lsl_head + i);
        origin += (target_lsm_p->sensor_interval) * (target_lsm_p->num - 1)
            + target_lsm_p->ls_interval;
    }
    origin /= 2;

    // 指定の参照センサがどのラインセンサにあるか探す
    target_lsm_p = *(p_lsl->lsl_head);
    while((lsl_index < p_lsl->num)
          && (reference >= target_lsm_p->num)){
        reference -= target_lsm_p->num;
        sensor_coord += (target_lsm_p->sensor_interval) * (target_lsm_p->num - 1)
            + target_lsm_p->ls_interval;
        lsl_index++;
        target_lsm_p = *(p_lsl->lsl_head + lsl_index);
    }

    // 座標を算出
    if((lsl_index < p_lsl->num) && (reference < target_lsm_p->num)){
        sensor_coord += (target_lsm_p->sensor_interval) * reference;
        return (sensor_coord - origin);
    }else{
        // 不正な参照には最大値を返す
        return origin;
    }    
}    
    

ls_line_t get_line_in_lsl(const p_ls_list_t p_lsl)
{
    int16_t i;
    uint8_t total_sensor_num = get_sensor_num_in_lsl(p_lsl); // センサ総数
    int32_t sum_of_ls_coord = 0; // 反応点座標の総和
    int16_t act_counter = 0; // 反応点カウンタ
    int32_t max_sum_of_ls_coord = 0; // 最長反応点群の反応点座標の総和
    int16_t max_act_counter = 0; // 最長反応点群の反応点数
    int16_t max_act_tail; // 最大反応点数ライン後最初の無反応点
    uint8_t sensor_act[total_sensor_num]; // 反応点格納配列
    ls_line_t retval = {0, 0}; // 返り値

    // 反応点群の誤反応を除去するため反応点に挟まれた無反応点は反応点として扱う
    // ただし両端の端点はフィルタをかけない
    for(i = 0; i < total_sensor_num; i++){
        sensor_act[i] = dv_is_linesensor_list(p_lsl, i);
    }
    for(i = 1; i < total_sensor_num - 1; i++){
        sensor_act[i] = ((sensor_act[i - 1] & sensor_act[i + 1]) | sensor_act[i]);
    }

    // 反応点座標のカウント
    for(i = 0; i < total_sensor_num; i++){
        if(sensor_act[i]){
            act_counter++;
            sum_of_ls_coord += get_sensor_coord_in_lsl(p_lsl, i);
        }else{
            if((act_counter >= _min_sensor_num_for_line)
               && (act_counter > max_act_counter)){
                max_act_tail = i;
                max_act_counter = act_counter;
                max_sum_of_ls_coord = sum_of_ls_coord;
            }
            act_counter = 0;
            sum_of_ls_coord = 0;
        }
    }

    // 最後に無反応点がなくても判定を行う
    if((act_counter >= _min_sensor_num_for_line)
       && (act_counter > max_act_counter)){
        max_act_tail = i;
        max_act_counter = act_counter;
        max_sum_of_ls_coord = sum_of_ls_coord;
    }
    act_counter = 0;
    sum_of_ls_coord = 0;

    // ラインが見つからなかったらエラーフラグを立て、座標・太さともに0を返す
    if(!max_act_counter){
        p_lsl->error = TRUE;
        return retval;
    }else{
        p_lsl->error = FALSE;
    }

    // 座標と太さの算出
    retval.dist = max_sum_of_ls_coord / max_act_counter;
    retval.width = get_sensor_coord_in_lsl(p_lsl, max_act_tail - 1) -
        get_sensor_coord_in_lsl(p_lsl, max_act_tail - max_act_counter);

    return retval;
}

position_t get_lslbit_coord_avg_in_map(const p_ls_list_t p_lsl, const p_map_t map, int16_t bit)
{
    position_t machine_pos = get_map_pos(map); // マシン中心座標
    int32_t lsl_pos = get_sensor_coord_in_lsl(p_lsl, bit); // ラインセンサ座標
    position_t retpos = {0, 0, 0, 0, 0}; // 返り値

    uint16_t dist_from_wheel_center = 0; // 車輪中心からマシン中心までの直線距離
    int16_t phi_of_lsl_center = 0; // ラインセンサリスト中心のマシンに対する姿勢角度(マシン進行方向が0で反時計回り正)

    // ラインセンサエラーの場合は問答無用でall 0を返す
    if(p_lsl->error == TRUE){
        return retpos;
    }
  
    // まずラインセンサリスト中心の極座標を出す
    polar(p_lsl->x_from_wheel_center, p_lsl->y_from_wheel_center, &dist_from_wheel_center, &phi_of_lsl_center);

    // ラインセンサリスト反応点の指定マップ上での座標を計算
    retpos.x = machine_pos.x
        + ((int32_t)dist_from_wheel_center * i_cos(machine_pos.theta + (int32_t)phi_of_lsl_center))
        - multiply_divide(lsl_pos, i_sin(machine_pos.theta + (int32_t)phi_of_lsl_center + p_lsl->psi_from_machine_y), UNIT_LS);
    retpos.y = machine_pos.y
        + ((int32_t)dist_from_wheel_center * i_sin(machine_pos.theta + (int32_t)phi_of_lsl_center))
        + multiply_divide(lsl_pos, i_cos(machine_pos.theta + (int32_t)phi_of_lsl_center + p_lsl->psi_from_machine_y), UNIT_LS);
    retpos.theta = machine_pos.theta + (int32_t)phi_of_lsl_center; // マップに対するラインセンサ中心の姿勢角度
    retpos.v = machine_pos.v;
    retpos.omega = machine_pos.omega;

    return retpos;
}


position_t get_lsl_coord_avg_in_map(const p_ls_list_t p_lsl, const p_map_t map)
{
    position_t machine_pos = get_map_pos(map); // マシン中心座標
    ls_line_t lsl_line = get_line_in_lsl(p_lsl); // ライン検知
    int32_t lsl_pos = lsl_line.dist; // ラインセンサ座標
    position_t retpos = {0, 0, 0, 0, 0}; // 返り値

    uint16_t dist_from_wheel_center = 0; // 車輪中心からマシン中心までの直線距離
    int16_t phi_of_lsl_center = 0; // ラインセンサリスト中心のマシンに対する姿勢角度(マシン進行方向が0で反時計回り正)

    // ラインセンサエラーの場合は問答無用でall 0を返す
    if(p_lsl->error == TRUE){
        return retpos;
    }
  
    // まずラインセンサリスト中心の極座標を出す
    polar(p_lsl->x_from_wheel_center, p_lsl->y_from_wheel_center, &dist_from_wheel_center, &phi_of_lsl_center);

    // ラインセンサリスト反応点の指定マップ上での座標を計算
    retpos.x = machine_pos.x
        + ((int32_t)dist_from_wheel_center * i_cos(machine_pos.theta + (int32_t)phi_of_lsl_center))
        - multiply_divide(lsl_pos, i_sin(machine_pos.theta + (int32_t)phi_of_lsl_center + p_lsl->psi_from_machine_y), UNIT_LS);
    retpos.y = machine_pos.y
        + ((int32_t)dist_from_wheel_center * i_sin(machine_pos.theta + (int32_t)phi_of_lsl_center))
        + multiply_divide(lsl_pos, i_cos(machine_pos.theta + (int32_t)phi_of_lsl_center + p_lsl->psi_from_machine_y), UNIT_LS);
    retpos.theta = machine_pos.theta + (int32_t)phi_of_lsl_center; // マップに対するラインセンサ中心の姿勢角度
    retpos.v = machine_pos.v;
    retpos.omega = machine_pos.omega;

    return retpos;
}

void set_ls_material(p_ls_material_t p_lsm, hal_linesensor _hal_ls, 
                     direction_t _direction, uint8_t _num, 
                     int32_t _sensor_interval, int32_t _ls_interval){
    p_lsm->hal_ls = _hal_ls;
    p_lsm->direction = _direction;
    p_lsm->num = _num;
    p_lsm->sensor_interval = _sensor_interval;
    p_lsm->ls_interval = _ls_interval;
}

void set_ls_list(p_ls_list_t p_lsl, p_ls_material_t *_lsl_head, 
                 uint8_t _num, int16_t _x_from_wheel_center,
                 int16_t _y_from_wheel_center, int32_t _psi_from_machine_y)
{
    p_lsl->lsl_head = _lsl_head;
    p_lsl->num = _num;
    p_lsl->x_from_wheel_center = _x_from_wheel_center;
    p_lsl->y_from_wheel_center = _y_from_wheel_center;
    p_lsl->psi_from_machine_y = _psi_from_machine_y;
    p_lsl->error = FALSE;
}

void set_lsl_package(p_lsl_package_t p_lsl_package,
                     p_ls_list_t *_device, uint8_t _num)
{
    p_lsl_package->device = _device;
    p_lsl_package->num = _num;
}




//! ラインセンサパッケージ内のラインセンサリストの数を返す.
/*!
  \param p_pack   対象のラインセンサパッケージ
  \retval         ラインセンサリストの数
 */
uint8_t get_lsl_num_in_package(const p_lsl_package_t p_pack)
{
    return p_pack->num;
}

//! ラインセンサパッケージ内のラインセンサリストの数を返す.
/*!
  \param p_pack   対象のラインセンサパッケージ
  \param id       ラインセンサリストのID
  \retval         ラインセンサリストを返す. 不正なIDに対しては, NULLを返す
 */
p_ls_list_t get_lsl_in_package(const p_lsl_package_t p_pack, uint8_t id)
{
    if(get_lsl_num_in_package(p_pack) > id){
        return p_pack->device[id];
    }
    return NULL;
}


//! ラインセンサが反応しているか否かを返す.
/*!
  \param p_pack    対象のラインセンサパッケージ
  \param index     対象の素子ID
  \param p_lsl     戻り値のラインセンサリストへのポインタ
  \param id_in_lsl 戻り値のラインセンサリスト内のID
  TODO: 動作確認
 */
void get_lsl_and_num_in_package(const p_lsl_package_t p_pack, int id_in_pack, p_ls_list_t* p_lsl, int* id_in_lsl)
{
    int n_lsl; //ラインセンサリストの数
    int n_sensor; //リスト内の素子数
    int n_sensor_all; //今までに数えた素子合計
    int i;
    n_lsl = get_lsl_num_in_package(p_pack);    
    n_sensor_all = 0;
    for(i=0;i<n_lsl;i++){
        *p_lsl = get_lsl_in_package(p_pack, i);
        n_sensor = get_sensor_num_in_lsl(*p_lsl);
        if(n_sensor_all+n_sensor > id_in_pack){
            *id_in_lsl = id_in_pack - n_sensor_all;         
            return;
        }
        n_sensor_all += n_sensor;
    }
    return;
}

//! ラインセンサが反応しているか否かを返す.
/*!
  \param p_pack   対象のラインセンサパッケージ
  \param index    対象の素子番号
  \retval         素子番号が反応しているときのみ真を返す
  TODO: 動作確認
 */
uint8_t dv_is_linesensor_in_package(const p_lsl_package_t p_pack, int id)
{
    int id_in_lsl;
    p_ls_list_t p_lsl;
    get_lsl_and_num_in_package(p_pack,id,&p_lsl,&id_in_lsl);
    return dv_is_linesensor_list(p_lsl,id_in_lsl);
}

//! ラインセンサを指定座標系での表現に直して取得する
/*!
  \param p_lsl       対象のラインセンサリスト
  \param machine_pos 指定マシンの中心座標
  \param bit         ラインセンサ素子のid

  get_lsl_coord_avg_in_map(dv_linesensor.c)のコードのコピペ.
  今までは, mapとposの情報がセットで与えられることが前提だった.
 */
position_t get_lslbit_coord_avg_in_pos(const p_ls_list_t p_lsl, position_t machine_pos, int16_t bit)
{
    int32_t lsl_pos = get_sensor_coord_in_lsl(p_lsl, bit); // ラインセンサ座標
    position_t retpos = {0, 0, 0, 0, 0}; // 返り値

    uint16_t dist_from_wheel_center = 0; // 車輪中心からマシン中心までの直線距離
    int16_t phi_of_lsl_center = 0; // ラインセンサリスト中心のマシンに対する姿勢角度(マシン進行方向が0で反時計回り正)

    // ラインセンサエラーの場合は問答無用でall 0を返す
    if(p_lsl->error == TRUE){
        return retpos;
    }
  
    // まずラインセンサリスト中心の極座標を出す
    polar(p_lsl->x_from_wheel_center, p_lsl->y_from_wheel_center, &dist_from_wheel_center, &phi_of_lsl_center);

    // ラインセンサリスト反応点の指定マップ上での座標を計算
    retpos.x = machine_pos.x
        + ((int32_t)dist_from_wheel_center * i_cos(machine_pos.theta + (int32_t)phi_of_lsl_center))
        - multiply_divide(lsl_pos, i_sin(machine_pos.theta + (int32_t)phi_of_lsl_center + p_lsl->psi_from_machine_y), UNIT_LS);
    retpos.y = machine_pos.y
        + ((int32_t)dist_from_wheel_center * i_sin(machine_pos.theta + (int32_t)phi_of_lsl_center))
        + multiply_divide(lsl_pos, i_cos(machine_pos.theta + (int32_t)phi_of_lsl_center + p_lsl->psi_from_machine_y), UNIT_LS);
    retpos.theta = machine_pos.theta + (int32_t)phi_of_lsl_center; // マップに対するラインセンサ中心の姿勢角度
    retpos.v = machine_pos.v;
    retpos.omega = machine_pos.omega;

    return retpos;
}

//! ラインセンサを指定座標系での表現に直して取得する
/*!
  \param p_pack   対象のラインセンサパッケージ
  \param pos      指定マシン座標
  \param bit      ラインセンサ素子のid
 */
position_t get_lslpackbit_coord_avg_in_pos(const p_lsl_package_t p_pack, position_t pos, int16_t bit)
{
    int id_in_lsl;
    p_ls_list_t p_lsl = NULL;
    get_lsl_and_num_in_package(p_pack,bit,&p_lsl,&id_in_lsl);
    return get_lslbit_coord_avg_in_pos(p_lsl,pos,id_in_lsl);
}



#endif
