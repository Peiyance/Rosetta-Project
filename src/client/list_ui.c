/**************************************************/
/*名称：list_ui.c
/*描述：修改个人信息界面部分
/*作成日期： 2018-9-3
/*作者：程苗苗、zy
/***************************************************/
#include "GUI.h"
GtkWidget *main_window, *friends_expander, *groups_expander, *friends_listbox = NULL, *groups_listbox = NULL;
GtkWidget *results_expander = NULL, *results_listbox = NULL; 
GtkWidget *main_img_avatar;
/**************************************************/
/*名称：refresh_label
/*描述：加载历史消息记录部分
/*作成日期： 2018-9-4
/*参数：参数1:expander,GtkExpander*类型,输入,label的容器
		参数2: prefix,const char*类型,输入,代表需要加入的前缀
		参数3：cnt,int 类型,输入,代表个数
/*返回值：void
/*作者：zy
/***************************************************/
void refresh_label(GtkExpander* expander, const char* prefix, int cnt){
	static char tmp[40];
	sprintf(tmp, "%s (%d)", prefix, cnt);
	gtk_expander_set_label(expander, tmp);
}
/**************************************************/
/*名称：search_friends
/*描述：搜索好友逻辑
/*作成日期： 2018-9-4
/*参数：参数1:entry,GtkWidget*类型,输入,搜索框部分
/*返回值：void
/*作者：zy
/***************************************************/
void search_friends(GtkWidget* entry)
{
	char* pattern = gtk_entry_get_text(entry);
	Entity* list = (Entity*)malloc(sizeof(Entity)*(friend_cnt+group_cnt));
	int cnt = 0;
	for(int i = 0; i < friend_cnt; i++){
		if(strstr(friendlist[i].nickname, pattern)){
			list[cnt++] = friendlist[i];
		}
	}
	for(int i = 0; i < group_cnt; i++){
		if(strstr(grouplist[i].nickname, pattern)){
			list[cnt++] = grouplist[i];
		}
	}
	load_information(results_expander, &results_listbox, list, cnt, on_click_friend);
	free(list);
	refresh_label(results_expander, "Search Results ", cnt);
	gtk_expander_set_expanded(results_expander, TRUE);
}
/**************************************************/
/*名称：load_friend_info load_group_info
/*描述：加载好友列表加载群列表
/*作成日期： 2018-9-4
/*参数：无
/*返回值：void
/*作者：zy
/***************************************************/
void load_friend_info(){
	load_information(friends_expander, &friends_listbox, friendlist, friend_cnt, on_click_friend);
	refresh_label(friends_expander, "Friends ", friend_cnt);
}

void load_group_info(){
	load_information(groups_expander, &groups_listbox, grouplist, group_cnt, on_click_group);
	refresh_label(groups_expander, "Groups ", group_cnt);
}
/**************************************************/
/*名称：load_friend_info load_group_info
/*描述：更换头像图片
/*作成日期： 2018-9-4
/*参数：参数1:w ,GtkWidget*,输入,当前所咋的eventbox容器
		参数2:e,GdkEvent,输入,当前事件
		参数3,data,gpointer,无意义
/*返回值：gboolean 回调函数自带返回值
/*作者：程苗苗
/***************************************************/
gboolean reload_img(GtkWidget *w,GdkEvent *e,gpointer data)
{
	myself-> avatar_id = (myself-> avatar_id+1)%5;
	char avatar_dir[100];
	GtkWidget *eventb = gtk_event_box_new();
	sprintf(avatar_dir, "./imgs/avatars/%02d.png",myself-> avatar_id);
	gtk_widget_destroy(main_img_avatar);
	main_img_avatar = gtk_image_new_from_file(avatar_dir);
	gtk_container_add(GTK_CONTAINER(w),main_img_avatar);
	gtk_widget_show(main_img_avatar);
	printf("avtar %d\n",myself->avatar_id);
	return TRUE;

}
/**************************************************/
/*名称：load_main_window
/*描述：加载主列表
/*作成日期： 2018-9-4
/*参数：参数1:无
/*返回值：void
/*作者：程苗苗
/***************************************************/
void load_main_window()
{
	char usern[513];
	strcpy(usern,myself->nickname);
	char* username = usern;
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
    //==========================主分割main_box==================================
    GtkWidget *main_box = gtk_vbox_new(FALSE,0);
    //=================user_information
    GtkWidget *main_box_user_information = gtk_hbox_new(FALSE,0);
    //!!===========个人信息=====================================
    char avatar_dir[100];
    GtkWidget *eventb = gtk_event_box_new();
    sprintf(avatar_dir, "./imgs/avatars/%02d.png", avatar_id);
    main_img_avatar = gtk_image_new_from_file(avatar_dir);
    gtk_container_add(GTK_CONTAINER(eventb),main_img_avatar);
    gtk_box_pack_start(GTK_CONTAINER(main_box_user_information),eventb,FALSE,FALSE,20);
    GtkWidget *main_label_username = gtk_label_new(NULL);
    g_signal_connect(G_OBJECT(eventb),"button-press-event",G_CALLBACK(reload_img),0);
    sprintf(avatar_dir, "<span foreground=\"white\">%s</span>", username);
    gtk_label_set_markup(GTK_LABEL(main_label_username), avatar_dir);
    gtk_box_pack_start(GTK_CONTAINER(main_box_user_information),main_label_username,FALSE,FALSE,20);

	//user_information
    gtk_box_pack_start(GTK_BOX(main_box),main_box_user_information,FALSE,FALSE,5);
    //========================搜索好友========================================
    GtkWidget *main_search_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(main_box),main_search_entry,FALSE,FALSE,10);
    g_signal_connect(GTK_OBJECT(main_search_entry), "activate", G_CALLBACK(search_friends), NULL);
    //=========================好友们=====================================
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

    gtk_box_pack_start(GTK_BOX(main_box),main_scrolledwindow,TRUE,TRUE,0);
    GtkWidget *toolbar = gtk_toolbar_new();
    GtkWidget* settings = gtk_tool_button_new_from_stock(GTK_STOCK_PROPERTIES);
    g_signal_connect(G_OBJECT(settings), "clicked", G_CALLBACK(load_setting_windows), NULL);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar),settings,0);
    gtk_box_pack_start(GTK_BOX(main_box),toolbar,FALSE,FALSE,0);
    gtk_container_add(GTK_CONTAINER(main_window),main_box );

    gtk_widget_show_all(main_window);
}
