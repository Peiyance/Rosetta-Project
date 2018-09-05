/********************************************************
	>Files			:
	>Description	:
	>Author			: zhouzhi
	>Last Modified	: 2018.09.03, 15:25
 ********************************************************/
#include <gtk/gtk.h>
#include "sungtk_interface.h"
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

void destroy (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();
}

int history_message_window()
{
	GtkWidget *history_window;
	GtkWidget *history_main_box;
	GtkWidget *the_top_box;
	GtkWidget *history_box;

	GtkWidget *history_label;
	GtkWidget *history_scrolled;
	GtkWidget *history_text;

	//=======================创建界面==============================
	history_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(history_window),300,400);
	gtk_signal_connect (GTK_OBJECT (history_window), "destroy",GTK_SIGNAL_FUNC (destroy), NULL);
    
	//=======================历史消息label=========================
    history_main_box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(history_window),history_main_box);

    the_top_box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(history_main_box),the_top_box,FALSE,FALSE,3);

    history_box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(history_main_box),history_box,FALSE,FALSE,3);

    history_label = gtk_label_new("历史记录");
    gtk_container_add(GTK_CONTAINER(the_top_box), history_label);
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
    history_text = gtk_text_view_new();/*创建文本视图构件*/
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(history_scrolled),history_text);/*将文本视图构件加入滚动窗口*/
    gtk_widget_set_size_request(history_scrolled, 300, 372);

    /*history_vbox = gtk_vbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(history_window), vbox);
    history_label = gtk_label_new("历史记录");
    gtk_label_set_text( GTK_LABEL(history_label), "历史记录");*/

	gtk_widget_show_all(history_window);

}

int main(int argv, char *argc[])
{
	gtk_init(&argv, &argc);
	history_message_window();
	gtk_main();
}


