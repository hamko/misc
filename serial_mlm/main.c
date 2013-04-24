#include"hal.h"
#include"util.h"
#include"kakitani.h"
#include"i_math.h"
#include"locus.h"
#include"timer_utility.h"
#include "sim_init.h"
#include "world.h"
#include "mlm.h"
#include "map.h"
#include "odometry.h"

//#include "stream.h"
//ライブラリの初期化
extern void initialize_sim_library(void);

// init.cで定義
extern void init_robolib(void);

int main(void)
{
    initialize_sim_library();
    set_world_pos_all(0, 0, 3.14 / 300, 0, 0);
//    set_world_pos_all(10, 10, 3.14 / 100, 0, 0);
//    set_world_pos_all(0, 0, 0, 0, 0);
    init_robolib();

    init_mlm(get_odometry_map(),
        pmlms, 500,
        100,
        2000, 500, 5000,
//        200, 50, 500,
        3, 3,
        0, 0, 0);
    set_execute_mlm_flag(TRUE);

    go_curve(5000,2000,0,1500,500);
//    go_curve(5000,0,0,1000,500);
    
    while(1);
    return 0;
}
