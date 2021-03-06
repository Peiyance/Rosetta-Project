#include <stdio.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "connector.h"
#include "common.h"


char remoteIP[] = "192.168.0.100";
short remotePort = 10005;
int count = 0;
GtkWidget *window;

gboolean on_finish(gpointer data);
void cb_connection_lost(int error_code);

void on_button_clicked(GtkWidget *button, gpointer userdata)
{
    char username[] = "username,x\\xx\\,\\";
    char password[] = "pswwwwwfse";
    //req_authentication("u","p",on_finish);
    //req_register("v","w",on_finish);
    //File_private("/media/psf/Data/Stellardrone - Billions And Billions.mp3","myName");
    //post_msg_unicast("123","hello1234567890");
    char user[]="123";
    char msg[]="msgmsgmsg!";
    //post_msg_unicast(user, msg);
    //post_msg_multicast("g1",msg);
    printf("send !");
    //File_private("/var/tmp/a.txt","myName");
    req_contacts("fihxc",on_finish);
}

gboolean on_finish(gpointer data)
{
    Entity *contacts = (Entity*)data;
    if (data)
         g_print("登陆结果：1\n", data);
     else
         g_print("登陆结果：0\n", data);

    char txt[40];
    static int x = 0;
    sprintf(txt, "counter：%d", x++);

    gtk_window_set_title(GTK_WINDOW(window), txt);
    
    return FALSE;
}


int main(int argc, char *argv[])
{
    //std::cout << "Hello World!" << std::endl;
    init_connector(remoteIP, remotePort);

    //if (!g_thread_supported()) //如果gthread沒有被初始化
    //   g_thread_init(NULL);   //進行初始化
    //gdk_threads_init();        //初始化GDK多線程

    GtkWidget *button;
    gtk_init(&argc, &argv);
    //创建最上层主窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    //连接信号"delete_event",使得窗口关闭时发生
    g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);
    //设置窗口标题
    gtk_window_set_title(GTK_WINDOW(window), "Hello 功能实现");
    //设定窗口的默认宽高
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    //设定窗口的位置
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    //设定容器边框的宽度，否则按钮填充整个窗口
    gtk_container_set_border_width(GTK_CONTAINER(window), 60);

    //创建一个带有标签的按钮
    button = gtk_button_new_with_label("按下此按钮会在终端上显示一行信息");
    //连接信号"clicked",使得窗口关闭时发生
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button_clicked), (gpointer)count);
    //把按钮装入窗口
    gtk_container_add(GTK_CONTAINER(window), button);

    //显示窗口和按钮
    gtk_widget_show(button);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}
