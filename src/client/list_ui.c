#include "GUI.h"
/*************************************************************************
	> File Name: main.c
	> Author: 程苗苗
	> Mail: 
	> Created Time: Mon 03 Sep 2018 07:29:03 PM CST
 ************************************************************************/
GtkWidget *main_window, *friends_expander, *groups_expander, *friends_listbox = NULL, *groups_listbox = NULL;
GtkWidget *results_expander = NULL, *results_listbox = NULL;

void refresh_label(GtkExpander* expander, const char* prefix, int cnt){
    static char tmp[40];
    sprintf(tmp, "%s (%d)", prefix, cnt);
    gtk_expander_set_label(expander, tmp);
}

void search_friends(GtkWidget* entry)
{
    char* pattern = gtk_entry_get_text(entry);
    Entity* list = (Entity*)malloc(sizeof(Entity)*(friend_cnt+group_cnt));
    int cnt = 0;
    for(int i = 0; i < friend_cnt; i++){
        if(strstr(friendlist[i].nickname, pattern)){
            list[cnt++] = friendlist[i];
            sprintf(list[cnt-1].nickname, "%s(Friend)", friendlist[i].nickname);
        }
    }
    for(int i = 0; i < group_cnt; i++){
        if(strstr(grouplist[i].nickname, pattern)){
            list[cnt++] = grouplist[i];
            sprintf(list[cnt-1].nickname, "%s(Group)", grouplist[i].nickname);
        }
    }
    load_information(results_expander, &results_listbox, list, cnt, on_click_friend);
    refresh_label(results_expander, "Search Results ", cnt);
}

void load_friend_info(){
    load_information(friends_expander, &friends_listbox, friendlist, friend_cnt, on_click_friend);
    refresh_label(friends_expander, "Friends ", friend_cnt);
}

void load_group_info(){
    load_information(groups_expander, &groups_listbox, grouplist, group_cnt, on_click_group);
    refresh_label(groups_expander, "Groups ", group_cnt);
}

void load_main_window()
{
    char* username = myself->nickname;
    int avatar_id = myself->avatar_id;
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
                char avatar_dir[100];
                sprintf(avatar_dir, "./imgs/avatars/%02d.png", avatar_id);
                GtkWidget *main_img_avatar = gtk_image_new_from_file(avatar_dir);
                gtk_box_pack_start(GTK_CONTAINER(main_box_user_information),main_img_avatar,FALSE,FALSE,20);
                GtkWidget *main_label_username = gtk_label_new(NULL);
                sprintf(avatar_dir, "<span foreground=\"white\">%s</span>", username);
                gtk_label_set_markup(GTK_LABEL(main_label_username), avatar_dir);
                 gtk_box_pack_start(GTK_CONTAINER(main_box_user_information),main_label_username,FALSE,FALSE,20);
                
            //user_information
            gtk_box_pack_start(GTK_BOX(main_box),main_box_user_information,FALSE,FALSE,5);
         //====================搜索好友
            GtkWidget *main_search_entry = gtk_entry_new();
            gtk_box_pack_start(GTK_BOX(main_box),main_search_entry,FALSE,FALSE,10);
            g_signal_connect(GTK_OBJECT(main_search_entry), "activate", G_CALLBACK(search_friends), NULL);
        //=========================好友们
        GtkWidget *main_scrolledwindow = gtk_scrolled_window_new(NULL,NULL);
        gtk_scrolled_window_set_policy(main_scrolledwindow,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC); 
            GtkWidget *main_viewport = gtk_viewport_new(NULL,NULL);/*创建文本视图构件*/
                    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(main_scrolledwindow),main_viewport);
                GtkWidget *box_div = gtk_vbox_new(FALSE,0);
                    results_expander = gtk_expander_new("Search Results (0)");
                        gtk_box_pack_start(GTK_BOX(box_div),results_expander,FALSE,FALSE,0);
                    gtk_expander_set_expanded(results_expander, TRUE);
                    friends_expander = gtk_expander_new("My Friends (0)");
                        gtk_box_pack_start(GTK_BOX(box_div),friends_expander,FALSE,FALSE,0);
                    gtk_expander_set_expanded(friends_expander, TRUE);
                    groups_expander = gtk_expander_new("My Groups (0)");
                        gtk_box_pack_start(GTK_BOX(box_div),groups_expander,FALSE,FALSE,0);
                    gtk_expander_set_expanded(groups_expander, TRUE);
                    gtk_container_add(GTK_CONTAINER(main_viewport),box_div);
        //=============================
        gtk_box_pack_start(GTK_BOX(main_box),main_scrolledwindow,TRUE,TRUE,0);
        GtkWidget *toolbar = gtk_toolbar_new();
            GtkWidget* settings = gtk_tool_button_new_from_stock(GTK_STOCK_PROPERTIES);
            g_signal_connect(G_OBJECT(settings), "clicked", G_CALLBACK(load_setting_windows), NULL);
            gtk_toolbar_insert(GTK_TOOLBAR(toolbar),settings,0);
        gtk_box_pack_start(GTK_BOX(main_box),toolbar,FALSE,FALSE,0);
    gtk_container_add(GTK_CONTAINER(main_window),main_box );

    gtk_widget_show_all(main_window);
}
