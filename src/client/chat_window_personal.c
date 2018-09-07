/********************************************************************************
 * Files         : chat_window_personal.c
 * Description   : to show the chat window
 * Date:         : 2018.09.07
 * Author        : zhouzhi
 ********************************************************************************/
#include "GUI.h"
#include <time.h>
#include <gdk/gdkkeysyms.h>

GtkWidget *main_chat_personal_window = NULL;
GtkWidget *input_text_buffer;
Entity* chatting_with = NULL;

/********************************************************************************
 * Name          : screenshot
 * Description   : to screen
 * Parameter     : area:       需要截图的窗口指针
                   x, y, w, h: 截图的范围
                   path_name:  保存图片的路径名字，
 * Date:         : 2018.09.07
 * Author        : zhouzhi
 ********************************************************************************/
GdkPixbuf *screenshot(GtkWidget *area, int x, int y, int w, int h, char *path_name)
{
    GdkPixbuf *pixbuf = NULL;
    pixbuf = gdk_pixbuf_get_from_drawable(NULL, area->window, NULL,  x, y, 0, 0, w, h);
    // 如果没有获取成功，结束函数
    if(NULL == pixbuf)
    {
        msgbox("gdk_pixbuf_get_from_drawable failed\n");
        return  NULL;
    }

    cairo_surface_t *surface = NULL;
    cairo_t *cr;
    surface = cairo_image_surface_create ( CAIRO_FORMAT_ARGB32, w, h) ;
    cr = cairo_create( surface) ;
    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
    cairo_paint(cr);
    cairo_surface_write_to_png ( surface, path_name ) ;

    cairo_destroy(cr) ;
    cairo_surface_destroy( surface) ;
	msgbox("截图成功！已保存到./shoot");
    return pixbuf;
}

void deal_button(GtkWidget *w, gpointer data)
{
    // 截图，指定窗口区域为(0, 0, 300, 300)，在当前路径保存图片为save.png
    screenshot(GTK_WIDGET(data), 0, 0, 300, 300, "./shoot/save.png");
}


void cb_openfile(GtkWidget* trigger, gint response_id, gpointer data)
{
    if(response_id == -5){
        printf("Triggered File Sent: [path]=%s\n[username]:%s\n", gtk_file_selection_get_filename(trigger), data);
        File_private(gtk_file_selection_get_filename(trigger), data);
    }
    gtk_widget_destroy(trigger);    
}

/********************************************************************************
 * Name          : button_openfile
 * Description   : to open the file window
 * Date:         : 2018.09.07
 * Author        : zhouzhi
 ********************************************************************************/

void button_openfile (GtkWidget * widget, gpointer * data)
{
    GtkWidget *FileSelection;
    FileSelection = gtk_file_selection_new ("选择文件");    /*创建文件选择构件 */
    gtk_file_selection_set_filename (GTK_FILE_SELECTION (FileSelection),"*.jpg *.png *.bmp");
    g_signal_connect(G_OBJECT(FileSelection), "response", G_CALLBACK(cb_openfile), data);
    gtk_widget_show (FileSelection);
    // printf("filename: %s", gtk_file_selection_get_filename(FileSelection));
}

/********************************************************************************
 * Name          : get_time
 * Description   : to get the system time
 * Date:         : 2018.09.07
 * Author        : zhouzhi
 ********************************************************************************/
char *get_time()
{
    time_t *timep = malloc(sizeof(*timep));
    time(timep);
    char *s = ctime(timep);
    s[strlen(s)-1] = '\0'; //delete '\n'
    return s;
}

/********************************************************************************
 * Name          : update_buffer
 * Description   : update the chat window
 * Date:         : 2018.09.07
 * Author        : zhouzhi
 ********************************************************************************/
void update_buffer(GtkTextBuffer *buffer, char *msg, char *username)
{
    char now_time[50];
    sprintf(now_time, "%s (%s)\n", username, get_time());
    gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer), now_time, -1);
    gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer), msg, -1);
}

/********************************************************************************
 * Name          : send_msg_to
 * Description   : to send the message
 * Date:         : 2018.09.07
 * Author        : zhouzhi
 ********************************************************************************/
void send_msg_to(GtkWidget *button, Entity* who)
{
    int id = get_friend_local_id(who->nickname);
    GtkTextIter start,end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(input_text_buffer),&start,&end);
    static char input[500];
    sprintf(input,"%s\n", gtk_text_buffer_get_text(GTK_TEXT_BUFFER(input_text_buffer),&start,&end,FALSE));
    gtk_text_buffer_set_text(input_text_buffer,"",0);
    post_msg_unicast(who->nickname, input);
    update_buffer(f_msgbufs[id], input, myself->nickname);
}

/********************************************************************************
 * Name          : cb_close_window
 * Description   : to close the window
 * Date:         : 2018.09.07
 * Author        : zhouzhi
 ********************************************************************************/
void cb_close_window()
{
    gtk_widget_destroy(main_chat_personal_window);
    main_chat_personal_window = NULL;
    chatting_with = NULL;
}

/********************************************************************************
 * Name          : load_chat_personal_window
 * Description   : to load the main chat window for friends
 * Date:         : 2018.09.07
 * Author        : zhouzhi
 ********************************************************************************/
void load_chat_personal_window(Entity* who)
{
    int id = get_friend_local_id(who->nickname);
    chatting_with = who;
    GtkWidget *main_box;
    GtkWidget *friend_information_box;
    GtkWidget *friend_table;
    GtkWidget *friend_image;
    GtkWidget *friend_name_label;
    GtkWidget *friend_geqian_label;
    GtkWidget *chat_view_box;
    GtkWidget *chat_hpaned_window;
    GtkWidget *show_msg_scrolled_window;
    GtkWidget *msg_text;
    GtkWidget *chat_text_view;
    GtkWidget *under_tool_box;
    GtkWidget *smile_button;
    GtkWidget *file_button;
    GtkWidget *chat_history_button;
    GtkWidget *send_msg_box;
    GtkWidget *send_msg_button;
	GtkWidget *cutting_button;
	GtkWidget *game_button;
    GtkWidget *input_msg_box;
    GtkWidget *input_msg_text;
    GtkWidget *input_msg_scrolled_window;
    GtkWidget *the_last_box;

    if(main_chat_personal_window){
        gtk_widget_destroy(main_chat_personal_window);
    }

    //创建界面
    main_chat_personal_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(main_chat_personal_window),600,500);
    gtk_window_set_title(GTK_WINDOW(main_chat_personal_window), who->nickname);
    g_signal_connect(GTK_OBJECT(main_chat_personal_window), "delete_event", G_CALLBACK(cb_close_window), NULL);
    change_background(main_chat_personal_window,600,500,"imgs/bg/04.jpg");

    //上层界面
    main_box = gtk_vbox_new(FALSE, 0);
    under_tool_box = gtk_hbox_new(FALSE, 0);
    input_msg_box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(main_chat_personal_window), main_box);
    friend_information_box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_box),friend_information_box,FALSE,FALSE,0);

    friend_table = gtk_table_new(5,5,FALSE);
    gtk_box_pack_start(GTK_BOX(friend_information_box),friend_table,FALSE,FALSE,0);
    char* imgdir[100];
    sprintf(imgdir, "./imgs/avatars/%02d.png", who->avatar_id);
    friend_image = gtk_image_new_from_file (imgdir);
    gtk_table_attach(GTK_TABLE(friend_table),friend_image,1,3,1,5,GTK_FILL,GTK_FILL,0,0);

    //朋友信息
    friend_name_label = gtk_label_new(who->nickname);
    gtk_table_attach(GTK_TABLE(friend_table),friend_name_label,4,5,1,3,GTK_FILL,GTK_FILL,0,0);

    friend_geqian_label = gtk_label_new("MOTTO");
    gtk_table_attach(GTK_TABLE(friend_table),friend_geqian_label,4,5,4,5,GTK_FILL,GTK_FILL,0,0);

    chat_view_box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_box),chat_view_box,FALSE,FALSE,0);

    show_msg_scrolled_window = gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(show_msg_scrolled_window, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    //显示信息框
    gtk_box_pack_start(GTK_BOX(chat_view_box),show_msg_scrolled_window,TRUE,TRUE,0);
    gtk_widget_show(show_msg_scrolled_window);
    msg_text = gtk_text_view_new_with_buffer(f_msgbufs[id]);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(show_msg_scrolled_window),msg_text);
    gtk_widget_set_size_request(show_msg_scrolled_window, 400, 300);
    gtk_text_view_set_editable(msg_text, FALSE);


    //功能栏
    gtk_box_pack_start(GTK_BOX(main_box),under_tool_box,FALSE,FALSE,0);

    smile_button = sungtk_button_new_with_image("./imgs/IMSmallToolbarFace.ico", 0, 0);
    gtk_box_pack_start(GTK_BOX(under_tool_box),smile_button,FALSE,FALSE,5);
	

    file_button = sungtk_button_new_with_image("./imgs/IMBigToolbarSendFile.ico", 0, 0);
    gtk_box_pack_start(GTK_BOX(under_tool_box),file_button,FALSE,FALSE,5);
	g_signal_connect (G_OBJECT (file_button), "clicked", G_CALLBACK (button_openfile), who->nickname);

	cutting_button = sungtk_button_new_with_image("./imgs/IMSmallToolbarPicture.ico", 0, 0);
    gtk_box_pack_start(GTK_BOX(under_tool_box),cutting_button,FALSE,FALSE,5);
    g_signal_connect(cutting_button, "clicked", G_CALLBACK(deal_button), main_chat_personal_window);

	game_button = sungtk_button_new_with_image("./imgs/IMSmallToolbarFont.ico", 0, 0);
    gtk_box_pack_start(GTK_BOX(under_tool_box),game_button,FALSE,FALSE,5);
    g_signal_connect (G_OBJECT (game_button), "clicked", G_CALLBACK (load_five_window), 0);

    chat_history_button = gtk_button_new_with_label("聊天历史");
    gtk_box_pack_end(GTK_BOX(under_tool_box),chat_history_button,FALSE,FALSE,5);
    g_signal_connect(GTK_OBJECT(chat_history_button), "clicked", G_CALLBACK(history_message_window), f_msgbufs[id]);

	//输入框
	send_msg_box = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(main_box),send_msg_box,FALSE,FALSE,0);

    input_msg_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(input_msg_scrolled_window),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	input_msg_box = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(send_msg_box),input_msg_box,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(input_msg_box),input_msg_scrolled_window ,FALSE,FALSE,0);
	gtk_widget_show(input_msg_scrolled_window);

    input_text_buffer = gtk_text_buffer_new(NULL);
	input_msg_text = gtk_text_view_new_with_buffer(input_text_buffer);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(input_msg_text), GTK_WRAP_WORD);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(input_msg_scrolled_window), input_msg_text);
	gtk_widget_set_size_request(input_msg_scrolled_window, 400, 150);

	g_signal_connect (G_OBJECT (smile_button), "clicked", G_CALLBACK (load_emotion_window), input_text_buffer);

	the_last_box = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(send_msg_box),the_last_box,FALSE,FALSE,0);
	send_msg_button = gtk_button_new_with_label("  发  送  ");
    gtk_box_pack_end(GTK_BOX(the_last_box),send_msg_button,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(send_msg_button),"clicked",G_CALLBACK(send_msg_to), who);

    //热键
	GtkAccelGroup *face = gtk_accel_group_new();
    gtk_widget_add_accelerator(smile_button,"clicked",face,GDK_T,GDK_MOD1_MASK,GTK_ACCEL_VISIBLE);
    gtk_window_add_accel_group(GTK_WINDOW(main_chat_personal_window),face);

	GtkAccelGroup *file = gtk_accel_group_new();
    gtk_widget_add_accelerator(file_button,"clicked",file,GDK_Y,GDK_MOD1_MASK,GTK_ACCEL_VISIBLE);
    gtk_window_add_accel_group(GTK_WINDOW(main_chat_personal_window),file);

	GtkAccelGroup *history = gtk_accel_group_new();
    gtk_widget_add_accelerator(chat_history_button,"clicked",history,GDK_U,GDK_MOD1_MASK,GTK_ACCEL_VISIBLE);
    gtk_window_add_accel_group(GTK_WINDOW(main_chat_personal_window),history);

	GtkAccelGroup *enter = gtk_accel_group_new();
    gtk_widget_add_accelerator(send_msg_button,"clicked",enter, GDK_J, GDK_MOD1_MASK,GTK_ACCEL_VISIBLE);
    gtk_window_add_accel_group(GTK_WINDOW(main_chat_personal_window),enter);

    gtk_widget_show_all(main_chat_personal_window);

}


/********************************************************************************
 * Name          : reload_chat_personal_window
 * Description   : re open the chat window
 * Date:         : 2018.09.07
 * Author        : zhouzhi
 ********************************************************************************/
void reload_chat_personal_window(){
    if(chatting_with && ~get_friend_local_id(chatting_with)){
        load_chat_personal_window(chatting_with);
    }else if(chatting_with){
        cb_close_window();
    }
}

GtkWidget *group_main_chat_personal_window = NULL;
GtkTextBuffer* group_input_text_buffer;
Entity* chatting_in = NULL;


/********************************************************************************
 * Name          : close_group_window
 * Description   : to close the chat window
 * Date:         : 2018.09.07
 * Author        : zhouzhi
 ********************************************************************************/
void close_group_window()
{
    if(group_main_chat_personal_window){
        gtk_widget_destroy(group_main_chat_personal_window);
    }
    group_main_chat_personal_window = NULL;
    chatting_in = NULL;
}

/********************************************************************************
 * Name          : group_send_msg_in
 * Description   : to send the message to groups
 * Date:         : 2018.09.07
 * Author        : zhouzhi
 ********************************************************************************/
void group_send_msg_in(GtkWidget* trigger, Entity* who)
{
    int id = get_group_local_id(who->nickname);
    GtkTextIter start,end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(group_input_text_buffer),&start,&end);
    static char input[500];
    sprintf(input,"%s\n", gtk_text_buffer_get_text(GTK_TEXT_BUFFER(group_input_text_buffer),&start,&end,FALSE));
    gtk_text_buffer_set_text(input_text_buffer,"",0);
     post_msg_multicast(who->nickname, input);
    update_buffer(g_msgbufs[id], input, myself->nickname);
}

void load_chess_window(GtkWidget *button,GdkEvent *e,gpointer data)
{
	load_five_window();
	return;
}

/********************************************************************************
 * Name          : load_chat_group_window
 * Description   : to load the chat window for groups
 * Date:         : 2018.09.07
 * Author        : zhouzhi
 ********************************************************************************/
void load_chat_group_window(Entity* who)
{
    close_group_window();
    chatting_in = who;
    int id = get_group_local_id(who->nickname);
    GtkWidget *group_main_box;
    GtkWidget *group_friend_information_box;
    GtkWidget *group_friend_table;
    GtkWidget *group_friend_image;
    GtkWidget *group_friend_name_label;
    GtkWidget *group_friend_geqian_label;
    GtkWidget *group_chat_view_box;
    GtkWidget *group_chat_hpaned_window;
    GtkWidget *group_show_msg_scrolled_window;
    GtkWidget *group_msg_text;
    GtkWidget *group_chat_text_view;
    GtkWidget *group_under_tool_box;
    GtkWidget *group_smile_button;
    GtkWidget *group_file_button;
    GtkWidget *group_member_button;
    GtkWidget *group_chat_history_button;
    GtkWidget *group_send_msg_box;
    GtkWidget *group_send_msg_button;
	GtkWidget *group_cutting_button;
	GtkWidget *group_game_button;
    GtkWidget *group_input_msg_box;
    GtkWidget *group_input_msg_text;
    GtkWidget *group_input_msg_scrolled_window;
    GtkWidget *group_the_last_box;

    //创建界面
    group_main_chat_personal_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(group_main_chat_personal_window),600,500);
    change_background(group_main_chat_personal_window,600,500,"imgs/bg/04.jpg");

    //上层界面
    group_main_box = gtk_vbox_new(FALSE, 0);
    group_under_tool_box = gtk_hbox_new(FALSE, 0);
    group_input_msg_box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(group_main_chat_personal_window), group_main_box);
    group_friend_information_box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(group_main_box),group_friend_information_box,FALSE,FALSE,0);

    group_friend_table = gtk_table_new(5,5,FALSE);
    gtk_box_pack_start(GTK_BOX(group_friend_information_box),group_friend_table,FALSE,FALSE,0);

    //显示群信息
    char* imgdir[100];
    sprintf(imgdir, "./imgs/avatars/%02d.png", who->avatar_id);
    group_friend_image = gtk_image_new_from_file (imgdir);
    gtk_table_attach(GTK_TABLE(group_friend_table),group_friend_image,1,3,1,5,GTK_FILL,GTK_FILL,0,0);

    group_friend_name_label = gtk_label_new(who->nickname);
    gtk_table_attach(GTK_TABLE(group_friend_table),group_friend_name_label,4,5,1,3,GTK_FILL,GTK_FILL,0,0);

    group_friend_geqian_label = gtk_label_new("(Group)");
    gtk_table_attach(GTK_TABLE(group_friend_table),group_friend_geqian_label,4,5,4,5,GTK_FILL,GTK_FILL,0,0);

    group_chat_view_box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(group_main_box),group_chat_view_box,FALSE,FALSE,0);

    //显示聊天信息框
    group_show_msg_scrolled_window = gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(group_show_msg_scrolled_window, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_box_pack_start(GTK_BOX(group_chat_view_box),group_show_msg_scrolled_window,TRUE,TRUE,0);
    gtk_widget_show(group_show_msg_scrolled_window);
    group_msg_text = gtk_text_view_new_with_buffer(g_msgbufs[id]);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(group_show_msg_scrolled_window),group_msg_text);
    gtk_widget_set_size_request(group_show_msg_scrolled_window, 400, 300);


    //功能栏
    gtk_box_pack_start(GTK_BOX(group_main_box),group_under_tool_box,FALSE,FALSE,0);
    group_smile_button = sungtk_button_new_with_image("./imgs/IMSmallToolbarFace.ico", 0, 0);
    gtk_box_pack_start(GTK_BOX(group_under_tool_box),group_smile_button,FALSE,FALSE,5);

    group_file_button = sungtk_button_new_with_image("./imgs/IMBigToolbarSendFile.ico", 0, 0);
    gtk_box_pack_start(GTK_BOX(group_under_tool_box),group_file_button,FALSE,FALSE,5);
	g_signal_connect (G_OBJECT (group_file_button), "clicked", G_CALLBACK (button_openfile), (gpointer) "打开");

	group_cutting_button = sungtk_button_new_with_image("./imgs/IMSmallToolbarPicture.ico", 0, 0);
    gtk_box_pack_start(GTK_BOX(group_under_tool_box),group_cutting_button,FALSE,FALSE,5);
    g_signal_connect(group_cutting_button, "clicked", G_CALLBACK(deal_button), group_main_chat_personal_window);


	group_game_button = sungtk_button_new_with_image("./imgs/IMSmallToolbarFont.ico", 0, 0);
    gtk_box_pack_start(GTK_BOX(group_under_tool_box),group_game_button,FALSE,FALSE,5);
    g_signal_connect (G_OBJECT (group_game_button), "clicked", G_CALLBACK (load_five_window), 0);

    group_member_button = sungtk_button_new_with_image("./imgs/IMBigToolbarGroupMember.ico", 0, 0);
    gtk_box_pack_start(GTK_BOX(group_under_tool_box),group_member_button,FALSE,FALSE,5);


    group_chat_history_button = gtk_button_new_with_label("聊天历史");
    gtk_box_pack_end(GTK_BOX(group_under_tool_box),group_chat_history_button,FALSE,FALSE,5);

    //输入框
    group_send_msg_box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(group_main_box),group_send_msg_box,FALSE,FALSE,0);

    group_input_msg_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(group_input_msg_scrolled_window),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    group_input_msg_box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(group_send_msg_box),group_input_msg_box,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(group_input_msg_box),group_input_msg_scrolled_window ,FALSE,FALSE,0);
    gtk_widget_show(group_input_msg_scrolled_window);

    group_input_text_buffer = gtk_text_buffer_new(NULL);
    group_input_msg_text = gtk_text_view_new_with_buffer(group_input_text_buffer);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(group_input_msg_text), GTK_WRAP_WORD);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(group_input_msg_scrolled_window), group_input_msg_text);
    gtk_widget_set_size_request(group_input_msg_scrolled_window, 400, 150);


    group_the_last_box = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(group_send_msg_box),group_the_last_box,FALSE,FALSE,0);
    group_send_msg_button = gtk_button_new_with_label("  发  送  ");
    gtk_box_pack_end(GTK_BOX(group_the_last_box),group_send_msg_button,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(group_send_msg_button),"clicked",G_CALLBACK(group_send_msg_in), who);
	g_signal_connect (G_OBJECT (group_smile_button), "clicked", G_CALLBACK (load_emotion_window), group_input_text_buffer);


	//设置热键
	GtkAccelGroup *face = gtk_accel_group_new();
    gtk_widget_add_accelerator(group_smile_button,"clicked",face,GDK_Q,GDK_MOD1_MASK,GTK_ACCEL_VISIBLE);
    gtk_window_add_accel_group(GTK_WINDOW(group_main_chat_personal_window),face);

	GtkAccelGroup *file = gtk_accel_group_new();
    gtk_widget_add_accelerator(group_file_button,"clicked",file,GDK_W,GDK_MOD1_MASK,GTK_ACCEL_VISIBLE);
    gtk_window_add_accel_group(GTK_WINDOW(group_main_chat_personal_window),file);

    GtkAccelGroup *member = gtk_accel_group_new();
    gtk_widget_add_accelerator(group_member_button,"clicked",member,GDK_E,GDK_MOD1_MASK,GTK_ACCEL_VISIBLE);
    gtk_window_add_accel_group(GTK_WINDOW(group_main_chat_personal_window),member);

	GtkAccelGroup *history = gtk_accel_group_new();
    gtk_widget_add_accelerator(group_chat_history_button,"clicked",history,GDK_R,GDK_MOD1_MASK,GTK_ACCEL_VISIBLE);
    gtk_window_add_accel_group(GTK_WINDOW(group_main_chat_personal_window),history);

	GtkAccelGroup *enter = gtk_accel_group_new();
    gtk_widget_add_accelerator(group_send_msg_button,"clicked",enter, GDK_S, GDK_MOD1_MASK,GTK_ACCEL_VISIBLE);
    gtk_window_add_accel_group(GTK_WINDOW(group_main_chat_personal_window),enter);

    gtk_widget_show_all(group_main_chat_personal_window);
}
