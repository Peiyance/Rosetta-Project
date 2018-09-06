/********************************************************
	>Files			:
	>Description	: the UI for group chat window
	>Author			: zhouzhi
	>Last Modified	: 2018.09.06, 13:24
 ********************************************************/
#include "GUI.h"
#include <time.h>
GtkWidget *group_view_text_buffer;
GtkWidget *group_input_text_buffer;
//得到当前时间
/*char *get_time()
{
    time_t *timep = malloc(sizeof(*timep));
    time(timep);
    char *s = ctime(timep);
    return s;
}*/
//更新对话界面
/*void group_update_buffer(GtkWidget *group_buffer,char *msg)
{
    GtkTextIter start,end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(group_buffer),&start,&end);
    //插入文本到缓冲区
    char now_time[40];
    sprintf(now_time,get_time());
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(group_buffer),&end,now_time,strlen(now_time));
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(group_buffer),&start,&end);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(group_buffer),&end,msg,strlen(msg));
    gtk_widget_show_all(group_buffer);
}
void group_send_msg_to(GtkWidget *group_button,gpointer data)
{
    GtkTextIter start,end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(group_input_text_buffer),&start,&end);
    g_print("01\n");
    //插入文本到缓冲区:
    char input[500];
    sprintf(input,"%s\n", gtk_text_buffer_get_text(GTK_TEXT_BUFFER(group_input_text_buffer),&start,&end,FALSE));
    g_print("02");
    gtk_text_buffer_set_text(group_input_text_buffer,"",0);

    这里是将input中的内容传到服务器的部分
    */

    /*group_update_buffer(group_view_text_buffer,input);
    gtk_widget_show_all(group_input_text_buffer);
    gtk_widget_show_all(group_view_text_buffer);
}*/
void chat_group_window()
{
    //get_time();
    GtkWidget *group_main_chat_personal_window;
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
    GtkWidget *group_input_msg_box;
    GtkWidget *group_input_msg_text;
    GtkWidget *group_input_msg_scrolled_window;
    GtkWidget *group_the_last_box;

    //=======================创建界面==============================
    group_main_chat_personal_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(group_main_chat_personal_window),400,500);

    //=======================上层界面==============================
    group_main_box = gtk_vbox_new(FALSE, 0);
    group_under_tool_box = gtk_hbox_new(FALSE, 0);
    group_input_msg_box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(group_main_chat_personal_window), group_main_box);
    group_friend_information_box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(group_main_box),group_friend_information_box,FALSE,FALSE,0);

    group_friend_table = gtk_table_new(5,5,FALSE);
    gtk_box_pack_start(GTK_BOX(group_friend_information_box),group_friend_table,FALSE,FALSE,0);
    group_friend_image = gtk_image_new_from_file ("./imgs/avatars/03.png");
    gtk_table_attach(GTK_TABLE(group_friend_table),group_friend_image,1,3,1,5,GTK_FILL,GTK_FILL,0,0);

    group_friend_name_label = gtk_label_new("group_name");
    gtk_table_attach(GTK_TABLE(group_friend_table),group_friend_name_label,4,5,1,3,GTK_FILL,GTK_FILL,0,0);

    group_friend_geqian_label = gtk_label_new("group_introduction");
    gtk_table_attach(GTK_TABLE(group_friend_table),group_friend_geqian_label,4,5,4,5,GTK_FILL,GTK_FILL,0,0);

    group_chat_view_box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(group_main_box),group_chat_view_box,FALSE,FALSE,0);

    group_show_msg_scrolled_window = gtk_scrolled_window_new(NULL,NULL);                              /*创建滚动窗口构件*/
    gtk_scrolled_window_set_policy(group_show_msg_scrolled_window, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_box_pack_start(GTK_BOX(group_chat_view_box),group_show_msg_scrolled_window,TRUE,TRUE,0);
    gtk_widget_show(group_show_msg_scrolled_window);               /*显示滚动窗口构件*/
    group_view_text_buffer = gtk_text_buffer_new(NULL);
    group_msg_text = gtk_text_view_new_with_buffer(group_view_text_buffer);                                                             /*创建文本视图构件*/
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(group_show_msg_scrolled_window),group_msg_text);      /*将文本视图构件加入滚动窗口*/
    gtk_widget_set_size_request(group_show_msg_scrolled_window, 400, 300);


    //功能栏
    gtk_box_pack_start(GTK_BOX(group_main_box),group_under_tool_box,FALSE,FALSE,0);

    group_smile_button = sungtk_button_new_with_image("./imgs/IMSmallToolbarFace.ico", 0, 0);
    //获得缓冲区开始和结束位置的Iter
    gtk_box_pack_start(GTK_BOX(group_under_tool_box),group_smile_button,FALSE,FALSE,5);
    group_file_button = sungtk_button_new_with_image("./imgs/IMBigToolbarSendFile.ico", 0, 0);
    gtk_box_pack_start(GTK_BOX(group_under_tool_box),group_file_button,FALSE,FALSE,5);

    group_member_button = sungtk_button_new_with_image("./imgs/IMBigToolbarGroupMember.ico", 0, 0);
    gtk_box_pack_start(GTK_BOX(group_under_tool_box),group_member_button,FALSE,FALSE,5);


    group_chat_history_button = gtk_button_new_with_label("聊天历史");
    gtk_box_pack_end(GTK_BOX(group_under_tool_box),group_chat_history_button,FALSE,FALSE,5);

	//输入
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
    //g_signal_connect(GTK_OBJECT(group_send_msg_button),"clicked",G_CALLBACK(group_send_msg_to),0);

    gtk_widget_show_all(group_main_chat_personal_window);

}
