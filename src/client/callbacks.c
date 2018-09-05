#include "GUI.h"
#include "connector.h"

void load_information(GtkWidget* expander, GtkWidget* listbox, Entity* list, int cnt, void cb(GtkWidget* widget, GdkEvent* event, Entity* who))
{
    if(listbox){
        gtk_widget_destroy(listbox);
    }
    listbox = gtk_vbox_new(FALSE,0) ;
    gtk_container_add(GTK_CONTAINER(expander), listbox);
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
        gtk_box_pack_start(GTK_BOX(listbox),eventbox,FALSE,FALSE,3);
        g_signal_connect(G_OBJECT(eventbox), "button_press_event", G_CALLBACK(cb), &list[i]);
    }
    gtk_widget_show_all(expander);
}

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

gboolean cb_contacts(gpointer data)
{
	friend_cnt = *(int*)data;
	friendlist = (Entity*)(data+4);
	load_friend_info();
}

gboolean cb_groups(gpointer data)
{
	group_cnt = *(int*)data;
	grouplist = (Entity*)(data+4);
	load_group_info();
}

gboolean cb_auth(gpointer data)
{
	if(data) {
		//success
		gtk_widget_destroy(login_window);
		myself = (Entity*) data;
		load_main_window();
		req_contacts(cb_contacts);		
		// req_groups(cb_groups);
	}else{
		//error
		msgbox("验证失败，请检查用户名和密码!");
	}
	return FALSE;
}

gboolean cb_reg(gpointer data)
{
	if(data){
		//success
		msgbox("注册成功，请登陆");
		gtk_widget_destroy(signup_window);

	}else{
		msgbox("REGIESTER FAILED!");
	}
	return FALSE;
}

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
	msgbox(who->nickname);
}

void on_click_group(GtkWidget* widget, GdkEvent* event, Entity* who)
{
	msgbox(who->nickname);
}

void test_cb(gpointer x, GtkTextView* test)
{
	printf("triggered\n");
	GtkTextBuffer *buf = gtk_text_view_get_buffer(test);
	gtk_text_buffer_insert_at_cursor(buf, "23333\n", -1);
	gtk_widget_show(test);
}