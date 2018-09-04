#include <iostream>
#include <unistd.h>
#include <gtk/gtk.h>
#include "connector.h"

char remoteIP[] = "10.211.55.2";
short remotePort = 8888;
int count = 0;
GtkWidget *window;

void cb_req_authentication(int result);
void cb_connection_lost(int error_code);

void on_button_clicked(GtkWidget *button, gpointer userdata)
{
    // //g_print("你好，这是测试程序。");
    // g_print("Hello, this is a test!");
    // //g_print("%d\n",(gint)userdata);
    // g_print("%d\n", count);
    // count = count + 1;
    char username[] = "username,x\\xx\\,\\" ;
    char password[] = "pswwwwwfse";

    req_authentication(username,password, cb_req_authentication);
}

void cb_req_authentication(int result)
{
    g_print("登陆结果：%d\n", result);
    // gtk_window_set_title(GTK_WINDOW(window), "ffss"); //回调中修改GTK元素会产生错误?

    /*
    The program 'main' received an X Window System error.
    This probably reflects a bug in the program.
    The error was 'BadDrawable (invalid Pixmap or Window parameter)'.
  (Details: serial 3540 error_code 9 request_code 141 minor_code 4)
  (Note to programmers: normally, X errors are reported asynchronously;
   that is, you will receive the error a while after causing it.
   To debug your program, run it with the --sync command line
   option to change this behavior. You can then get a meaningful
   backtrace from your debugger if you break on the gdk_x_error() function.)
   */
}

int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;
    init_connector(remoteIP, remotePort);

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
