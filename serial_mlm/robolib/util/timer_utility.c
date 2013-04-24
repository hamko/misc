/*!
  \file    timer_utility.c
  \ingroup timer
  \author  Tanaka Taichi<t.t.tecobom@gmail.com>
  \date 2010/7/15
  
  \brief タイマーとそれを利用したツールの内容
*/

#include "timer_utility.h"
#include "function_list.h"
#ifdef __INSTALL_TIMER__

static volatile uint32_t _time = 0;    //!< 経過時間を保持する
static uint32_t _cycle;  //!<制御周期を保持する。
static void timer(void);

void init_timer_utility(uint32_t cycle){
    set_function(timer,LABEL_TIMER);
    _cycle = cycle;
    _time = 0;
    return;
}

//! カウントアップ
static void timer(void){
    _time += _cycle;
    return;
}

int32_t get_time(void){
    return _time;
}

void wait_cycles(uint32_t n){
    uint32_t end_time = _time + n*_cycle;
    
    //割り込み内部では実行されないようにしている
    if(is_interrupting() != 0){
        return;
    }
    
    while(end_time > _time);
    return;
}

void wait_ms(uint32_t ms){
    uint32_t end_time = _time + ms;
    
    //割り込み内部では実行されないようにしている
    if(is_interrupting() != 0){
        return;
    }
    while(end_time > _time);
    return;
}

#endif
