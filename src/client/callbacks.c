/**************************************************/
/*名称：callbacks.c
/*描述：实现客户端逻辑的回调函数
/*作成日期：2018-9-5
/*作者：zy
/***************************************************/

#include "GUI.h"
#include "connector.h"
/**************************************************/
/*名称：load_information
/*描述：将list中的数据显示在存放列表的容器中
/*作成日期： 2018-9-5
/*参数：
        参数1：expander、GtkWidget*、输入、存放列表的容器
        参数2：listbox、GtkWidget** 、输入、列表
        参数3：list、Entity *、输入、列表中存在的实体
	参数4：cnt、int、输入、列表中实体的个数
	参数5：cb(GtkWidget* widget, GdkEvent* event, Entity* who)、回调函数指针、输入、列表中每个控件的回调函数
/*返回值：void
/*作者：zy
/***************************************************/
void load_information(GtkWidget* expander, GtkWidget** listbox, Entity* list, int cnt, void cb(GtkWidget* widget, GdkEvent* event, Entity* who))
{
    if(*listbox){
        gtk_widget_destroy(*listbox);
        printf("removed\n");
    }
    *listbox = gtk_vbox_new(FALSE,0) ;
    gtk_container_add(GTK_CONTAINER(expander), *listbox);
    for(int i = 0; i < cnt; i++)
    {
        GtkWidget *eventbox = gtk_event_box_new();
        GtkWidget *boxinfo = gtk_hbox_new(FALSE,0);
        gtk_container_add(GTK_CONTAINER(eventbox), boxinfo);
        char avatar_dir_now[40];
        sprintf(avatar_dir_now, "./imgs/avatars/%02d.png", list[i].avatar_id);
        GtkWidget *img = gtk_image_new_from_file(avatar_dir_now);
        gtk_box_pack_start(GTK_BOX(boxinfo),img,FALSE,FALSE,10);
        GtkWidget *username_lable = gtk_label_new(list[i].nickname);
        gtk_box_pack_start(GTK_BOX(boxinfo),username_lable,FALSE,FALSE,0);
        gtk_box_pack_start(GTK_BOX(*listbox),eventbox,FALSE,FALSE,3);
        g_signal_connect(G_OBJECT(eventbox), "button_press_event", G_CALLBACK(cb), &list[i]);
    }
    gtk_widget_show_all(expander);
}
/**************************************************/
/*名称：msgbox
/*描述：显示出一个窗口，窗口中显示需要显示的字符
/*作成日期： 2018-9-5
/*参数：
        参数1：msg、const char*、输入、需要显示的字符串
/*返回值：void
/*作者：zy
/***************************************************/
void msgbox(const char* msg)
{
	GtkWidget* pop = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget* label = gtk_label_new(msg);
	gtk_container_add(GTK_CONTAINER(pop), label);
	gtk_window_set_position(GTK_WINDOW(pop), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(pop), 300, 200);
	g_signal_connect(G_OBJECT(pop), "activate_default", G_CALLBACK(gtk_widget_destroy), NULL);
	g_signal_connect(G_OBJECT(pop), "delete_event", G_CALLBACK(gtk_widget_destroy), NULL);
	gtk_widget_show_all(pop);
}
/**************************************************/
/*名称：get_friend_local_id
/*描述：获取好友在本地的编号
/*作成日期： 2018-9-5
/*参数：
        参数1：name、const char*、输入、好友的名字
/*返回值：int类型 -1表示不存在这个用户，否则返回用户的id
/*作者：zy
/***************************************************/
int get_friend_local_id(const char* name){
	for(int i = 0; i < friend_cnt; i++){
		if(strcmp(friendlist[i].nickname, name) == 0){
			return i;
		}
	}
	return -1;
}

/**************************************************/
/*名称：get_friend_local_id
/*描述：获取群组在本地的编号
/*作成日期： 2018-9-5
/*参数：
        参数1：name、const char*、输入、群组的名字
/*返回值：int类型 -1表示不存在这个用户，否则返回用户的id
/*作者：zy
/***************************************************/
int get_group_local_id(const char* name){
	for(int i = 0; i < group_cnt; i++){
		if(strcmp(grouplist[i].nickname, name) == 0){
			return i;
		}
	}
	return -1;
}
/**************************************************/
/*名称：cb_contacts
/*描述：获取好友列表之后的回调函数
/*作成日期： 2018-9-5
/*参数：
        参数1：好友列表信息, 指针, 顺次指向一个int, 随后是struct Entity
/*返回值：类型为gboolean
/*作者：zy
/***************************************************/
gboolean cb_contacts(gpointer data)
{
	// unload
	for(int i = 0; i < friend_cnt; i++){
		gtk_object_destroy(f_msgbufs[i]);
	}
	free(f_msgbufs);

	friend_cnt = *(int*)data;
	friendlist = (Entity*)(data+4);
	f_msgbufs = (GtkTextBuffer**) malloc(sizeof(void*)*friend_cnt);
	for(int i = 0; i < friend_cnt; i++)
		f_msgbufs[i] = gtk_text_buffer_new(NULL);
	reload_chat_personal_window();

	load_friend_info();
}
/**************************************************/
/*名称：cb_groups
/*描述：获取群组列表之后的回调函数
/*作成日期： 2018-9-5
/*参数：
        参数1：群组列表信息, 指针, 顺次指向一个int, 随后是struct Entity
/*返回值：类型为gboolean
/*作者：zy
/***************************************************/
gboolean cb_groups(gpointer data)
{
	//unload
	for(int i = 0; i < group_cnt; i++){
		gtk_object_destroy(g_msgbufs[i]);
	}
	free(g_msgbufs);

	group_cnt = *(int*)data;
	grouplist = (Entity*)(data+4);
	g_msgbufs = (GtkTextBuffer**) malloc(sizeof(void*)*group_cnt);
	for(int i = 0; i < group_cnt; i++)
		g_msgbufs[i] = gtk_text_buffer_new(NULL);

	load_group_info();
}
/**************************************************/
/*名称：cb_auth
/*描述：登陆验证完成之后的回调函数
/*作成日期： 2018-9-5
/*参数：
        参数1：若成功则为指向已登陆用户的个人信息指针,  否则为null
/*返回值：类型为gboolean
/*作者：zy
/***************************************************/
gboolean cb_auth(gpointer data)
{
	if(data) {
		//success
		gtk_widget_destroy(login_window);
		myself = (Entity*) data;
		sleep(0.3);
		load_main_window();
		req_contacts(myself->nickname, cb_contacts);		
		req_groups(myself->nickname, cb_groups);
	}else{
		//error
		msgbox("验证失败，请检查用户名和密码!");
	}
	return FALSE;
}
/**************************************************/
/*名称：cb_reg
/*描述：注册完成之后的回调函数
/*作成日期： 2018-9-5
/*参数：
        参数1：整数, 1: success, 0: fail
/*返回值：类型为gboolean
/*作者：zy
/***************************************************/
gboolean cb_reg(gpointer data)
{
	if(data){
		//success
		msgbox("注册成功，请登陆!");
		gtk_widget_destroy(signup_window);

	}else{
		msgbox("REGIESTER FAILED!");
	}
	return FALSE;
}
/**************************************************/
/*名称：on_click_****
/*描述：GUI点击事件的回调
/*作成日期： 2018-9-5
/*参数：
        参数1：触发控件
	参数2: 附带参数
/*返回值：类型为void
/*作者：zy
/***************************************************/
void on_click_signup_return(gpointer button, gpointer window)
{
	gtk_widget_destroy(window); 
}

void on_click_signup(gpointer button, gpointer* entries)
{
	gchar* pwd = gtk_entry_get_text(entries[1]), *pwd2 = gtk_entry_get_text(entries[2]);
	if(strcmp(pwd, pwd2) != 0){
		msgbox("两次输入不一致!");
		return;
	}
	gchar* username = gtk_entry_get_text(entries[0]);
	if(strlen(username) >= 30){
		msgbox("Username is too long!\n");
		return;
	}
	req_register(username, pwd, cb_reg);
}

void on_click_login(gpointer button, gpointer* entries)
{
	gchar* username = gtk_entry_get_text(entries[0]), *pwd = gtk_entry_get_text(entries[1]);
	req_authentication(username, pwd, cb_auth);
}

void on_click_friend(GtkWidget* widget, GdkEvent* event, Entity* who)
{
	load_chat_personal_window(who);
}

void on_click_group(GtkWidget* widget, GdkEvent* event, Entity* who)
{
	load_chat_group_window(who);
}
/**************************************************/
/*名称：on_recv_xxx_msg
/*描述：获取到消息之后的回调函数
/*作成日期： 2018-9-5
/*参数：
        参数1：若成功则为指向已登陆用户的个人信息指针,  否则为null
/*返回值：类型为gboolean
/*作者：zy
/***************************************************/
gboolean on_recv_unicast_msg(gpointer data)
{
	char* raw = (char*) data;
	static char username[100], msg[1024];
	sscanf(raw, "%s:%s", username, msg);
	int uid = get_friend_local_id(username);
	if(uid == -1) {
		printf("Received strange username [%s] which is not recorded locally\n", username);
		printf("This echo comes from [callback.c : line.260]\n");
		return FALSE;
	}
	update_buffer(f_msgbufs[uid], msg, username);
	return FALSE;
}

gboolean on_recv_multicast_msg(gpointer data)
{
	char* raw = (char*) data;
	static char groupname[100], username[100], msg[1024];
	sscanf(raw, "%s:%s:%d", groupname, username, msg);
	int gid = get_group_local_id(groupname);
	if(gid == -1){
		printf("Received strange group name [%s] which is not recorded locally\n", groupname);
		printf("This echo comes from [callback.c : line 275]\n");
		return FALSE;
	}
	update_buffer(g_msgbufs[gid], msg, username);
	return FALSE;
}

/**************************************************/
/*名称：cb_list_operation
/*描述：好友/群组列表更新之后的回调函数
/*作成日期： 2018-9-5
/*参数：
        参数1：更新结果, 1为成功, 0为失败
/*返回值：类型为gboolean
/*作者：zy
/***************************************************/

gboolean cb_list_operation(gpointer res){
	if(res){
		msgbox("Operation Success!");
		req_contacts(myself->nickname, cb_contacts);
		req_groups(myself->nickname, cb_groups);
	}else{
		msgbox("Operation Failed!");
	}
	return FALSE;
}
/**************************************************/
/*名称：on_click_add/del_friend/group
/*描述：点击添加/删除好友/Group之后的回调函数
/*作成日期： 2018-9-5
/*参数：
        参数1,2：事件默认参数
	参数3  : 指向用户结构体的指针
/*返回值：类型为gboolean
/*作者：zy
/***************************************************/
void on_click_add_friend(GtkWidget* widget, GdkEvent* event, Entity* who){
	req_add_contacts(myself->nickname, who->nickname, cb_list_operation);
}

void on_click_del_friend(GtkWidget* widget, GdkEvent* event, Entity* who){
	req_delete_contacts(myself->nickname, who->nickname, cb_list_operation);
}

void on_click_add_group(GtkWidget* widget, GdkEvent* event, Entity* who){
	// req_add_groups(myself-)
}

void on_click_del_group(GtkWidget* widget, GdkEvent* event, Entity* who){
	// req_quit_group(who->nickname);
}

/**************************************************/
/*名称：cb_search_contacts
/*描述：获取到用户搜索结果后的回调函数
/*作成日期： 2018-9-5
/*参数：
        参数1：指针, 顺次指向一个int为记录条数, 随后是struct Entity数组
/*返回值：类型为gboolean
/*作者：zy
/***************************************************/

gboolean cb_search_contacts(gpointer data)
{
	int cnt = *(int*)data;
	Entity* list = (Entity*)(data+4);
	load_information(result_main_viewport, &result_show_box, list, cnt, on_click_add_friend);
	return FALSE;
}

/**************************************************/
/*名称：search_user/delete_user
/*描述：点击好友操作后的响应函数
/*作成日期： 2018-9-5
/*参数：
        参数1：指向输入框的指针
/*返回值：类型为gboolean
/*作者：zy
/***************************************************/

void search_user(GtkWidget* entry)
{
	char* pattern = gtk_entry_get_text(entry);
	req_search_contacts(pattern, cb_search_contacts);
}

void delete_user(GtkWidget* entry)
{
    char* pattern = gtk_entry_get_text(entry);
    Entity* list = (Entity*)malloc(sizeof(Entity)*friend_cnt);
    int cnt = 0;
    for(int i = 0; i < friend_cnt; i++){
        if(strstr(friendlist[i].nickname, pattern)){
            list[cnt++] = friendlist[i];
        }
    }
    load_information(result_main_viewport, &result_show_box, list, cnt, on_click_del_friend);
    free(list);
}

gboolean cb_search_groups(gpointer data)
{
	int cnt = *(int*)data;
	Entity* list = (Entity*)(data+4);
	load_information(result_main_viewport, &result_show_box, list, cnt, on_click_add_group);
}

/**************************************************/
/*名称：search_user/delete_user
/*描述：点击 Group 操作后的响应函数
/*作成日期： 2018-9-6
/*参数：
        参数1：指向输入框的指针
/*返回值：类型为gboolean
/*作者：zy
/***************************************************/

void search_group(GtkWidget* entry)
{
	char* pattern = gtk_entry_get_text(entry);
	// req_search_groups(pattern, cb_seach_groups);
}

void delete_group(GtkWidget* entry)
{
    char* pattern = gtk_entry_get_text(entry);
    Entity* list = (Entity*)malloc(sizeof(Entity)*group_cnt);
    int cnt = 0;
    for(int i = 0; i < group_cnt; i++){
	    if(strstr(grouplist[i].nickname, pattern)){
	        list[cnt++] = grouplist[i];
	    }
	}
	load_information(result_main_viewport, &result_show_box, list, cnt, on_click_del_group);
    free(list);
}