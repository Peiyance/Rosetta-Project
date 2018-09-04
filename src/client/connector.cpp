#include <iostream>
#include <cstring>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <gtk/gtk.h>
// #include <event2/event.h>
// #include <event2/event_struct.h>
// #include <event2/buffer.h>
// #include <event2/bufferevent.h>
// #include <event2/util.h>
#include "connector.h"

namespace connector
{
int sockfd;
sockaddr_in sin;
int send_package_sequence = 0; //发送封包编号计数器
int recv_package_sequence = 0; //接收封包编号计数器

struct Package
{
    int package_sequence;
    short ver;
    int len;
    char payload[0];
};

gboolean (*cb_req_contacts)(gpointer) = NULL;
gboolean (*cb_req_authentication)(gpointer) = NULL;
gboolean (*cb_req_register)(gpointer) = NULL;
gboolean (*cb_connection_lost)(gpointer) = NULL;
} // namespace connector
using namespace connector;

char *escape_string(char *msg, char *escaped);

// Interfaces
// returns sockfd. create a thread maintaining the long-term TCP.
extern "C" int init_connector(char remoteIP[], short remotePort)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //tcp
    if (sockfd <= 0)
        return -1;

    sin.sin_family = AF_INET;           //ipv4
    sin.sin_port = htons(remotePort);   //port
    inet_aton(remoteIP, &sin.sin_addr); //addr

    pthread_t thread1;
    pthread_create(&thread1, NULL, pthread, (void *)0);

    return sockfd;
}

/********************************************************************************
Description : User Authentication 
Parameter   : username, password, Func_cb
Return      : int (0 == success, -1 == failed)
Side effect :
Author      : zyc
Date        : 2018.9.3
********************************************************************************/
extern "C" int req_authentication(char *str_username, char *str_password, gboolean (*callback)(gpointer))
{
    cb_req_authentication = callback; // reg callback
    // 转义
    char escaped_username[1024], escaped_password[1024];
    escape_string(str_username, escaped_username);
    escape_string(str_password, escaped_password);

    // 构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;

    strcat(escaped_username, ",");
    memcpy(pkg->payload, "/0", strlen("/0"));
    memcpy(pkg->payload + 2, escaped_username, strlen(escaped_username));
    memcpy(pkg->payload + 2 + strlen(escaped_username), escaped_password, strlen(escaped_password));
    pkg->len = 2 + strlen(escaped_username) + strlen(escaped_password);

    write(sockfd, pkg, sizeof(Package) + pkg->len);
    return 0;
}

extern "C" int req_register(char *str_username, char *str_password, gboolean (*callback)(gpointer))
{
    cb_req_register = callback; // reg callback
    // 转义
    char escaped_username[1024], escaped_password[1024];
    escape_string(str_username, escaped_username);
    escape_string(str_password, escaped_password);

    // 构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;

    strcat(escaped_username, ",");
    memcpy(pkg->payload, "/1", strlen("/1"));
    memcpy(pkg->payload + 2, escaped_username, strlen(escaped_username));
    memcpy(pkg->payload + 2 + strlen(escaped_username), escaped_password, strlen(escaped_password));
    pkg->len = 2 + strlen(escaped_username) + strlen(escaped_password);

    write(sockfd, pkg, sizeof(Package) + pkg->len);
    return 0;
}

int req_contacts(gboolean (*callback)(gpointer))
{
    cb_req_contacts = callback; // reg callback

    // 构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + 4];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;

    memcpy(pkg->payload, "/2", strlen("/2"));
    pkg->len = 2;

    write(sockfd, pkg, sizeof(Package) + pkg->len);
    return 0;
}

int req_edit_contacts(char *username, int opt, gboolean (*callback)(gpointer))
{
    return 0;
}

int post_msg_unicast(char *str_peer, char *msg)
{
    return 0;
}

void reg_cb_connection_lost(gboolean (*callback)(gpointer))
{
    cb_connection_lost = callback;
}

static void *pthread(void *arg)
{
    int first_contact = 1;
    std::cout << "接收线程，启动！" << std::endl;

    char msg[1024];

    for (;;)
    {
        int len = read(sockfd, msg, sizeof(msg) - 1);

        //断线重连
        if (len <= 0)
        {
            for (;;)
            {
                if (1)
                {
                    printf("[Error] Connection Lost\n");
                    if (cb_connection_lost)
                        g_idle_add(cb_connection_lost, (void*)10086);
                }
                close(sockfd);
                sockfd = socket(AF_INET, SOCK_STREAM, 0); //tcp

                ioctl(sockfd, FIONBIO, 1); //1:非阻塞 0:阻塞
                int conn = connect(sockfd, (sockaddr *)&sin, sizeof(sin));

                if (conn == 0) //connected
                {
                    ioctl(sockfd, FIONBIO, 0);
                    first_contact = 0;
                    printf("Connected.\n");

                    break;
                }
                else
                {
                    fd_set fd;
                    FD_ZERO(&fd);
                    FD_SET(sockfd, &fd);
                    timeval timeout; //设置超时时间
                    timeout.tv_sec = 3;
                    timeout.tv_usec = 0;
                    int ret = select(1 + 1, 0, &fd, 0, &timeout);

                    if (ret <= 0) //select() timeout
                    {
                        continue;
                    }
                    else //connected
                    {
                        ioctl(sockfd, FIONBIO, 0);
                        first_contact = 0;
                        printf("Connected.\n");

                        break;
                    }
                }
            }

            continue;
        }
        msg[len] = '\0';

        // 解包，调用回调
        Package *pkg = (Package *)msg;
        if (pkg->payload[0] == '/' && pkg->payload[1] == '0')
        { // /0 login
            if (pkg->payload[2] == '1') //success
                g_idle_add(cb_req_authentication, (void*)1);
            else
                g_idle_add(cb_req_authentication, (void*)0);
        }
        else if (pkg->payload[0] == '/' && pkg->payload[1] == '1')
        { // /1 register
            if (pkg->payload[2] == '1')  //success
                g_idle_add(cb_req_register, (void*)1);
            else
                g_idle_add(cb_req_register, (void*)0);
        }
        else if (pkg->payload[0] == '/' && pkg->payload[1] == '2')
        { // /2 contacts
            g_idle_add(cb_req_contacts, &pkg->payload[2]);
        }

        printf("recv %s from server\n", pkg->payload);
    }
}

char *escape_string(char *msg, char *escaped)
{
    int temp[1024], cnt = 0, temp_cnt = 0, i;

    int len = strlen(msg);
    for (int i = 0; i < len; i++)
    {
        if (msg[i] == '\\')
        {
            temp[cnt] = i + cnt;
            cnt++;
        }
        else if (msg[i] == ',')
        {
            temp[cnt] = i + cnt;
            cnt++;
        }
    }
    for (int i = 0; i < len + cnt; i++)
    {
        if (i == temp[temp_cnt])
        {
            escaped[i] = '\\';
            escaped[i + 1] = msg[i - temp_cnt];
            temp_cnt++;
        }
        else
            escaped[i] = msg[i - temp_cnt];
    }
    escaped[len + cnt] = '\0';
    return escaped;
}
