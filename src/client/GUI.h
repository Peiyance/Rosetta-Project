#ifndef _GUI_H
#define _GUI_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "sungtk_interface.h"

//login_signup.c
GtkWidget *login_window;
GtkWidget *signup_window;
void change_background(GtkWidget *widget, int w, int h, const gchar *path);
void load_login_window();
void load_signup_window();

//list_ui.c
Entity *myself, *friendlist, *grouplist;
int friend_cnt, group_cnt;
void load_main_window();
void load_group_info();
void load_friend_info();

//settings.c
void load_setting_windows();

//callbacks.c
void msgbox(const char* msg);
void on_click_signup_return(gpointer button, gpointer window);
void on_click_signup(gpointer button, gpointer* entries);	
void on_click_login(gpointer button, gpointer* entries);
void on_click_friend(GtkWidget* widget, GdkEvent* event, Entity* who);
void on_click_group(GtkWidget* widget, GdkEvent* event, Entity* who);


#endif