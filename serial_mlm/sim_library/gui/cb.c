#include "cb.h"

// 10msec周期で呼ばれる
gboolean display_timer(void *arg)
{
    // "expose-event"ハンドラ(draw)を呼ぶ
    if(!stop_interrupt) {
        gtk_widget_queue_draw(((GtkWidget**)arg)[0]);
        gtk_widget_queue_draw(((GtkWidget**)arg)[1]);
        return TRUE;
    } else {
        return FALSE;
    }
}

// "Start/Stop"ボタンコールバック関数
void cb_start(GtkWidget *button, gpointer user_data)
{
    stop_interrupt = 1 - stop_interrupt;
    if(stop_interrupt) {
        gtk_button_set_label(GTK_BUTTON(button), "Start");
    } else {
        gtk_button_set_label(GTK_BUTTON(button), "Stop");
        gtk_timeout_add(gui_cycle, (GtkFunction)display_timer, (gpointer)user_data);
    }
}

// cb_machine
void cb_machine_checked(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        machine_checked = 1;
    } else {
        machine_checked = 0;
    }
    gtk_widget_queue_draw(user_data);
}

// cb_orbit
void cb_orbit_checked(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        orbit_checked = 1;
    } else {
        orbit_checked = 0;
    }
    gtk_widget_queue_draw(user_data);
}

// cb_graph
void cb_graph_x_checked(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        graph_checked[1] = 1;
    } else {
        graph_checked[1] = 0;
    }
    gtk_widget_queue_draw(user_data);
}
void cb_graph_y_checked(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        graph_checked[2] = 1;
    } else {
        graph_checked[2] = 0;
    }
    gtk_widget_queue_draw(user_data);
}
void cb_graph_theta_checked(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        graph_checked[3] = 1;
    } else {
        graph_checked[3] = 0;
    }
    gtk_widget_queue_draw(user_data);
}
void cb_graph_v_checked(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        graph_checked[4] = 1;
    } else {
        graph_checked[4] = 0;
    }
    gtk_widget_queue_draw(user_data);
}
void cb_graph_omega_checked(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        graph_checked[5] = 1;
    } else {
        graph_checked[5] = 0;
    }
    gtk_widget_queue_draw(user_data);
}
void cb_graph_lduty_checked(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        graph_checked[6] = 1;
    } else {
        graph_checked[6] = 0;
    }
    gtk_widget_queue_draw(user_data);
}
void cb_graph_rduty_checked(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        graph_checked[7] = 1;
    } else {
        graph_checked[7] = 0;
    }
    gtk_widget_queue_draw(user_data);
}

// cb_scale
void cb_scale_up(GtkWidget *button, gpointer user_data)
{
    UNUSED_VARIABLE(button);
    scale /= 1.5;
    gtk_widget_queue_draw(user_data);
}

void cb_scale_down(GtkWidget *button, gpointer user_data)
{
    UNUSED_VARIABLE(button);
    UNUSED_VARIABLE(user_data);
    scale *= 1.5;
    gtk_widget_queue_draw(user_data);
}

void cb_graph_scale_up(GtkWidget *button, gpointer user_data)
{
    UNUSED_VARIABLE(button);
    graph_scale /= 2;
    gtk_widget_queue_draw(user_data);
}

void cb_graph_scale_down(GtkWidget *button, gpointer user_data)
{
    UNUSED_VARIABLE(button);
    UNUSED_VARIABLE(user_data);
    graph_scale *= 2;
    gtk_widget_queue_draw(user_data);
}

// cb_fps
void cb_fps_changed(GtkSpinButton *spinbutton, gpointer user_data)
{
    UNUSED_VARIABLE(user_data);
    gui_cycle = 1000 / gtk_spin_button_get_value(spinbutton);
}
