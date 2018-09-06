#ifndef _GUI_H
#define _GUI_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "sungtk_interface.h"

//globals
Entity *myself, *friendlist, *grouplist;
GtkTextBuffer** f_msgbufs, **g_msgbufs;
int friend_cnt, group_cnt;


//login_signup.c
GtkWidget *login_window;
GtkWidget *signup_window;
void change_background(GtkWidget *widget, int w, int h, const gchar *path);
void load_login_window();
void load_signup_window();

//list_ui.c
void load_main_window();
void load_group_info();
void load_friend_info();

//settings.c
void load_setting_windows();

//chat_history_window.c
void history_message_window(GtkWidget* trigger, GtkTextBuffer* buf);

//callbacks.c
int get_friend_local_id(const char* name);
int get_group_local_id(const char* name);
void load_information(GtkWidget* expander, GtkWidget** listbox, Entity* list, int cnt, void cb(GtkWidget* widget, GdkEvent* event, Entity* who));
void msgbox(const char* msg);
void on_click_signup_return(gpointer button, gpointer window);
void on_click_signup(gpointer button, gpointer* entries);	
void on_click_login(gpointer button, gpointer* entries);
void on_click_friend(GtkWidget* widget, GdkEvent* event, Entity* who);
void on_click_group(GtkWidget* widget, GdkEvent* event, Entity* who);
gboolean on_recv_unicast_msg(gpointer data);
gboolean on_recv_multicast_msg(gpointer data);
void search_user(GtkWidget* entry);
void search_friends(GtkWidget* entry);
void delete_user(GtkWidget* entry);

//friends_groups_settings.c
GtkWidget* result_main_viewport, *result_show_box;
void load_add_new_friend_window();

//chat_personal_window.c
void load_chat_personal_window(Entity* who);
void reload_chat_personal_window();
#endif
