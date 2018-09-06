/*************************************************************************
	> File Name: group_members_window.c
	> Author:程苗苗
	> Mail: 
	> Created Time: Thu 06 Sep 2018 10:55:34 AM CST
 ************************************************************************/

#include "GUI.h"
void load_add_group_members_window()
{
    //===========================主界面search_window=============================
    GtkWidget *search_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(search_window),400,600);
    gtk_window_set_position(GTK_WINDOW(search_window),GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(search_window),"group_members");
    gtk_window_set_opacity(GTK_WINDOW(search_window), 0.95);       // 设置透明度函数
    //设置背景图片
    change_background(search_window, 400, 600, "./imgs/bg/01.png");
    //==========================主分割main_box===========================================
    GtkWidget *main_box = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(search_window),main_box);

            gtk_container_set_border_width(GTK_CONTAINER(main_box),40);
        //=========================成员们
        GtkWidget *search_scrolledwindow = gtk_scrolled_window_new(NULL,NULL);
        gtk_scrolled_window_set_policy(search_scrolledwindow,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC); 
             result_main_viewport = gtk_viewport_new(NULL,NULL);/*创建文本视图构件*/
                    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(search_scrolledwindow),result_main_viewport);
                result_show_box = gtk_vbox_new(FALSE,0);
                        gtk_box_pack_start(GTK_BOX(result_show_box),result_main_viewport,FALSE,FALSE,0);

                gtk_container_add(GTK_CONTAINER(result_main_viewport),result_show_box);
        //=============================
        gtk_box_pack_start(GTK_BOX(main_box),search_scrolledwindow,TRUE,TRUE,0);
    

    gtk_widget_show_all(search_window);
}
