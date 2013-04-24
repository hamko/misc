#ifndef _GUI_INIT_H
#define _GUI_INIT_H

#include <gtk/gtk.h>
// undefしないとエラーになる
#undef FALSE
#undef TRUE
#include "util.h"

void cb_start(void);
void cb_orbit_checked(GtkToggleButton *togglebutton, gpointer user_data);
void cb_machine_checked(GtkToggleButton *togglebutton, gpointer user_data);
void cb_graph_x_checked(GtkToggleButton *togglebutton, gpointer user_data);
void cb_graph_y_checked(GtkToggleButton *togglebutton, gpointer user_data);
void cb_graph_theta_checked(GtkToggleButton *togglebutton, gpointer user_data);
void cb_graph_v_checked(GtkToggleButton *togglebutton, gpointer user_data);
void cb_graph_omega_checked(GtkToggleButton *togglebutton, gpointer user_data);
void cb_graph_lduty_checked(GtkToggleButton *togglebutton, gpointer user_data);
void cb_graph_rduty_checked(GtkToggleButton *togglebutton, gpointer user_data);
void cb_scale_up(GtkWidget *button, gpointer user_data);
void cb_scale_down(GtkWidget *button, gpointer user_data);
void cb_graph_scale_up(GtkWidget *button, gpointer user_data);
void cb_graph_scale_down(GtkWidget *button, gpointer user_data);
void cb_fps_changed(GtkSpinButton *spinbutton, gpointer user_data);
gboolean draw(GtkWidget *widget, GdkEventExpose *event, gpointer user_data);
gboolean draw_graph(GtkWidget *widget, GdkEventExpose *event, gpointer user_data);
gboolean display_timer(void *arg);

#endif //_GUI_INIT_H
