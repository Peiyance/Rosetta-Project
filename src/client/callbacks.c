#include "GUI.h"
#include "connector.h"

void msgbox(const char* msg)
{
	GtkWidget* pop = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget* label = gtk_label_new(msg);
	gtk_container_add(GTK_CONTAINER(pop), label);
	gtk_window_set_position(GTK_WINDOW(pop), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(pop), 300, 200);
	gtk_widget_show_all(pop);
	g_signal_connect(G_OBJECT(pop), "delete_event", G_CALLBACK(gtk_widget_destroy), (gpointer)pop);
}

gboolean cb_auth(gpointer data)
{
	if(data) {
		//success
		msgbox("LOGIN SUCCESS!");
	}else{
		//error
		msgbox("验证失败，请检查用户名和密码!");
	}
}

gboolean cb_reg(gpointer data)
{
	if(data){
		//success
		msgbox("SUCESS!");
	}else{
		msgbox("FAILED!");
	}
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
	req_regiester(username, pwd, cb_reg);
}

void on_click_login(gpointer button, gpointer* entries)
{
	gchar* username = gtk_entry_get_text(entries[0]), *pwd = gtk_entry_get_text(entries[1]);
	req_authentacation(username, pwd, cb_auth);
}
