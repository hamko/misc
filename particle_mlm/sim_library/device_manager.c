/*!
  \file   device_manager.c
  \author TAKISAWA, Jun'ichi.
  \date   Fri Jan 11 10:04:23 2008

  \brief  デバイス管理プログラム。プレフィクスは"DM"。
 */

#include "device_manager.h"
#include "util.h"

#define DEVICE_MAX 30

static DM_device_info device_list[DEVICE_MAX];
static int device_counter = 0;

void DM_initialize(void)
{
    /*!
      全ての関数ポインタをNULLに初期化し、カウンタを0クリアする。
     */
    int i;

    for(i = 0 ; i < DEVICE_MAX ; i++)
    {
        device_list[i].presync = NULL;
        device_list[i].postsync = NULL;
    }
    device_counter = 0;

    dprintf("\tMaximum number of device is %d.\n", DEVICE_MAX);
}

hal_device* DM_add_device(void (*presync)(hal_device *device),
        void (*postsync)(hal_device *device))
{
    /*!
      デバイスが登録可能かを判定し、引数として渡された関数を登録する。
      登録後デバイスカウンタをの値を更新する。
     */

    if(device_counter >= DEVICE_MAX)
    {
        dprintf("\nFatal: No more devices.\n-ABORT-\n");
        while(1);
    }

    device_list[device_counter].presync = presync;
    device_list[device_counter].postsync = postsync;
    device_counter++;

    return &(device_list[device_counter-1].device);
}

void DM_presync(void)
{
    int i;
    //! 登録されているデバイスに対し、presync関数が登録されていればpresyncを行う。
    for(i = 0; i < device_counter; i++)
    {
        if(device_list[i].presync != NULL)
        {
            device_list[i].presync(&(device_list[i].device));
        }
    }
}

void DM_postsync(void)
{
    int i;
    //! 登録されているデバイスに対し、postsync関数が登録されていればpostsyncを行う。
    for(i = 0; i < device_counter; i++)
    {
        if(device_list[i].postsync != NULL)
        {
            device_list[i].postsync(&(device_list[i].device));
        }
    }
}
