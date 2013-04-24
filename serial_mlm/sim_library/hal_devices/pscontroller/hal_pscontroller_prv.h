/*!
 *  \file hal_pscontroller_prv.h
 * \brief プレイステーション用プライベートヘッダ
 * \author YABE Hiroki
 * \date 200/1/8
 */

#ifndef HAL_PSCONTROLLER_PRV_H_
#define HAL_PSCONTROLLER_PRV_H_

#include "hal_pscontroller.h"

/*! \brief 仮想コントローラの、更新フラグ
 *
 */
typedef enum {
    PSC_MODE_CHANGED = 0x01,              //!< mode needs updating.
    PSC_MODE_LOCK_CHANGED = 0x02,         //!< mode lock needs updating.
    PSC_VIBRATION_ENABLED_CHANGED = 0x04, //!< for future use.
    PSC_VIBRATE = 0x08,                   //!< for future use.
} psc_changed;

/*!
 * \brief 仮想コントローラ
 */
struct hal_pscontroller_tag{
    psc_keys key_data;       //!< key_data. see psc_keys for details.
    uchar model;              //!< PSC_MODEL_NORMAL or PSC_MODE_DUALSHOCK
    uchar mode;               //!< PSC_MODE_ANALOG or PSC_MODE_DIGITAL
    uchar mode_lock;          //!< PSC_MODE_LOCK or PSC_MODE_UNLOCK
    uchar vibration_enabled;  //!< reserved for future use.
    psc_changed changed;      //!< changeflg. see psc_changed for detail.
};



#endif /*HAL_PSCONTROLLER_PRV_H_*/
