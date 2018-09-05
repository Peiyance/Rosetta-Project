/*************************************************************************
	> File Name: main.c
	> Author: 程苗苗 
	> Mail: 
	> Created Time: Mon 03 Sep 2018 12:39:14 PM CST
 ************************************************************************/
#include "GUI.h"

void change_background(GtkWidget *widget, int w, int h, const gchar *path)
{
        //1.允许窗口可以绘图
        gtk_widget_set_app_paintable(widget, TRUE);
        gtk_widget_realize(widget);
 
	/* 更改背景图时，图片会重叠 
	* 这时要手动调用下面的函数，让窗口绘图区域失效，产生窗口重绘制事件（即 expose 事件）。 
	*/ 
        gtk_widget_queue_draw(widget);
        GdkPixbuf *src = gdk_pixbuf_new_from_file(path, NULL);
        GdkPixbuf *dst = gdk_pixbuf_scale_simple(src,w,h,GDK_INTERP_BILINEAR);
 
	/* 创建pixmap图像;  
	* NULL：不需要蒙版;  
	* 123： 0~255，透明到不透明 
	*/
        GdkPixmap *pixmap = NULL;
        gdk_pixbuf_render_pixmap_and_mask(dst,&pixmap,NULL,255);

	// 通过pixmap给widget设置一张背景图，最后一个参数必须为: FASL
        gdk_window_set_back_pixmap(widget->window,pixmap,FALSE);
        
 
        g_object_unref(src);
        g_object_unref(dst);
        g_object_unref(pixmap);
 
        return;
}
//===============================加载登陆页面===========================
void load_login_window()
{    
    // GtkWidget *login_window;
    GtkWidget *main_table;
    GtkWidget *input_box;
    //GtkWidget *login_img_logo;
    GtkWidget *login_entry_name;
    GtkWidget *login_entry_psw;
    GtkWidget *login_lable_name;
    GtkWidget *login_lable_psw;
    GtkWidget *login_button_signup;
    GtkWidget *login_button_return;
    GtkWidget *tmp;
    GtkWidget *login_button_login;
    GtkWidget *login_button_modify_to_signup;
    //=======================创建界面==============================
    login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(login_window,NULL);
    gtk_window_set_default_size (GTK_WINDOW(login_window),533,500);
    gtk_window_set_title(GTK_WINDOW(login_window),"Rosetta");
    gtk_window_set_position(GTK_WINDOW(login_window), GTK_WIN_POS_CENTER);
    gtk_window_set_opacity(GTK_WINDOW(login_window), 0.83);       // 设置透明度函数
    //设置背景图片
   change_background(login_window, 533, 500, "./imgs/login.png");
    //=======================分割界面==============================
    main_table = gtk_table_new(10,18,FALSE);
    gtk_container_add(GTK_CONTAINER(login_window),main_table);
    gtk_container_set_border_width(GTK_CONTAINER(main_table),100); 
    gtk_table_set_row_spacings(GTK_TABLE(main_table),8); 
    gtk_table_set_col_spacings(GTK_TABLE(main_table),8);
    //==================上方界面==================================
    input_box = gtk_table_new(4,1,FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(input_box),20);
     gtk_table_set_row_spacings(GTK_TABLE(input_box),8); 
    gtk_table_attach_defaults(GTK_TABLE(main_table),input_box,0,18,0,4);
    //=========================图片=================================
   // login_img_logo = gtk_image_new_from_file("./imgs/logo.png");
   // if(login_img_logo == NULL)g_print("logo image load failed!\n");
   // gtk_table_attach_defaults(GTK_TABLE(main_table),login_img_logo,0,18,0,4);
    //=========================输入用户名lable================================
    login_lable_name = gtk_label_new("username");
    gtk_misc_set_alignment(GTK_MISC(login_lable_name),0,0);//设定文本在左方
    gtk_table_attach_defaults(GTK_TABLE(input_box),login_lable_name,0,1,0,1);
    //=========================输入用户名entry===============================

    login_entry_name = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(input_box),login_entry_name,0,1,1,2);
    //=========================输入密码lable================================
    login_lable_psw = gtk_label_new("password");
    gtk_misc_set_alignment(GTK_MISC(login_lable_psw), 0, 0);
    gtk_table_attach_defaults(GTK_TABLE(input_box),login_lable_psw,0,1,2,3);
    //=========================输入密码entry===============================
    login_entry_psw = gtk_entry_new();
    GdkColor bg_color;
    GdkColor text_color;
    bg_color.red = 48;bg_color.green = 42;bg_color.blue = 80;
    text_color.red = 255;text_color.green = 255;text_color.blue = 255;
     // gtk_widget_modify_base(login_entry_psw, GTK_STATE_NORMAL, &bg_color);
     // gtk_widget_modify_text (login_entry_psw, GTK_STATE_NORMAL, &text_color);
    gtk_table_attach_defaults(GTK_TABLE(input_box),login_entry_psw,0,1,3,4);
    gtk_entry_set_visibility(GTK_ENTRY(login_entry_psw), FALSE); 
    //==========================登陆button===================================
    //
    login_button_login = sungtk_button_new_with_image("./imgs/loginbutton.png", 0, 0);  
    gtk_table_attach_defaults(GTK_TABLE(main_table),login_button_login,0,18,9,10);
    void** entries = (void**)malloc(sizeof(void*)*2);
    entries[0] = login_entry_name; entries[1] = login_entry_psw;
    g_signal_connect(G_OBJECT(login_button_login), "clicked", G_CALLBACK(on_click_login), (gpointer)entries);
    //===========================注册button=================================
    login_button_signup = sungtk_button_new_with_image("./imgs/login_signupbutton.png", 0, 0);  
    gtk_table_attach_defaults(GTK_TABLE(main_table),login_button_signup,0,9,5,9);
    g_signal_connect(G_OBJECT(login_button_signup), "clicked", G_CALLBACK(load_signup_window), NULL);
    //==============================返回button========================================
     login_button_return= sungtk_button_new_with_image("./imgs/returnbutton.png", 0, 0);  
     gtk_table_attach_defaults(GTK_TABLE(main_table),login_button_return,9,18,5,9);  
     g_signal_connect(G_OBJECT(login_button_return), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    //=========================占位符================================
    
    tmp = gtk_label_new("");
    gtk_widget_set_size_request (tmp,30,30);
    gtk_table_attach_defaults(GTK_TABLE(main_table),tmp,0,18,4,5);


    gtk_widget_show_all(login_window);
}
//=============================加载注册页面==========================
void load_signup_window()
{
    // GtkWidget *signup_window;
    GtkWidget *main_table;
    GtkWidget *signup_img_logo;
    GtkWidget *signup_entry_name;
    GtkWidget *signup_entry_psw;
    GtkWidget *signup_entry_repeat_psw;
    GtkWidget *signup_lable_name;
    GtkWidget *signup_lable_psw;
    GtkWidget *signup_lable_repeat_psw;
    GtkWidget *signup_button_signup;
    GtkWidget *signup_button_return;
    GtkWidget *tmp;
    //==========================创建界面==============================
    signup_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(signup_window),500,533);
   change_background(signup_window, 500, 533, "./imgs/signup.png");
    gtk_window_set_title(GTK_WINDOW(signup_window),"Rosetta"); 
     gtk_window_set_opacity(GTK_WINDOW(signup_window), 0.83);       // 设置透明度函数
    gtk_window_set_position(GTK_WINDOW(signup_window), GTK_WIN_POS_CENTER);
    
     gtk_window_set_decorated(signup_window,NULL);
    //==========================分割界面==============================
    main_table = gtk_table_new(10,18,FALSE);
    gtk_container_add(GTK_CONTAINER(signup_window),main_table);
    gtk_container_set_border_width(GTK_CONTAINER(main_table),100); 
    gtk_table_set_row_spacings(GTK_TABLE(main_table),15); 
    gtk_table_set_col_spacings(GTK_TABLE(main_table),15);
    //==============================图片=================================
    //signup_img_logo =  gtk_image_new_from_file("./imgs/logo_min.png");
    //if(signup_img_logo == NULL)g_print("logo image load failed!\n");
    //gtk_table_attach_defaults(GTK_TABLE(main_table),signup_img_logo,0,18,0,2);
    //=========================输入用户名lable================================
    signup_lable_name = gtk_label_new("username");
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_lable_name,0,5,0,1);
    //============================输入用户名entry===============================
    signup_entry_name = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_entry_name,5,18,0,1);
    //============================输入密码lable================================
    signup_lable_psw = gtk_label_new("password");
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_lable_psw,0,5,1,2);
    //============================输入密码entry===============================
    signup_entry_psw = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_entry_psw,5,18,1,2);
    gtk_entry_set_visibility(GTK_ENTRY(signup_entry_psw), FALSE);
   //============================重复输入密码lable================================
    signup_lable_psw = gtk_label_new("repeat password");
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_lable_psw,0,5,2,3);
    //============================重复输入密码entry===============================
    signup_entry_repeat_psw = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_entry_repeat_psw,5,18,2,3);
    gtk_entry_set_visibility(GTK_ENTRY(signup_entry_repeat_psw), FALSE);
    //==============================注册button===================================
    signup_button_signup = sungtk_button_new_with_image("./imgs/signupbutton.png", 0, 0);  
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_button_signup,0,18,8,10);
    void** entries = (void**)malloc(sizeof(void*)*3); entries[0] = signup_entry_name;
    entries[1] = signup_entry_psw; entries[2] = signup_entry_repeat_psw;
    g_signal_connect(G_OBJECT(signup_button_signup), "clicked", G_CALLBACK(on_click_signup), (gpointer*)entries);
    //==============================返回button=================================
    
    signup_button_return = sungtk_button_new_with_image("./imgs/returnbutton.png",0 ,0);
    // signup_button_return = gtk_button_new_with_label("return");
    //  gtk_misc_set_alignment(GTK_MISC(signup_button_return),0,0);//设定文本在左方
    // gtk_button_set_relief(signup_button_return,GTK_RELIEF_NONE);
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_button_return,0,9,3,6);
    g_signal_connect(G_OBJECT(signup_button_return), "clicked", G_CALLBACK(on_click_signup_return), (gpointer)signup_window);
    //=========================占位符================================
    tmp = gtk_label_new("");
    gtk_table_attach_defaults(GTK_TABLE(main_table),tmp,9,18,3,6);
    tmp = gtk_label_new("");
    gtk_table_attach_defaults(GTK_TABLE(main_table),tmp,0,18,6,8);


    gtk_widget_show_all(signup_window);
}

