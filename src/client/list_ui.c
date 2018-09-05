#include "GUI.h"
/*************************************************************************
	> File Name: main.c
	> Author: 程苗苗
	> Mail: 
	> Created Time: Mon 03 Sep 2018 07:29:03 PM CST
 ************************************************************************/
void load_main_window(char *username,int avatar_id,Entity *friends_list,int friend_num,Entity *groups_list,int group_num)
{
    //===========================主界面main_window=============================
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(main_window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_default_size (GTK_WINDOW(main_window),320,700);
    gtk_window_set_position(GTK_WINDOW(main_window),GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(main_window),"Rosetta");
    gtk_window_set_opacity(GTK_WINDOW(main_window), 0.95);       // 设置透明度函数
    //设置背景图片
    change_background(main_window, 320, 700, "./imgs/bg/01.png");
    //==========================主分割main_box===========================================
    GtkWidget *main_box = gtk_vbox_new(FALSE,0);
            //=================user_information
            GtkWidget *main_box_user_information = gtk_hbox_new(FALSE,0);
                 //!!===========个人信息哦
                char avatar_dir[20];
                sprintf(avatar_dir, "./imgs/avatars/%02d.png", avatar_id);
                GtkWidget *main_img_avatar = gtk_image_new_from_file(avatar_dir);
                gtk_box_pack_start(GTK_CONTAINER(main_box_user_information),main_img_avatar,FALSE,FALSE,20);
                GtkWidget *main_label_username = gtk_label_new(username);
                 gtk_box_pack_start(GTK_CONTAINER(main_box_user_information),main_label_username,FALSE,FALSE,20);
                
            //user_information
            gtk_box_pack_start(GTK_BOX(main_box),main_box_user_information,FALSE,FALSE,5);
         //====================搜索好友
            GtkWidget *main_search_entry = gtk_entry_new();
            gtk_box_pack_start(GTK_BOX(main_box),main_search_entry,FALSE,FALSE,10);
        //=========================好友们
        GtkWidget *main_scrolledwindow = gtk_scrolled_window_new(NULL,NULL);
        gtk_scrolled_window_set_policy(main_scrolledwindow,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC); 
            GtkWidget *main_viewport = gtk_viewport_new(NULL,NULL);/*创建文本视图构件*/
                    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(main_scrolledwindow),main_viewport);
                GtkWidget *box_div = gtk_vbox_new(FALSE,0);
                    GtkWidget *friends_expander = gtk_expander_new("my friends");
                        GtkWidget *friends_listbox = gtk_vbox_new(FALSE,0) ;
                        gtk_container_add(GTK_CONTAINER(friends_expander),friends_listbox);
                            for(int i=0;i<friend_num;i++)
                            {
                                GtkWidget *eventbox = gtk_event_box_new();
                                GtkWidget *boxinfo = gtk_hbox_new(FALSE,0);
                                gtk_container_add(GTK_CONTAINER(eventbox), boxinfo);
                                 char avatar_dir_now[20];
                                 sprintf(avatar_dir_now, "./imgs/avatars/%02d.png", friends_list[i].avatar_id);
                                GtkWidget *img = gtk_image_new_from_file(avatar_dir_now);
                                gtk_box_pack_start(GTK_BOX(boxinfo),img,FALSE,FALSE,10);
                                GtkWidget *username_lable = gtk_label_new(friends_list[i].nickname);
                                gtk_box_pack_start(GTK_BOX(boxinfo),username_lable,FALSE,FALSE,0);
                                gtk_box_pack_start(GTK_BOX(friends_listbox),eventbox,FALSE,FALSE,3);
                                g_signal_connect(G_OBJECT(eventbox), "button_press_event", G_CALLBACK(on_click_friend), friends_list[i].id);
                             }
                        gtk_box_pack_start(GTK_BOX(box_div),friends_expander,FALSE,FALSE,0);
                    GtkWidget *groups_expander = gtk_expander_new("my groups");
                        GtkWidget *groups_listbox = gtk_vbox_new(FALSE,0);
                        gtk_container_add(GTK_CONTAINER(groups_expander),groups_listbox);
                                for(int i=0;i<friend_num;i++)
                            {
                                GtkWidget *eventbox = gtk_event_box_new();
                                GtkWidget *boxinfo = gtk_hbox_new(FALSE,0);
                                gtk_container_add(GTK_CONTAINER(eventbox), boxinfo);
                                char avatar_dir_now[20];
                                sprintf(avatar_dir_now, "./imgs/avatars/%02d.png", groups_list[i].avatar_id);
                                GtkWidget *img = gtk_image_new_from_file(avatar_dir_now);
                                gtk_box_pack_start(GTK_BOX(boxinfo),img,FALSE,FALSE,10);
                                GtkWidget *username_lable = gtk_label_new(groups_list[i].nickname);
                                gtk_box_pack_start(GTK_BOX(boxinfo),username_lable,FALSE,FALSE,0);
                                gtk_box_pack_start(GTK_BOX(groups_listbox),eventbox,FALSE,FALSE,3);
                                g_signal_connect(G_OBJECT(eventbox), "button_press_event", G_CALLBACK(on_click_group), groups_list[i].id);
                             }
                        gtk_box_pack_start(GTK_BOX(box_div),groups_expander,FALSE,FALSE,0);
                    gtk_container_add(GTK_CONTAINER(main_viewport),box_div);
        //=============================
        gtk_box_pack_start(GTK_BOX(main_box),main_scrolledwindow,TRUE,TRUE,0);
        GtkWidget *toolbar = gtk_toolbar_new();
            GtkWidget* settings = gtk_tool_button_new_from_stock(GTK_STOCK_PROPERTIES);
            gtk_toolbar_insert(GTK_TOOLBAR(toolbar),settings,0);
        gtk_box_pack_start(GTK_BOX(main_box),toolbar,FALSE,FALSE,0);
    gtk_container_add(GTK_CONTAINER(main_window),main_box );

    gtk_widget_show_all(main_window);

}
