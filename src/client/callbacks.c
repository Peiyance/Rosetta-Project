#include "GUI.h"
#include "connector.h"

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

gboolean cb_auth(gpointer data)
{
	if(data) {
		//success
		gtk_widget_destroy(login_window);
		// Entity* e = (Entity*) data;
		// ****** test code
		Entity *e = (Entity*) malloc(sizeof(Entity));
		memcpy(e->nickname, "heiheihei\0", sizeof(e->nickname));
		e->avatar_id = 0;
	    Entity *a = (Entity*) malloc(sizeof(Entity)), *b = (Entity*) malloc(sizeof(Entity));
	    a[0].avatar_id = 0;
	    memcpy(a[0].nickname, "test", sizeof(a[0].nickname));
	    b[0].avatar_id = 0;
	    memcpy(b[0].nickname, "test", sizeof(b[0].nickname));
	    // ****** test code end 
	    load_main_window(e->nickname,e->avatar_id,a,1,b,0);	
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
		printf("error!\n");		
		msgbox("两次输入不一致!");
		return;
	}
	gchar* username = gtk_entry_get_text(entries[0]);
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