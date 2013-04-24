#include "sim_init.h"
#include "util.h"
#include "device_manager.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <gtk/gtk.h>

// 割り込み要求するか
int stop_interrupt = 1;
// 起動時からの割り込み回数
int interrupt_count = 0;
// マシン座標などをリストに保存 // draw.cで定義
void save_pos();

//! 割り込み開始
void start_timer(void);

void int_wovi(int signo);

//! Initializer of the Device Manager.
/*!
  device_manager.cで定義されているべき関数
 */
void DM_initialize(void);

//! Initializer of the Hardware Abstruction Layer.
/*!
  hal.cで定義されているべき関数。
 */
void HAL_initialize(void);

//! プログラマ処理関数
/*!
  control.cで定義されているべき関数。
 */
void control(void);

void initialize_sim_library(void)
{
    signal(SIGUSR1, int_wovi);

    dprintf("- 2008 Board Library Ver.2 -\n");
    dprintf("\t(C) 2008 RoboTech, The University of Tokyo.\n\n");

    dprintf("System intialization starts...\n\n");

    dprintf("Initializing device manager...\n");
    DM_initialize();
    dprintf("Done.\n\n");

    dprintf("Initializing HAL...\n");
    HAL_initialize();
    dprintf("Done.\n\n");

    dprintf("System initialization finished.\n");
    dprintf("2008 Board is now ready.\n\n");

    start_timer();
}

void int_wovi(int signo)
{
    UNUSED_VARIABLE(signo);
    DM_presync();
    control();
    DM_postsync();
    save_pos(); // for drawing orbit and graph
}

void *thread_int(void *arg)
{
    UNUSED_VARIABLE(arg);
    while(1){
    #ifdef WAIT_TIME_MODE
        usleep(WAIT_TIME_PER_CYCLE);
    #endif
        if(!stop_interrupt) {
            raise(SIGUSR1);
            interrupt_count++;
        }
    }
}

void *thread_gui(void *arg)
{
    UNUSED_VARIABLE(arg);
    gui_init();
    gtk_main();
    exit(0);
}

void start_timer(void)
{
    pthread_t pt;
    #ifdef GUI_MODE
    pthread_create( &pt, NULL, &thread_gui, NULL ); 
    #endif
    pthread_create( &pt, NULL, &thread_int, NULL );
}

