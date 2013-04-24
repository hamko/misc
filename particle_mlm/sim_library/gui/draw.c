#include "draw.h"
#include "gui_params.h"
#include "hdwdef.h"
#include "world.h"

// フィールド縮尺
double scale = 10;
// グラフ縮尺
double graph_scale = 1;
// ウィンドウの幅,高さ
static int window_width;
static int window_height;
// 軌道を描画するか
int orbit_checked = 1;
// マシンを描画するか
int machine_checked = 1;
// 各データをグラフに描画するか
int graph_checked[8] = {0,0,0,0,0,0,0,0};
// マシン座標などのデータを保存するリスト
static GList *list = NULL;

void draw_orbit_list(gpointer data, gpointer usr_data)
{
    cairo_line_to((cairo_t*)usr_data,
     window_width / 2.0 - ((double*)data)[2] / scale, window_height / 2.0 - ((double*)data)[1] / scale);
}

// 使わない
void draw_graph_list(gpointer data, gpointer usr_data)
{
    cairo_line_to((cairo_t*)usr_data,
     ((double*)data)[0], window_height / 2.0 - 50 + ((double*)data)[1] / graph_scale);
}

// フィールド・マシンを再描画する // "expose-event"ハンドラ // 10msec周期で呼ばれる
gboolean draw(GtkWidget *widget, GdkEventExpose *event, gpointer user_data)
{
    double i;
    cairo_t *cr = gdk_cairo_create(widget->window);
    cairo_t *cr_orbit = gdk_cairo_create(widget->window);

    UNUSED_VARIABLE(event);

    gtk_window_get_size(GTK_WINDOW(user_data), &window_width, &window_height);
    
    // フィールド描画
    cairo_rectangle(cr, 0, 0, window_width, window_height);
    cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
    cairo_fill(cr);
    cairo_set_line_width(cr, LINE_WIDTH / scale);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    for(i = 0; i <= window_width / 2; i += LINE_INTERVAL / scale) {
        cairo_move_to(cr, window_width / 2 + i, 0);
        cairo_line_to(cr, window_width / 2 + i, window_height);
        cairo_stroke(cr);
        cairo_move_to(cr, window_width / 2 - i, 0);
        cairo_line_to(cr, window_width / 2 - i, window_height);
        cairo_stroke(cr);
    }
    for(i = 0; i <= window_height / 2; i += LINE_INTERVAL / scale) {
        cairo_move_to(cr, 0, window_height / 2 + i);
        cairo_line_to(cr, window_width, window_height / 2 + i);
        cairo_stroke(cr);
        cairo_move_to(cr, 0, window_height / 2 - i);
        cairo_line_to(cr, window_width, window_height / 2 - i);
        cairo_stroke(cr);
    }
    // 文字表示
    cairo_move_to(cr, 5, window_height - 110);
    cairo_set_font_size(cr, 13.0);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    world_pos_t *wpos;
    wpos = get_world_pos();
    cairo_show_text(cr,
    g_strdup_printf( 
     "x:%5.0lf  y:%5.0lf  theta:%5.2lf  v:%5.0lf  omega:%5.2lf  lduty:%4.0d  rduty:%4.0d  interrupt count:%4.0d",
     wpos->x, wpos->y, wpos->theta, wpos->v, wpos->omega, dv_get_signed_duty(left_mot), dv_get_signed_duty(right_mot), interrupt_count));
    if(machine_checked) {
        // 座標変換
        cairo_translate(cr, window_width / 2.0 - wpos->y / scale, window_height / 2.0 - wpos->x / scale);
        cairo_rotate(cr, -wpos->theta);
        // マシン描画
        cairo_set_line_width(cr, MACHINE_SIZE / 10 / scale);
        cairo_rectangle(cr, -MACHINE_SIZE / 2 / scale, -MACHINE_SIZE / 2 / scale, MACHINE_SIZE / scale, MACHINE_SIZE / scale);
        cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);
        cairo_fill_preserve(cr);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_stroke(cr);
        cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
        cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_move_to(cr, -MACHINE_SIZE / 2 / scale, 0);
        cairo_line_to(cr, 0, -MACHINE_SIZE / 2 / scale);
        cairo_line_to(cr, MACHINE_SIZE / 2 / scale, 0);
        cairo_stroke(cr);
        cairo_destroy(cr);
    }
    // 軌道描画
    if(orbit_checked) {
        g_list_foreach(list, (GFunc)draw_orbit_list, (gpointer)cr_orbit);
    }
    cairo_set_source_rgb(cr_orbit, 0.0, 0.0, 1.0);
    cairo_stroke(cr_orbit);
    cairo_destroy(cr_orbit);

    return FALSE;
}

// グラフを再描画する // "expose-event"ハンドラ // 10msec周期で呼ばれる
gboolean draw_graph(GtkWidget *widget, GdkEventExpose *event, gpointer user_data)
{
    int i;
    cairo_t *cr = gdk_cairo_create(widget->window);
    cairo_t *cr_graph = gdk_cairo_create(widget->window);
    GList *tmp_list = NULL;

    UNUSED_VARIABLE(event);

    gtk_window_get_size(GTK_WINDOW(user_data), &window_width, &window_height);

    // 背景描画
    cairo_rectangle(cr, 0, 0, window_width, window_height);
    cairo_set_source_rgb(cr, 0.6, 0.9, 0.9);
    cairo_fill(cr);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    for(i = 0; i <= window_width; i += 25) {
        cairo_move_to(cr, i, 0);
        cairo_line_to(cr, i, window_height);
        if(i % 100) {
            cairo_set_line_width(cr, 0.5);
        } else {
            cairo_set_line_width(cr, 2);
        }
        cairo_stroke(cr);
    }
    for(i = 0; i <= window_height / 2; i += 25) {
        /* canvasの中央に合わせるため-50する */
        cairo_move_to(cr, 0, window_height / 2 - 50 + i);
        cairo_line_to(cr, window_width, window_height / 2 - 50 + i);
        if(i % 100) {
            cairo_set_line_width(cr, 0.5);
        } else {
            cairo_set_line_width(cr, 2);
        }
        cairo_stroke(cr);
        cairo_move_to(cr, 0, window_height / 2 - 50 - i);
        cairo_line_to(cr, window_width, window_height / 2 - 50 - i);
        if(i % 100) {
            cairo_set_line_width(cr, 0.5);
        } else {
            cairo_set_line_width(cr, 2);
        }
        cairo_stroke(cr);
    }
    // 文字表示
    cairo_move_to(cr, 5, window_height - 110);
    cairo_set_font_size(cr, 13.0);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    world_pos_t *wpos;
    wpos = get_world_pos();
    cairo_show_text(cr,
     g_strdup_printf( 
     "x:%5.0lf  y:%5.0lf  theta:%5.2lf  v:%5.0lf  omega:%5.2lf  lduty:%4.0d  rduty:%4.0d  interrupt count:%4.0d",
     wpos->x, wpos->y, wpos->theta, wpos->v, wpos->omega, dv_get_signed_duty(left_mot), dv_get_signed_duty(right_mot), interrupt_count));
    cairo_stroke(cr);
    cairo_destroy(cr);

//    g_list_foreach(list, (GFunc)draw_graph_list, (gpointer)cr_graph);
    for(i = 1; i <= 7; i++) {
        if(graph_checked[i]) {
            for(tmp_list = g_list_first(list); tmp_list != NULL; tmp_list = tmp_list->next) {
                cairo_line_to(cr_graph,
                 ((double*)tmp_list->data)[0], window_height / 2.0 - 50 + ((double*)tmp_list->data)[i] / graph_scale);
            }
            switch(i) {
                case 1:
                    cairo_set_source_rgb(cr_graph, 0.0, 0.2, 0.8); // blue
                    break;
                case 2:
                    cairo_set_source_rgb(cr_graph, 0.0, 0.6, 0.2); // green
                    break;
                case 3:
                    cairo_set_source_rgb(cr_graph, 0.8, 0.0, 0.0); // red
                    break;
                case 4:
                    cairo_set_source_rgb(cr_graph, 1.0, 1.0, 0.4); // yellow
                    break;
                case 5:
                    cairo_set_source_rgb(cr_graph, 0.8, 0.6, 1.0); // purple
                    break;
                case 6:
                    cairo_set_source_rgb(cr_graph, 1.0, 0.6, 0.0); // orange
                    break;
                case 7:
                    cairo_set_source_rgb(cr_graph, 0.8, 0.6, 0.0); // brown
                    break;
            }
            cairo_stroke(cr_graph);
        }
    }
    cairo_destroy(cr_graph);

    return FALSE;
}

void save_pos()
{
    double *p_data;

    p_data = g_new0(double, 8);
    world_pos_t *wpos;
    wpos = get_world_pos();
    p_data[1] = wpos->x;
    p_data[2] = wpos->y;
    p_data[3] = wpos->theta;
    p_data[4] = wpos->v;
    p_data[5] = wpos->omega;
    p_data[6] = get_motor_duty(get_left_motor());
    p_data[7] = get_motor_duty(get_right_motor());
    p_data[0] = interrupt_count;
    list = g_list_append(list, (gpointer)p_data);
}
