#include "GUI.h"

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
}
