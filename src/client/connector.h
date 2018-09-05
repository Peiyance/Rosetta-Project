#include <gtk/gtk.h>

// interfaces
#ifdef __cplusplus
extern "C"
{
#endif

    // interfaces
    int init_connector(char remoteIP[], short remotePort);                                          // 与服务器建立连接。将会创建新的线程pthread
    int req_authentication(char *str_username, char *str_password, gboolean (*callback)(gpointer)); // 发送登陆请求。结果将在回调中给出
    int req_register(char *str_username, char *str_password, gboolean (*callback)(gpointer));
    int req_contacts(gboolean (*callback)(gpointer));
    int req_search_contacts(char *keyword, gboolean (*callback)(gpointer));
    int req_delete_contacts(char *username, char *contact_name, gboolean (*callback)(gpointer));
    int req_add_contacts(char *username, char *contact_name, gboolean (*callback)(gpointer));

    int post_msg_unicast(char *str_peer, char *msg);
    int post_msg_multicast(unsigned int groupId, char *msg);
    void reg_cb_recv_unicast(gboolean (*callback)(gpointer));
    void reg_cb_recv_multicast(gboolean (*callback)(gpointer));
#ifdef __cplusplus
}
#endif /* end of __cplusplus */

//
static void *pthread(void *arg); // socket接收线程