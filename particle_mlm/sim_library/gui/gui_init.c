#include "gui_init.h"
#include "gui_params.h"

// ディスプレイ更新周期(=1000/fps)
int gui_cycle; /* msec */

// GUI初期化
void gui_init(void)
{
    GtkWidget *window;
    GtkWidget *notebook;
    GtkWidget *table;
    GtkWidget *inner_table;
    GtkWidget *canvas;
    GtkWidget *graph_canvas;
    GtkWidget *button;
    GtkWidget *label;
    GtkObject *adjustment;
    GtkWidget *spinbutton;
    GtkWidget *hbox;
    GtkWidget **p_canvas;
    
    // init
    /* 本来は *argc, *argvを引数に取るべきだが、main()の外なのでとりあえずNULLをキャストして渡しておく */
    gtk_init((int*)NULL, (char***)NULL);
    // window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "simulator");
    gtk_widget_set_size_request(window, 800, 400);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    /* ウィンドウを閉じたら終了 */
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // page widget
    notebook = gtk_notebook_new();
    // table
    table = gtk_table_new(2, 5, FALSE);
    gtk_container_add(GTK_CONTAINER(window), table);
        // notebook
        gtk_table_attach(GTK_TABLE(table), notebook, 0, 5, 0, 1,
         GTK_EXPAND | GTK_FILL | GTK_SHRINK,
         GTK_EXPAND | GTK_FILL | GTK_SHRINK, 2, 2);
            // inner table
            inner_table = gtk_table_new(2, 5, FALSE);
            // label
            label = gtk_label_new("field & machine");
            gtk_notebook_append_page(GTK_NOTEBOOK(notebook), inner_table, label);
                // canvas
                /* display */
                canvas = gtk_drawing_area_new();
                gtk_drawing_area_size(GTK_DRAWING_AREA(canvas), 200, 200);
                gtk_table_attach(GTK_TABLE(inner_table), canvas, 0, 5, 0, 1,
                 GTK_EXPAND | GTK_FILL | GTK_SHRINK,
                 GTK_EXPAND | GTK_FILL | GTK_SHRINK, 2, 2);
                gtk_signal_connect(GTK_OBJECT(canvas), "expose-event", G_CALLBACK(draw), (gpointer)window);
                // check button
                /* "draw machine" */
                button = gtk_check_button_new_with_label("draw machine");
                gtk_table_attach(GTK_TABLE(inner_table), button, 2, 3, 1, 2,
                 GTK_FILL | GTK_SHRINK,
                 GTK_FILL | GTK_SHRINK, 2, 2);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
                g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(cb_machine_checked), (gpointer)canvas);
                /* "draw orbit" */
                button = gtk_check_button_new_with_label("draw orbit");
                gtk_table_attach(GTK_TABLE(inner_table), button, 3, 4, 1, 2,
                 GTK_FILL | GTK_SHRINK,
                 GTK_FILL | GTK_SHRINK, 2, 2);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
                g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(cb_orbit_checked), (gpointer)canvas);
                // hbox
                hbox = gtk_hbox_new(FALSE, 0);
                gtk_table_attach(GTK_TABLE(inner_table), hbox, 4, 5, 1, 2,
                 GTK_FILL | GTK_SHRINK,
                 GTK_FILL | GTK_SHRINK, 2, 2);
                    // label
                    label = gtk_label_new("scale :");
                    gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);
                    // button
                    /* "Scale Up" */
                    button = gtk_button_new_with_label("↑");
                    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
                    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(cb_scale_up), (gpointer)canvas);
                    /* "Scale Down" */
                    button = gtk_button_new_with_label("↓");
                    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
                    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(cb_scale_down), (gpointer)canvas);
            // inner table
            inner_table = gtk_table_new(3, 5, FALSE);
            // label
            label = gtk_label_new("graph");
            gtk_notebook_append_page(GTK_NOTEBOOK(notebook), inner_table, label);
                // canvas
                /* display */
                graph_canvas = gtk_drawing_area_new();
                gtk_drawing_area_size(GTK_DRAWING_AREA(graph_canvas), 200, 200);
                gtk_table_attach(GTK_TABLE(inner_table), graph_canvas, 0, 5, 0, 1,
                 GTK_EXPAND | GTK_FILL | GTK_SHRINK,
                 GTK_EXPAND | GTK_FILL | GTK_SHRINK, 2, 2);
                gtk_signal_connect(GTK_OBJECT(graph_canvas), "expose-event", G_CALLBACK(draw_graph), (gpointer)window);
                // check button
                /* "x" */
                button = gtk_check_button_new_with_label("x (blue)");
                gtk_table_attach(GTK_TABLE(inner_table), button, 0, 1, 1, 2,
                 GTK_FILL | GTK_SHRINK,
                 GTK_FILL | GTK_SHRINK, 2, 2);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
                g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(cb_graph_x_checked), (gpointer)graph_canvas);
                /* "y" */
                button = gtk_check_button_new_with_label("y (green)");
                gtk_table_attach(GTK_TABLE(inner_table), button, 1, 2, 1, 2,
                 GTK_FILL | GTK_SHRINK,
                 GTK_FILL | GTK_SHRINK, 2, 2);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
                g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(cb_graph_y_checked), (gpointer)graph_canvas);
                /* "theta" */
                button = gtk_check_button_new_with_label("theta (red)");
                gtk_table_attach(GTK_TABLE(inner_table), button, 2, 3, 1, 2,
                 GTK_FILL | GTK_SHRINK,
                 GTK_FILL | GTK_SHRINK, 2, 2);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
                g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(cb_graph_theta_checked), (gpointer)graph_canvas);
                /* "v" */
                button = gtk_check_button_new_with_label("v (yellow)");
                gtk_table_attach(GTK_TABLE(inner_table), button, 3, 4, 1, 2,
                 GTK_FILL | GTK_SHRINK,
                 GTK_FILL | GTK_SHRINK, 2, 2);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
                g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(cb_graph_v_checked), (gpointer)graph_canvas);
                /* "omega" */
                button = gtk_check_button_new_with_label("omega (purple)");
                gtk_table_attach(GTK_TABLE(inner_table), button, 4, 5, 1, 2,
                 GTK_FILL | GTK_SHRINK,
                 GTK_FILL | GTK_SHRINK, 2, 2);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
                g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(cb_graph_omega_checked), (gpointer)graph_canvas);
                /* "lduty" */
                button = gtk_check_button_new_with_label("lduty (orange)");
                gtk_table_attach(GTK_TABLE(inner_table), button, 0, 1, 2, 3,
                 GTK_FILL | GTK_SHRINK,
                 GTK_FILL | GTK_SHRINK, 2, 2);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
                g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(cb_graph_lduty_checked), (gpointer)graph_canvas);
                /* "rduty" */
                button = gtk_check_button_new_with_label("rduty (brown)");
                gtk_table_attach(GTK_TABLE(inner_table), button, 1, 2, 2, 3,
                 GTK_FILL | GTK_SHRINK,
                 GTK_FILL | GTK_SHRINK, 2, 2);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
                g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(cb_graph_rduty_checked), (gpointer)graph_canvas);
                // hbox
                hbox = gtk_hbox_new(FALSE, 0);
                gtk_table_attach(GTK_TABLE(inner_table), hbox, 4, 5, 2, 3,
                 GTK_FILL | GTK_SHRINK,
                 GTK_FILL | GTK_SHRINK, 2, 2);
                    // label
                    label = gtk_label_new("scale :");
                    gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);
                    // button
                    /* "Scale Up" */
                    button = gtk_button_new_with_label("↑");
                    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
                    g_signal_connect(G_OBJECT(button), "clicked", 
                     G_CALLBACK(cb_graph_scale_up), (gpointer)graph_canvas);
                    /* "Scale Down" */
                    button = gtk_button_new_with_label("↓");
                    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
                    g_signal_connect(G_OBJECT(button), "clicked", 
                     G_CALLBACK(cb_graph_scale_down), (gpointer)graph_canvas);
        // hbox
        hbox = gtk_hbox_new(FALSE, 0);
        gtk_table_attach(GTK_TABLE(table), hbox, 1, 2, 2, 3,
         GTK_FILL | GTK_SHRINK,
         GTK_FILL | GTK_SHRINK, 2, 2);
            // label
            label = gtk_label_new("fps :");
            gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);
            // spinbutton
            adjustment = gtk_adjustment_new(25, 5, 100, 5, 0, 0);
            spinbutton = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment), 1, 0);
            gtk_box_pack_start(GTK_BOX(hbox), spinbutton, TRUE, TRUE, 0);
            g_signal_connect(G_OBJECT(spinbutton), "value-changed", G_CALLBACK(cb_fps_changed), NULL);
        // button
        /* "Start" */
        button = gtk_button_new_with_label("Start");
        gtk_table_attach(GTK_TABLE(table), button, 2, 3, 2, 3,
         GTK_FILL | GTK_SHRINK,
         GTK_FILL | GTK_SHRINK, 2, 2);
        p_canvas = g_new0(GtkWidget*, 2);
        p_canvas[0] = canvas;
        p_canvas[1] = graph_canvas;
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(cb_start), (gpointer)p_canvas);
        /* "Save" */
        button = gtk_button_new_with_mnemonic("_Save");
        gtk_table_attach(GTK_TABLE(table), button, 3, 4, 2, 3,
         GTK_FILL | GTK_SHRINK,
         GTK_FILL | GTK_SHRINK, 2, 2);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
        /* "Quit" */
        button = gtk_button_new_with_mnemonic("_Quit");
        gtk_table_attach(GTK_TABLE(table), button, 4, 5, 2, 3,
         GTK_FILL | GTK_SHRINK,
         GTK_FILL | GTK_SHRINK, 2, 2);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    // タイマ
    gui_cycle = 1000 / gtk_spin_button_get_value((GtkSpinButton*)spinbutton);
    /* "Start"ボタンコールバックの中で登録する */
//    gtk_timeout_add(gui_cycle, (GtkFunction)display_timer, (gpointer)canvas);
    // 描画
    gtk_widget_show_all(window);
}
