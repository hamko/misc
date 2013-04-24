/*!
 *  \file  hal_pscontroller.h
 * \author YABE Hiroki
 * \brief hal_pscontroller.c用ヘッダファイル
 * \date 2008/1/8
 */

#include "util.h"

#ifndef HAL_PSCONTROLLER_H_
#define HAL_PSCONTROLLER_H_

/* //! 符号なし1バイト変数 */
/* typedef unsigned char uchar; */

//! hal_pscontroller_tagの定義はhal_pscontroller_prv.h参照
typedef struct hal_pscontroller_tag *hal_pscontroller;

/*! \brief データ構造体
 *
 * コントローラの状態を格納する構造体。
 */
typedef struct _psc_keys{
    uchar id;                                     //!< コントローラID
    uchar change_flag;                        //!< 大抵0x5Aが入っているはず。使わないでください。

    union {
        uchar byte3;
        struct {
            unsigned key_left:1;                //!< 十字キー左。
            unsigned key_down:1;              //!< 十字キー下。
            unsigned key_right:1;               //!< 十字キー右。
            unsigned key_up:1;                  //!< 十字キー上。
            unsigned key_start:1;             //!< スタートボタン。
            unsigned key_right_handle_sw:1; //!< ハンドルスイッチ右。アナログモードの時のみ有効。それ以外の時は常に1。
            unsigned key_left_handle_sw:1;  //!< ハンドルスイッチ左。アナログもーどの時のみ有効。それ以外の時は常に1。
            unsigned key_select:1;           //!< セレクトボタン。
        };
    };

    union {
        uchar byte4;
        struct {
            unsigned key_rect:1;              //!< しかくボタン。
            unsigned key_cross:1;           //!< ばつボタン。
            unsigned key_circle:1;            //!< まるボタン。
            unsigned key_triangle:1;          //!< さんかくボタン。
            unsigned key_r1:1;                //!< R1。
            unsigned key_l1:1;                //!< L1。
            unsigned key_r2:1;                //!< R2。
            unsigned key_l2:1;                //!< L2。
        };
    };

    uchar c_right_handle_horizontal;    //!< 右ハンドル左右方向。アナログモード時のみ有効。倒さない状況で0x80付近、上に倒しきると0x00、下に倒しきると0xFFとなる。
    uchar c_right_handle_vertical;      //!< 右ハンドル上下方向。アナログモード時のみ有効。倒さない状況で0x80付近、左に倒しきると0x00、右に倒しきると0xFFとなる。
    uchar c_left_handle_horizontal;   //!< 左ハンドル左右方向。
    uchar c_left_handle_vertical;       //!< 左ハンドル上下方向。

    uchar c_pressure_right; //!< 感圧右 押していない時0x00, 押しているとき0xFF。以下同様
    uchar c_pressure_left;  //!< 感圧左
    uchar c_pressure_up;    //!< 感圧上
    uchar c_pressure_down;  //!< 感圧下

    uchar c_pressure_triangle; //!< 感圧 さんかく
    uchar c_pressure_circle;   //!< 感圧 まる
    uchar c_pressure_cross;    //!< 感圧 ばつ
    uchar c_pressure_rect;     //!< 感圧 しかく

    uchar c_pressure_l1;  //!< 感圧 L1
    uchar c_pressure_r1;  //!< 感圧 R1
    uchar c_pressure_l2;  //!< 感圧 L2
    uchar c_pressure_r2;  //!< 感圧 R2

} psc_keys, *ppsc_keys;

enum{
    PSC_MODEL_NORMAL = 0x00, PSC_MODEL_DUALSHOCK = 0x01
};

enum{
    PSC_MODE_ANALOG = 0x70, PSC_MODE_DIGITAL = 0x40, PSC_MODE_CONFIG = 0xF0
};

enum{
    PSC_MODE_LOCK = 0x03, PSC_MODE_UNLOCK = 0x00
};

//enum{
//  PSC_VIBRATION_ENABLED = 0x01, PSC_VIBRATION_DISABLED = 0x00
//};

/*! \brief コントローラを初期化する。
 * \param _controller 対象のコントローラ
 */
void clear_pscontroller(hal_pscontroller _controller);

/*! \brief キーを取得する。
 * \param _controller 対象のコントローラ
 * \return psc_keys型構造体へのポインタ。
 */
ppsc_keys get_pscontroller_keys(hal_pscontroller _controller);

/*! \brief コントローラのモデルを取得。
 * \param _controller 対象のコントローラ
 * \retval PSC_MODEL_NORMAL 普通のコントローラ
 * \retval PSC_MODEL_DUALSHOCK DualShock
 */


/*! \brief モードを設定する
 * \param _controller 対象のコントローラ
 * \param _mode モードを指定する。PSC_MODE_ANALOG または PSC_MODE_DIGITAL(デフォルト)を指定する。
 */
void set_pscontroller_mode(hal_pscontroller _controller, uchar _mode);

/*! \brief モードを取得する
 * \param _controller 対象のコントローラ
 * \retval PSC_MODE_ANALOG アナログモード
 * \retval PSC_MODE_DIGITAL デジタルモード
 */
uchar get_pscontroller_mode(hal_pscontroller _controller);

/*! \brief モードロックを設定する
 *
 * モードロックを設定すると、コントローラのANALOGボタン(ハンドルの間にあるボタン)によるモード変更が禁止される。
 * すなわち、モード変更はプログラム側からのみ許可される。
 * \param _controller 対象のコントローラ
 * \param _mode_lock PSC_MODE_LOCK(設定) または PSC_MODE_UNLOCK(解除;デフォルト) を指定する。
 */
void set_pscontroller_mode_lock(hal_pscontroller _controller, uchar _mode_lock);

/*! \brief モードロックを取得する
 * \param _controller 対象のコントローラ
 * \retval PSC_MODE_LOCK モードロックは設定されている。
 * \retval PSC_MODE_UNLOCK モードロックは解除されている。
 */
uchar get_pscontroller_mode_lock(hal_pscontroller _controller);

/*! \brief 感圧モードを有効にする
 *
 * 実装は H1_as_pscontroller 内
 */
void enable_pscontroller_pressure_mode(hal_pscontroller _controller);

/*! \brief 感圧モードを無効にする
 */
void disable_pscontroller_pressure_mode(hal_pscontroller _controller);

#endif /*HAL_PSCONTROLLER_H_*/
