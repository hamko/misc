/*! \file hal_pscontroller.c
 *   \brief プレイステーションコントローラオブジェクト
 *   \author YABE Hiroki
 *   \date 2008/1/8
 */

#include "hal_pscontroller_prv.h"

void clear_pscontroller(hal_pscontroller _controller)
{
    _controller->key_data.id = 0;
    _controller->key_data.change_flag = 0;
    _controller->key_data.byte3 = 0;
    _controller->key_data.byte4 = 0;
    _controller->key_data.c_right_handle_horizontal = 0;
    _controller->key_data.c_right_handle_vertical = 0;
    _controller->key_data.c_left_handle_horizontal = 0;
    _controller->key_data.c_left_handle_vertical = 0;
    _controller->model = 0;
    _controller->mode = PSC_MODE_DIGITAL;
    _controller->mode_lock = 0;
    _controller->vibration_enabled = 0;
    _controller->changed =0;

    return;
}

ppsc_keys get_pscontroller_keys(hal_pscontroller _controller)
{
    return &_controller->key_data;
}

void set_pscontroller_mode(hal_pscontroller _controller, uchar _mode)
{
    // 不正なモード指定を拒否する。
    if( _mode != PSC_MODE_DIGITAL && _mode != PSC_MODE_ANALOG ) return;

    _controller->changed |= PSC_MODE_CHANGED;
    _controller->mode = _mode;

    return;
}

uchar get_pscontroller_mode(hal_pscontroller _controller)
{
    return _controller->mode;
}

void set_pscontroller_mode_lock(hal_pscontroller _controller, uchar _mode_lock)
{
    //不正な入力を拒否する
    if( _mode_lock != PSC_MODE_LOCK && _mode_lock != PSC_MODE_UNLOCK ) return;

    _controller->changed |= PSC_MODE_LOCK_CHANGED;
    _controller->mode_lock = _mode_lock;

    return;
}

uchar get_pscontroller_mode_lock(hal_pscontroller _controller)
{
    return _controller->mode_lock;
}

