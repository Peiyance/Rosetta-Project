#include <gtk/gtk.h>

// interfaces
#ifdef __cplusplus
extern "C"{
#endif

    /* C头文件的其它声明 */
    int init_connector(char remoteIP[], short remotePort);                                          // 与服务器建立连接。将会创建新的线程pthread
    int req_authentication(char *str_username, char *str_password, gboolean (*callback)(gpointer)); // 发送登陆请求。结果将在回调中给出
    int req_register(char *str_username, char *str_password, gboolean (*callback)(gpointer));

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

// int req_contacts(gboolean (*callback)(gpointer));
// int req_edit_contacts(char *username, int opt, gboolean (*callback)(gpointer));
// int post_msg_unicast(char *str_peer, char *msg);
// int post_msg_multicast(unsigned int groupId, char *msg);
// void reg_cb_recv_unicast(gboolean (*callback)(gpointer));
// void reg_cb_recv_multicast(gboolean (*callback)(gpointer));

//
static void *pthread(void *arg); // socket接收线程。所有回调函数也将在这一线程中执行，所以回调函数应尽可能简短，以免阻塞消息接受
                                 //void socket_read_cb(int fd, short event, void *arg);