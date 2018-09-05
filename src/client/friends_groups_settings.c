#include "GUI.h"
/*************************************************************************
	> File Name: main.c
	> Author: 程苗苗
	> Mail: 
	> Created Time: Mon 03 Sep 2018 07:29:03 PM CST
 ************************************************************************/
GtkWidget *main_window, *friends_expander, *groups_expander, *friends_listbox = NULL, *groups_listbox = NULL;

//void load_information(GtkWidget* expander, GtkWidget* listbox, Entity* list, int cnt, int is_friend)

void load_add_new_friend_window()
{
    //===========================主界面search_window=============================
    search_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(search_window),400,600);
    gtk_window_set_position(GTK_WINDOW(search_window),GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(search_window),"Rosetta");
    gtk_window_set_opacity(GTK_WINDOW(search_window), 0.95);       // 设置透明度函数
    //设置背景图片
    change_background(main_window, 400, 600, "./imgs/bg/01.png");
    //==========================主分割main_box===========================================
    GtkWidget *main_box = gtk_vbox_new(FALSE,0);
            //=================user_information
            GtkWidget *main_entry = gtk_entry_new(); 
            gtk_box_pack_start(GTK_BOX(main_box),main_entry,FALSE,FALSE,10);
            gtk_container_set_border(GTK_CONTAINER(main_box),40);
        //=========================好友们
        GtkWidget *search_scrolledwindow = gtk_scrolled_window_new(NULL,NULL);
        gtk_scrolled_window_set_policy(search_scrolledwindow,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC); 
            GtkWidget *main_viewport = gtk_viewport_new(NULL,NULL);/*创建文本视图构件*/
                    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(search_scrolledwindow),main_viewport);
                GtkWidget *show_box = gtk_vbox_new(FALSE,0);
                        gtk_box_pack_start(GTK_BOX(show_box),main_viewport,FALSE,FALSE,0);
                //==========================显示好友
                    load_information(main_viewport,show_box, );   
                gtk_container_add(GTK_CONTAINER(main_viewport),box_div);
        //=============================
        gtk_box_pack_start(GTK_BOX(main_box),main_scrolledwindow,TRUE,TRUE,0);
    

    gtk_widget_show_all(main_window);
}
