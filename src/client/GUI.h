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
GtkWidget *main_window;
void load_main_window(char *username, int avator_id, Entity *friends_list, int friend_num, Entity *groups_list, int group_num);

//callbacks.c
void on_click_signup_return(gpointer button, gpointer window);
void on_click_signup(gpointer button, gpointer* entries);	
void on_click_login(gpointer button, gpointer* entries);
void on_click_friend(GtkWidget* widget, GdkEvent* event, Entity* who);
void on_click_group(GtkWidget* widget, GdkEvent* event, Entity* who);


#endif