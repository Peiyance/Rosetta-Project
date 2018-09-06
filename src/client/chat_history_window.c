/**************************************************/
/*名称：change_info.c
/*描述：修改个人信息界面部分
/*作成日期： 2018-9-3
/*作者：周智
/***************************************************/

#include "GUI.h"
/**************************************************/
/*名称：history_message_window
/*描述：加载历史消息记录部分
/*作成日期： 2018-9-3
/*参数：参数1:trigger,GtkWidget*类型,输入,加载历史消息记录部分
		参数2: buf,GtkTextBuffer*类型,输入,无意义
/*返回值：void
/*作者：周智
/***************************************************/
GtkWidget *history_window = NULL;
void closing_window(){
    gtk_widget_destroy(history_window);
    history_window = NULL;
}

void history_message_window(GtkWidget* trigger, GtkTextBuffer* buf)
{
	// GtkWidget *history_window;
    if(history_window){
        gtk_widget_destroy(history_window);
    }    
	GtkWidget *history_main_box;
	GtkWidget *the_top_box;
	GtkWidget *history_box;

	GtkWidget *history_label;
	GtkWidget *history_scrolled;
	GtkWidget *history_text;

	//=======================创建界面==============================
	history_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(history_window),300,400);
    g_signal_connect(GTK_OBJECT(history_window), "delete_event", G_CALLBACK(closing_window), NULL);
    
	//=======================历史消息label=========================
    history_main_box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(history_window),history_main_box);

    // the_top_box = gtk_vbox_new(FALSE, 0);
    // gtk_box_pack_start(GTK_BOX(history_main_box),the_top_box,FALSE,FALSE,3);

    history_box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(history_main_box),history_box,FALSE,FALSE,3);

    history_label = gtk_label_new("历史记录");
    // gtk_container_add(GTK_CONTAINER(the_top_box), history_label);
	//gtk_container_add(GTK_CONTAINER(history_window), history_box);
    
	//gtk_widget_set_usize(history_label,100,50);
    gtk_misc_set_alignment(GTK_MISC(history_label), 0.5, 0);
	//gtk_widget_set_usize(history_label, 450, 30);
	gtk_widget_set_size_request(history_label,30,-1);
    gtk_container_add(GTK_CONTAINER(history_box), history_label);

	//=======================历史消息界面==========================
    history_scrolled=gtk_scrolled_window_new(NULL,NULL); /*创建滚动窗口构件*/
    gtk_scrolled_window_set_policy(history_scrolled, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(history_box),history_scrolled);/*将滚动窗口构件加入窗体*/
    gtk_widget_show(history_scrolled);/*显示滚动窗口构件*/
    history_text = gtk_text_view_new_with_buffer(buf);/*创建文本视图构件*/
    gtk_text_view_set_editable(history_text, FALSE);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(history_scrolled),history_text);/*将文本视图构件加入滚动窗口*/
    gtk_widget_set_size_request(history_scrolled, 300, 372);

    /*history_vbox = gtk_vbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(history_window), vbox);
    history_label = gtk_label_new("历史记录");
    gtk_label_set_text( GTK_LABEL(history_label), "历史记录");*/

	gtk_widget_show_all(history_window);

}


