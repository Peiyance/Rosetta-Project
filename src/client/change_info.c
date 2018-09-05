/*************************************************************************
	> File Name: change_info.c
	> Author: 
	> Mail: 
	> Created Time: Wed 05 Sep 2018 09:10:08 PM CST
 ************************************************************************/

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
    
    // gtk_window_set_decorated(signup_window,NULL);
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
    signup_lable_name = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(signup_lable_name),("<span foreground=\"white\">Username</span>"));
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_lable_name,0,5,0,1);
    //============================输入用户名entry===============================
    signup_entry_name = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_entry_name,5,18,0,1);
    //============================输入密码lable================================
    signup_lable_psw = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(signup_lable_psw),("<span foreground=\"white\">Password</span>"));
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_lable_psw,0,5,1,2);
    //============================输入密码entry===============================
    signup_entry_psw = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_entry_psw,5,18,1,2);
    gtk_entry_set_visibility(GTK_ENTRY(signup_entry_psw), FALSE);
   //============================重复输入密码lable================================
    signup_lable_repeat_psw = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(signup_lable_repeat_psw),("<span foreground=\"white\">Confirm</span>"));
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_lable_repeat_psw,0,5,2,3);
    //============================重复输入密码entry===============================
    signup_entry_repeat_psw = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_entry_repeat_psw,5,18,2,3);
    gtk_entry_set_visibility(GTK_ENTRY(signup_entry_repeat_psw), FALSE);
    //g_signal_connect(GTK_OBJECT(signup_entry_repeat_psw), "activate", G_CALLBACK(on_click_signup), entries);
    //==============================注册button===================================
    signup_button_signup = sungtk_button_new_with_image("./imgs/signupbutton.png", 0, 0);  
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_button_signup,0,18,8,10);
    g_signal_connect(G_OBJECT(signup_button_signup), "clicked", G_CALLBACK(on_click_signup), entries);
    //==============================返回button=================================
    
    signup_button_return = sungtk_button_new_with_image("./imgs/returnbutton.png",0 ,0);
    // signup_button_return = gtk_button_new_with_label("return");
    //  gtk_misc_set_alignment(GTK_MISC(signup_button_return),0,0);//设定文本在左方
    // gtk_button_set_relief(signup_button_return,GTK_RELIEF_NONE);
    gtk_table_attach_defaults(GTK_TABLE(main_table),signup_button_return,0,9,3,6);
   // g_signal_connect(G_OBJECT(signup_button_return), "clicked", G_CALLBACK(on_click_signup_return), (gpointer)signup_window);
    //=========================占位符================================
    tmp = gtk_label_new("");
    gtk_table_attach_defaults(GTK_TABLE(main_table),tmp,9,18,3,6);
    tmp = gtk_label_new("");
    gtk_table_attach_defaults(GTK_TABLE(main_table),tmp,0,18,6,8);


    gtk_widget_show_all(signup_window);
}

