#ifndef _CB_H_
#define _CB_H_

#include <gtk/gtk.h>
// undefしないとエラーになる
#undef FALSE
#undef TRUE
#include "util.h"

// 割り込みを要求するか
extern int stop_interrupt;
// フィールド縮尺
extern double scale;
// グラフ縮尺
extern double graph_scale;
// ディスプレイ更新周期(=1000/fps)
extern int gui_cycle; /* msec */
// 軌道を描画するか
extern int orbit_checked;
// マシンを描画するか
extern int machine_checked;
// 各データをグラフに描画するか
int graph_checked[8];

#endif //_CB_H_
