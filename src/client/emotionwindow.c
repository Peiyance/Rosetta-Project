/**************************************************/
/*名称：emmotionwndow.c
/*描述：选择表情界面
/*作成日期： 2018-9-7
/*作者：程苗苗
/***************************************************/
#include "GUI.h"
char *emtion[9]  = {"(＾ω＾)","(*^▽^*)","(＾▽＾)","(≧∀≦)♪","(￣▽￣)／","(⊙…⊙)","(T＿T)","|°з°|","╥﹏╥"};
void update_bu(GtkTextBuffer *buffer, gchar *msg)
{
    gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer), msg, -1);
}
/**************************************************/
/*名称：add_buffer_emotion
/*描述：选择表情并把表情加到输入框里
/*作成日期： 2018-9-7
/*参数：参数1:button,GtkWidget*,输入,当前点击的按钮
	参数2:buffer,gpointer,输入，输入框的textbuffer
/*返回值：void
/*作者：程苗苗
/***************************************************/
void add_buffer_emotion(GtkWidget *button,gpointer buffer)
{
	 gchar s[100];
	sprintf(s,gtk_button_get_label(GTK_BUTTON (button)));
    g_print("%s\n",s);
    gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer), s, -1);
}
/**************************************************/
/*名称：load_emotion_window
/*描述：选择表情界面
/*作成日期： 2018-9-7
/*参数：
/*返回值：void
/*作者：程苗苗
/***************************************************/
void load_emotion_window(GtkWidget *butt, gpointer buffer)
{
	GtkWidget *emotion_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(emotion_window),0,0);

	GtkWidget *emotion_button[9];
	GtkWidget *main_table = gtk_table_new(3,3,FALSE);
    gtk_container_add(GTK_CONTAINER(emotion_window),main_table);
    gtk_container_set_border_width(GTK_CONTAINER(main_table),30); 

    for(int i=0;i<9;i++){
		emotion_button[i] = gtk_button_new_with_label(emtion[i]);
		gtk_table_attach_defaults(GTK_TABLE(main_table),emotion_button[i],i/3,i/3+1,i%3,i%3+1);
		g_signal_connect(G_OBJECT(emotion_button[i]),"clicked",G_CALLBACK(add_buffer_emotion),buffer);
    }


	gtk_widget_show_all(emotion_window);
}
