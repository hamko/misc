/*!
  \file    dv_linesensor.h
  \ingroup device
  \author  Iori <ik0313@gmail.com>
  \date    2010/08/12

  \brief   複数のラインセンサを統一的に扱うための構造体・関数定義
 */

#include "install_option.h"
#ifndef __INSTALL_DEVICE__
#error device is not installed
#endif

#ifndef __INCLUDED_DV_LINESENSOR_H__
#define __INCLUDED_DV_LINESENSOR_H__

#include "device.h"
#include "libhal.h"
#include "r_stdint.h"
#include "map.h"

#define UNIT_LS 10 //! ラインセンサ座標の1mm

//! ラインセンサ単独構造体
typedef struct _ls_material_t{
    hal_linesensor hal_ls;
    direction_t direction; //!< NEGATIVEならセンサの番号付けが逆になる(デフォルトの方向はRoboTech座標系y方向の定義に準拠)
    uint8_t num; //!< ラインセンサ上のセンサ個数
    int32_t sensor_interval; //!< ラインセンサ上のセンサ間隔(単位0.1mm)
    int32_t ls_interval; //!< 最後のセンサから次のラインセンサ上のセンサまでの距離(単位0.1mm, 次がない場合は0)
} ls_material_t;
typedef ls_material_t *p_ls_material_t;

//! ラインセンサリスト構造体
typedef struct _ls_list_t{
    p_ls_material_t *lsl_head; //!< ラインセンサ構造体へのポインタを保持する配列の先頭ポインタ
    uint8_t num; //!< リストの保持しているラインセンサ数
    int16_t x_from_wheel_center; //!< 車輪中心からラインセンサリスト中心までのx方向距離(単位mm)
    int16_t y_from_wheel_center; //!< 車輪中心からラインセンサリスト中心までのy方向距離(単位mm)
    int32_t psi_from_machine_y; //!< ラインセンサリストの姿勢角度(単位1/10000rad)。マシン中心からラインセンサ中心に向かう直線にラインセンサリストが垂直の時0で半時計回り正
    BOOLEAN error; //!< ラインセンサに例外(ラインが見つからない等)が起こったら立てる
} ls_list_t;
typedef ls_list_t *p_ls_list_t;

//! ラインセンサリストをまとめて保持するための構造体
typedef struct _lsl_package_t{
    p_ls_list_t *device; //!< ラインセンサリストへのポインタ
    int16_t num; //!< ラインセンサリストの数
} lsl_package_t;
typedef lsl_package_t *p_lsl_package_t;

//! ラインセンサにより検知されたライン情報を格納する構造体
typedef struct _ls_line_t{
    int32_t dist; // ライン中心のラインセンサ中心から見た位置(単位0.1mm)
    int32_t width; // ラインの太さ(単位0.1mm, 対象ラインに反応しているセンサ番号が最大の反応点と最小の反応点の位置座標の差)
} ls_line_t;

//! ライン判別用定数の初期化
/*!
  \param min_sensor_num_for_line ラインとみなす最低連続反応数
 */
void init_line_info(uint8_t min_sensor_num_for_line);

//! ラインセンサ単独構造体の指定ビットの値を向きを考慮して返す
/*!
  \param p_lsm 対象の単独ラインセンサ構造体
  \param pos   指定ビット
 */
uint8_t dv_is_linesensor(const p_ls_material_t p_lsm, uint8_t pos);

//! ラインセンサリストの指定ビットの値を向きを考慮して返す
/*!
  \param p_lsl 対象のラインセンサリスト
  \param pos   指定ビット
 */
uint8_t dv_is_linesensor_list(const p_ls_list_t p_lsl, uint8_t pos);

//! ラインセンサリストにあるセンサ総数を返す
/*!
  \param p_lsl 対象のラインセンサリスト
 */
uint8_t get_sensor_num_in_lsl(const p_ls_list_t p_lsl);

//! ラインセンサリストの指定ビットに相当する点の座標を返す(単位0.1mm, ラインセンサリストの中心が原点)
/*!
  \param p_lsm 対象のラインセンサリスト
  \param pos   指定ビット
 */
int32_t get_sensor_coord_in_lsl(const p_ls_list_t p_lsl, uint8_t pos);

//! ラインセンサリストの反応点最大のラインを検出し、そのライン中心からの座標と太さを返す(単位0.1mm)
/*!
  \param p_lsl 対象のラインセンサリスト

  複数のラインセンサ反応点列がある場合、最も長い反応点列の平均座標を返す
  最長点列が複数有る場合最もラインセンサ番号が小さいものの平均座標を返す
  ラインが見つからない場合はerrorフラグを立てて座標・太さともに0を返す

  \todo ライン太さ単体からライン方向に対する傾きの正負が判別できない
*/
ls_line_t get_line_in_lsl(const p_ls_list_t p_lsl);

//! ラインセンサの指定ビットを指定座標系上での表現に直して取得する(単位0.1mm)
/*!
  \param p_lsl 対象のラインセンサリスト
  \param map   指定マップ
  \param bit   対象ラインセンサリストのビット

  thetaはラインセンサ中心のマップに対する姿勢角度を返す
  v, omegaはマシンのものと同じ
  ラインが見つからない場合はerrorフラグを立ててall 0を返す
*/
position_t get_lslbit_coord_avg_in_map(const p_ls_list_t p_lsl, const p_map_t map, int16_t bit);

//! ラインセンサの反応点を指定座標系上での表現に直して取得する(単位0.1mm)
/*!
  \param p_lsl 対象のラインセンサリスト
  \param map   指定マップ

  複数のラインセンサ反応点列がある場合、最も長い反応点列の平均座標を返す
  最長点列が複数有る場合最もラインセンサ番号が小さいものの平均座標を返す
  thetaはラインセンサ中心のマップに対する姿勢角度を返す
  v, omegaはマシンのものと同じ
  ラインが見つからない場合はerrorフラグを立ててall 0を返す
*/
position_t get_lsl_coord_avg_in_map(const p_ls_list_t p_lsl, const p_map_t map);

//! ls_material型構造体設定関数
/*!

  ls_materialはポインタで渡す。別途実体を定義する必要有り
*/
void set_ls_material(p_ls_material_t p_lsm, hal_linesensor _hal_ls, 
                     direction_t _direction, uint8_t _num, 
                     int32_t _sensor_interval, int32_t _ls_interval);

//! ls_list型構造体設定関数
/*!
  \param p_lsl 設定対象のラインセンサリスト
  
  ls_listはポインタで渡す。別途実体を定義する必要有り
  各種値の設定が非常に特殊なので注意
  _x_from_wheel_center, _y_from_wheel_centerについてはラインセンサの実際のマシン上の位置を登録する
  (これはラインセンサリストの回転角phiを導出するのに必要なため)
  _psi_from_machine_y及び_lsl_headのラインセンサの向き(POSITIVE / NEGATIVE)は「回転角phiだけラインセンサリストが回転する前の状態」、
  すなわち指定のラインセンサリストがマシンのエンコーダ中心からx軸正方向上にあると仮定した場合でのラインセンサリストy軸に対する回転角phi
  及びラインセンサの向きを指定しなければならない。

  \todo 指定の仕方の簡略化・直感化
*/

void set_ls_list(p_ls_list_t p_lsl, p_ls_material_t *_lsl_head, 
                 uint8_t _num, int16_t _x_from_wheel_center,
                 int16_t _y_from_wheel_center, int32_t _psi_from_machine_y);

//! lsl_package型構造体設定関数
/*!
  \param p_lsl_package 設定対象のラインセンサリスト

  lsl_packageはポインタで渡す。別途実体を定義する必要有り
*/
void set_lsl_package(p_lsl_package_t p_lsl_package,
                     p_ls_list_t *_device, uint8_t _num);



//! ラインセンサパッケージ内のラインセンサリストの数を返す.
/*!
  \param p_pack   対象のラインセンサパッケージ
  \retval         ラインセンサリストの数
 */
uint8_t get_lsl_num_in_package(const p_lsl_package_t p_pack);

//! ラインセンサパッケージ内のラインセンサリストの数を返す.
/*!
  \param p_pack   対象のラインセンサパッケージ
  \param id       ラインセンサリストのID
  \retval         ラインセンサリストを返す. 不正なIDに対しては, NULLを返す
 */
p_ls_list_t get_lsl_in_package(const p_lsl_package_t p_pack, uint8_t id);


//! ラインセンサが反応しているか否かを返す.
/*!
  \param p_pack    対象のラインセンサパッケージ
  \param index     対象の素子ID
  \param p_lsl     戻り値のラインセンサリスト
  \param id_in_lsl 戻り値のラインセンサリスト内のID
  TODO: 動作確認
 */
void get_lsl_and_num_in_package(const p_lsl_package_t p_pack, int id_in_pack, p_ls_list_t* p_lsl, int* id_in_lsl);

//! ラインセンサが反応しているか否かを返す.
/*!
  \param p_pack   対象のラインセンサパッケージ
  \param index    対象の素子番号
  \retval         素子番号が反応しているときのみ真を返す
  TODO: 動作確認
 */
uint8_t dv_is_linesensor_in_package(const p_lsl_package_t p_pack, int id);

//! ラインセンサを指定座標系での表現に直して取得する
/*!
  \param p_lsl       対象のラインセンサリスト
  \param machine_pos 指定マシンの中心座標
  \param bit         ラインセンサ素子のid

  get_lsl_coord_avg_in_map(dv_linesensor.c)のコードのコピペ.
  今までは, mapとposの情報がセットで与えられることが前提だった.
 */
position_t get_lslbit_coord_avg_in_pos(const p_ls_list_t p_lsl, position_t machine_pos, int16_t bit);

//! ラインセンサを指定座標系での表現に直して取得する
/*!
  \param p_pack   対象のラインセンサパッケージ
  \param pos      指定マシン座標
  \param bit      ラインセンサ素子のid
 */
position_t get_lslpackbit_coord_avg_in_pos(const p_lsl_package_t p_pack, position_t pos, int16_t bit);


#endif
