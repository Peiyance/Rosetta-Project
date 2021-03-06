/**************************************************/
/*名称：settings.c
/*描述：设置界面
/*作成日期： 2018-9-5
/*作者：程苗苗
/***************************************************/
#include "GUI.h"
/**************************************************/
/*名称：load_setting_windows
/*描述：加载设置界面
/*作成日期： 2018-9-5
/*参数：无
/*返回值：void
/*作者：程苗苗
/***************************************************/
void  load_setting_windows()
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *menubar;
	GtkWidget *statusbar;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window),50,10);

	GdkScreen *screen = gdk_screen_get_default();
	gint screen_w = gdk_screen_get_width(screen);
	gint screen_h = gdk_screen_get_height(screen);

	gtk_window_move(GTK_WINDOW(window), (screen_w/2), (screen_h/2));
	gtk_window_set_title(GTK_WINDOW(window),"Settings");
    //self-destory automatically when closing window
	g_signal_connect(GTK_WINDOW(window), "delete_event", G_CALLBACK(gtk_widget_destroy), NULL);
    //===========主分割======
	vbox = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),vbox);

	menubar = gtk_menu_bar_new();

	GtkWidget *menu_friend_groups_settings = gtk_menu_new();
	GtkWidget *friends_groups_settings = gtk_menu_item_new_with_label("好友/群设置");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(friends_groups_settings),menu_friend_groups_settings);

	GtkWidget *add_new_friend = gtk_menu_item_new_with_label("添加好友");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_friend_groups_settings),add_new_friend);
	g_signal_connect(G_OBJECT(add_new_friend),"activate",G_CALLBACK(load_add_new_friend_window), search_user);
	GtkWidget *delete_friend = gtk_menu_item_new_with_label("删除好友");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_friend_groups_settings),delete_friend);
	g_signal_connect(G_OBJECT(delete_friend),"activate",G_CALLBACK(load_add_new_friend_window), delete_user);
	GtkWidget *add_new_group = gtk_menu_item_new_with_label("添加群");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_friend_groups_settings),add_new_group);
    // g_signal_connect(G_OBJECT(add_new_group),"activate",G_CALLBACK(load_add_new_friend_window), delete_group);
	GtkWidget *menu_delete_group = gtk_menu_item_new_with_label("删除群");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_friend_groups_settings), menu_delete_group);
	g_signal_connect(G_OBJECT(menu_delete_group),"activate", G_CALLBACK(load_add_new_friend_window), delete_group);
	GtkWidget *menu_create_group = gtk_menu_item_new_with_label("创建群");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_friend_groups_settings),menu_create_group);
	g_signal_connect(G_OBJECT(menu_create_group),"activate",G_CALLBACK(create_group), 3);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),friends_groups_settings);

	GtkWidget *menu_change_interface = gtk_menu_new();
	GtkWidget *change_interface = gtk_menu_item_new_with_label("修改界面");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(change_interface),menu_change_interface);
	GtkWidget *change_bg_img = gtk_menu_item_new_with_label("修改背景图片");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_change_interface),change_bg_img);
  	//g_signal_connect(G_OBJECT(change_bg_img),"activate",G_CALLBACK(chang_theme),1);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),change_interface);

	GtkWidget *menu_change_personal_info=gtk_menu_new();
	GtkWidget *change_personal_info = gtk_menu_item_new_with_label("修改个人信息");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(change_personal_info),menu_change_personal_info);
	GtkWidget *change_avatar = gtk_menu_item_new_with_label("修改头像/昵称/签名");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_change_personal_info),change_avatar);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),change_personal_info);

	GtkWidget *menu_other = gtk_menu_new();
	GtkWidget *other = gtk_menu_item_new_with_label("其他");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(other),menu_other);
	GtkWidget *about= gtk_menu_item_new_with_label("关于");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_other),about);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),other);


	gtk_box_pack_start(GTK_BOX(vbox),menubar,FALSE,FALSE,0);



	gtk_widget_show_all(window);


}
