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
void load_login_window();
void load_signup_window();

//callbacks.c
void on_click_signup_return(gpointer button, gpointer window);
void on_click_signup(gpointer button, gpointer* entries);	
void on_click_login(gpointer button, gpointer* entries);

#endif