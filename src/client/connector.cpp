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
#include "common.h"

namespace connector
{
int sockfd;
sockaddr_in sin;
int send_package_sequence = 0; //发送封包编号计数器
int recv_package_sequence = 0; //接收封包编号计数器
char address_file[500];

struct Package
{
    int package_sequence;
    int ver;
    int len;
    char payload[0];
};

// 我们使用同步callback，类似于win中的PostMessage
gboolean (*cb_req_contacts)(gpointer) = NULL;
gboolean (*cb_req_authentication)(gpointer) = NULL;
gboolean (*cb_req_register)(gpointer) = NULL;
gboolean (*cb_connection_lost)(gpointer) = NULL;
gboolean (*cb_recv_unicast)(gpointer) = NULL;
gboolean (*cb_recv_multicast)(gpointer) = NULL;
gboolean (*cb_req_delete_contacts)(gpointer) = NULL;
gboolean (*cb_req_search_contacts)(gpointer) = NULL;
gboolean (*cb_req_add_contacts)(gpointer) = NULL;

} // namespace connector
using namespace connector;

char *escape_string(char *msg, char *escaped);

// Interfaces
// returns sockfd. create a thread maintaining the long-term TCP.
int init_connector(char remoteIP[], short remotePort)
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
int req_authentication(char *str_username, char *str_password, gboolean (*callback)(gpointer))
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

    // 发包 返回
    write(sockfd, pkg, sizeof(Package) + pkg->len);
    return 0;
}

int req_register(char *str_username, char *str_password, gboolean (*callback)(gpointer))
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

    // 发包 返回
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

/********************************************************************************
Description : Private_talk
Parameter   : char *msg, char *str_peer
Return      : int (0 == success, -1 == failed)
Side effect :
Author      : zhq
Date        : 2018.9.4
********************************************************************************/
int post_msg_unicast(char *str_peer, char *msg)
{
    //转义
    char escape_str_peer[1024], escape_msg[1024];
    escape_string(str_peer, escape_str_peer);
    escape_string(msg, escape_msg);

    //构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;
    strcat(escape_str_peer, ",");
    //   memcpy(pkg->payload, "/1", strlen("/1"));
    memcpy(pkg->payload, escape_str_peer, strlen(escape_str_peer));
    memcpy(pkg->payload + strlen(escape_str_peer), escape_msg, strlen(escape_msg));
    pkg->len = strlen(escape_str_peer) + strlen(escape_msg);

    // 发包 返回
    write(sockfd, pkg, sizeof(Package) + pkg->len);
    return 0;
}

/********************************************************************************
Description : Public_talk
Parameter   : char *msg, unsigned int Group_Id
Return      : int (0 == success, -1 == failed)
Side effect :
Author      : zhq
Date        : 2018.9.4
********************************************************************************/
int post_msg_multicast(unsigned int GroupId, char *msg)
{
    //转义
    char Group_Id[100], escape_msg[1024];
    snprintf(Group_Id, 5, "%d", GroupId);
    escape_string(msg, escape_msg);

    // 构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;
    strcat(Group_Id, ",");
    memcpy(pkg->payload, "$$", strlen("$$"));
    memcpy(pkg->payload + strlen("$$"), Group_Id, strlen(Group_Id));
    memcpy(pkg->payload + strlen(Group_Id) + strlen("$$"), escape_msg, strlen(escape_msg));
    pkg->len = strlen("$$") + strlen(escape_msg) + strlen(Group_Id);

    // 发包 返回
    write(sockfd, pkg, sizeof(Package) + pkg->len);
    return 0;
}

/********************************************************************************
Description : Reg_cb_recv_unicast
Parameter   : void (*callback)
Return      : None
Side effect :
Author      : zhq
Date        : 2018.9.4
********************************************************************************/
void reg_cb_recv_unicast(gboolean (*callback)(gpointer)) //Ë½ÁÄÏûÏ¢µ½´ï,µ÷ÓÃ¡£
{
    cb_recv_unicast = callback;
}

void reg_cb_recv_multicast(gboolean (*callback)(gpointer)) //ÈºÁÄ½ÓÊÕ»Øµ÷¡£Á¢¼´·µ»Ø
{
    cb_recv_multicast = callback;
}

/********************************************************************************
Description : Req_Search_Contacts
Parameter   : char *keyword void (*callback)(char *contacts_raw)
Return      : int (0 == success, -1 == failed)
Side effect :
Author      : zhq
Date        : 2018.9.4
********************************************************************************/
int req_search_contacts(char *keyword, gboolean (*callback)(gpointer))
{
    cb_req_search_contacts = callback;
    char escaped_keyword[1024];
    escape_string(keyword, escaped_keyword); //转义

    //构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;

    //strcat(escaped_username , ",");
    memcpy(pkg->payload, "/5", strlen("/5"));
    memcpy(pkg->payload + strlen("/5"), escaped_keyword, strlen(escaped_keyword));
    //memcpy(pkg->payload + strlen(escaped_username) + strlen("\2"), escape_msg, strlen(escape_msg));
    pkg->len = strlen("/5") + strlen(escaped_keyword);

    // 发送
    write(sockfd, pkg, sizeof(Package) + pkg->len);
    return 0;
}

/********************************************************************************
Description : Delete_Contacts
Parameter   : char *username  char *contact_name void (*callback)(char *contacts_raw)
Return      : int (0 == success, -1 == failed)
Side effect :
Author      : zhq
Date        : 2018.9.4
********************************************************************************/

int req_delete_contacts(char *username, char *contact_name, gboolean (*callback)(gpointer))
{
    cb_req_delete_contacts = callback;

    char escaped_username[1024], escaped_contact_name[1024];
    escape_string(username, escaped_username);
    escape_string(contact_name, escaped_contact_name); ///转义

    //构造数据包；
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;

    strcat(escaped_username, ",");
    memcpy(pkg->payload, "/6", strlen("/6"));
    memcpy(pkg->payload + strlen("/6"), escaped_username, strlen(escaped_username));
    memcpy(pkg->payload + strlen(escaped_username) + strlen("/6"), escaped_contact_name, strlen(escaped_contact_name));
    pkg->len = strlen("/6") + strlen(escaped_username) + strlen(escaped_contact_name);

    //发送
    write(sockfd, pkg, sizeof(Package) + pkg->len);
    return 0;
}

/********************************************************************************
Description : Add_Contacts
Parameter   : char *username  char *contact_name void (*callback)(char *contacts_raw)
Return      : int (0 == success, -1 == failed)
Side effect :
Author      : zhq
Date        : 2018.9.4
********************************************************************************/

int req_add_contacts(char *username, char *contact_name, gboolean (*callback)(gpointer))
{
    cb_req_add_contacts = callback;

    char escaped_username[1024], escaped_contact_name[1024];
    escape_string(username, escaped_username);
    escape_string(contact_name, escaped_contact_name); ///转义

    //构造数据包；
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;

    strcat(escaped_username, ",");
    memcpy(pkg->payload, "/3", strlen("/3"));
    memcpy(pkg->payload + strlen("/3"), escaped_username, strlen(escaped_username));
    memcpy(pkg->payload + strlen(escaped_username) + strlen("/3"), escaped_contact_name, strlen(escaped_contact_name));
    pkg->len = strlen("/3") + strlen(escaped_username) + strlen(escaped_contact_name);

    //发送
    write(sockfd, pkg, sizeof(Package) + pkg->len);
    return 0;
}

/********************************************************************************
Description : Get_Chat_record_Public
Parameter   : char *Group_Id, void (*callback)(char *contacts_raw)
Return      : int (0 == success, -1 == failed)
Side effect :
Author      : zhq
Date        : 2018.9.4
********************************************************************************/

int Get_Chat_record_Public(unsigned int GroupId, char *msg)
{
    //转义;
    char Group_Id[100], escape_msg[1024];
    snprintf(Group_Id, 5, "%d", GroupId);
    escape_string(msg, escape_msg);

    //构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;
    //	strcat(Group_Id, ",");
    memcpy(pkg->payload, "/4", strlen("/4"));
    memcpy(pkg->payload + strlen("/4"), Group_Id, strlen(Group_Id));
    memcpy(pkg->payload + strlen(Group_Id) + strlen("/4"), escape_msg, strlen(escape_msg));
    pkg->len = strlen("/4") + strlen(escape_msg) + strlen(Group_Id);

    //发送
    write(sockfd, pkg, sizeof(Package) + pkg->len);
    return 0;
}

/********************************************************************************
Description : File_private_for_server
Parameter   : char *address, char *username
Return      : int (0 == success, -1 == failed)
Side effect :
Author      : zhq
Date        : 2018.9.5
********************************************************************************/
int File_private(char *address, char *username)
{

    //转义;
    strcpy(address_file, address);
    char escape_username[1024];
    escape_string(username, escape_username);
    //构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;
    memcpy(pkg->payload, "#", strlen("#"));
    memcpy(pkg->payload + strlen("#"), escape_username, strlen(escape_username));
    //memcpy(pkg->payload + strlen(Group_Id) + strlen("/4"), escape_msg, strlen(escape_msg));
    pkg->len = strlen("#") + strlen(escape_username);
    //发送
    write(sockfd, pkg, sizeof(Package) + pkg->len);
    return 0;
}

/********************************************************************************
Description : File_private_for_target
Parameter   : char *ip;
Return      : none
Side effect :
Author      : zhq
Date        : 2018.9.5
********************************************************************************/

static void *thread_send_file(void *ip)
{
    int sd, rn;
    struct sockaddr_in client, server;
    char buf[1024];
    FILE *fq;
    int len, opt = 1;
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(4000);
    inet_aton((char *)ip, &server.sin_addr);
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (connect(sd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connect");
        close(sd);
        exit(1);
    }
    if ((fq = fopen(address_file, "rb")) == NULL)
    {
        perror("File open");
        close(sd);
        exit(1);
    }

    bzero(buf, sizeof(buf));

    while (!feof(fq))
    {
        len = fread(buf, 1, 1024, fq);
        if (len != write(sd, buf, len))
        {
            perror("write");
            break;
        }
    }
    close(sd);
    fclose(fq);
    return 0;
}

void reg_cb_connection_lost(gboolean (*callback)(gpointer))
{
    cb_connection_lost = callback;
}

/********************************************************************************
Description : 客户机接收线程
Parameter   : 
Return      : 
Side effect :
Author      : zyc
Date        : 2018.9.5
********************************************************************************/
static void *pthread(void *arg)
{
    int first_contact = 1;
    std::cout << "接收线程，启动！" << std::endl;

    char msg[1024];
    static Entity self;         //自己的信息
    static Entity contacts[20]; //联系人信息

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
                        g_idle_add(cb_connection_lost, (void *)10086);
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

        int sizeofEntity = 38; // 不知为何sizeof(Entity)是40

        // 解包，PostMessage
        Package *pkg = (Package *)msg;
        // /0 login
        if (pkg->payload[0] == '/' && pkg->payload[1] == '0')
        {
            if (pkg->len >= sizeofEntity) //success
            {
                //事件：登录结果
                memcpy(&self, &pkg->payload[2], sizeofEntity);
                g_idle_add(cb_req_authentication, (void *)&self);
            }

            else
                g_idle_add(cb_req_authentication, (void *)0);
        }
        // /1 register
        else if (pkg->payload[0] == '/' && pkg->payload[1] == '1')
        {
            if (pkg->payload[2] == '1') //success
                //事件：注册结果
                g_idle_add(cb_req_register, (void *)1);
            else
                g_idle_add(cb_req_register, (void *)0);
        }
        // /2 contacts, /5 search , /3 add, /6 delete
        else if (pkg->payload[0] == '/' && (pkg->payload[1] == '2' || pkg->payload[1] == '3' || pkg->payload[1] == '5' || pkg->payload[1] == '6'))
        {
            for (int i = 0; i < (pkg->len / sizeofEntity); i++)
            {
                memcpy(&contacts[i], &pkg->payload[2 + i * sizeofEntity], sizeofEntity);
            }

            //事件：好友列表已更新
            g_idle_add(cb_req_contacts, contacts);
        }
        //群聊消息
        else if (pkg->payload[0] == '$' && pkg->payload[1] == '$')
        {
            g_idle_add(cb_recv_multicast, &pkg->payload[2]); //风险操作：pkg生命周期？
        }
        // 私聊消息
        else
        {
            g_idle_add(cb_recv_unicast, &pkg->payload[2]); //风险操作：pkg生命周期？
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
/********************************************************************************
Description : Divide_String
Parameter   : char *msg, char **Divided_Strings
Return      : char ** Divided_Strings
Side effect :
Author      : zhq
Date        : 2018.9.4
********************************************************************************/
char **Divide_String(char *msg, char **Divided_Strings)
{
    int flag = 1, i_msg, i_Div, cnt = 0, strings_num = 0; //cnt 代表要删除的个数;
    for (i_msg = 0, i_Div = 0; i_msg < strlen(msg); i_msg++, i_Div++)
    {
        if (msg[i_msg] == '\\' && flag == 1)
        {
            i_msg++;
            Divided_Strings[strings_num][i_Div] = msg[i_msg];
            //          flag=-flag;
        } // 转义前字符,并且完成当前转义
          //      else if(msg[i_msg]=='\\'&&flag==-1){
          //          Divided_Strings[strings_num][i_Div]=msg[i_msg];
          //          flag=-flag;
          //      }            //
        else if (msg[i_msg] == ',' && flag == 1)
        {
            Divided_Strings[strings_num][i_Div] = '\0';
            strings_num++;
            i_Div = -1;
        } // 下一字符串
        else
        {
            Divided_Strings[strings_num][i_Div] = msg[i_msg];
        } // 简单拷贝
    }
    Divided_Strings[strings_num][i_Div] = '\0';
    return Divided_Strings;
}
