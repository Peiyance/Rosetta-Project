#include <iostream>
#include <cstring>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <gtk/gtk.h>
#include "connector.h"
#include "common.h"

typedef struct
{
    int count;
    Entity content[20]; //联系人信息
} Contacts;

Entity self; //自己的信息
Contacts contacts; // 好友、群列表

static void *pthread(void *arg);         // socket接收线程 - 常驻
static void *thread_send_file(void *ip); // 文件发送线程
static void *thread_recv_file(void *ip); // 文件接收 - 常驻
char *escape_string(char *msg, char *escaped);
int write_socket(char *payload);
void reconnect();

namespace connector
{
int sockfd, sock_listen;
sockaddr_in sin, sin_listen;
int send_package_sequence = 0; //发送封包编号计数器
int recv_package_sequence = 0; //接收封包编号计数器
char address_file[500], target_ip[100];

struct Package
{
    int package_sequence;
    int ver;
    int len;
    char payload[0];
};

// 我们使用同步callback，类似于win中的PostMessage
gboolean (*cb_req_authentication)(gpointer) = NULL;
gboolean (*cb_req_register)(gpointer) = NULL;

gboolean (*cb_req_contacts)(gpointer) = NULL;
gboolean (*cb_req_search_contacts)(gpointer) = NULL;
gboolean (*cb_req_add_contacts)(gpointer) = NULL;
gboolean (*cb_req_delete_contacts)(gpointer) = NULL;

gboolean (*cb_req_groups)(gpointer) = NULL;
gboolean (*cb_req_search_group)(gpointer) = NULL;

gboolean (*cb_recv_unicast)(gpointer) = NULL;
gboolean (*cb_recv_multicast)(gpointer) = NULL;
gboolean (*cb_chat_record_unicast)(gpointer) = NULL;
gboolean (*cb_chat_record_multicast)(gpointer) = NULL;

} // namespace connector
using namespace connector;



// Interfaces
// returns sockfd. create a thread maintaining the long-term TCP.
int init_connector(char remoteIP[], short remotePort)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);      //tcp
    sock_listen = socket(AF_INET, SOCK_STREAM, 0); //tcp
    if (sockfd <= 0)
        return -1;

    sin.sin_family = AF_INET;           //ipv4
    sin.sin_port = htons(remotePort);   //port
    inet_aton(remoteIP, &sin.sin_addr); //addr

    sin_listen.sin_family = AF_INET;                //ipv4
    sin_listen.sin_port = htons(8371);              //port
    sin_listen.sin_addr.s_addr = htonl(INADDR_ANY); // listen on 0.0.0.0

    // int opt = 1;
    // setsockopt(sock_listen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // int ret = bind(sock_listen, (sockaddr *)&sin_listen, sizeof(sin_listen));
    // if (ret < 0)
    // {
    //     printf("unable to bind file_listen");
    //     return -1;
    // }

    listen(sock_listen, 4);

    //pthread_t thread1, thread3;
    // pthread_create(&thread1, NULL, pthread, (void *)0);
    // pthread_create(&thread3, NULL, thread_recv_file, (void *)0);
    g_thread_create(pthread, 0, TRUE, NULL);
    // g_thread_create(thread_recv_file, 0, TRUE, NULL);

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

    char payload[1024];
    bzero(payload, sizeof(payload));

    strcat(payload, "/0*username:");
    strcat(payload, str_username);
    strcat(payload, ";password:");
    strcat(payload, str_password);
    strcat(payload, ";");

    write_socket(payload);
    return 0;
}

int req_register(char *str_username, char *str_password, gboolean (*callback)(gpointer))
{
    cb_req_register = callback; // reg callback

    char payload[1024];
    bzero(payload, sizeof(payload));

    strcat(payload, "/1*username:");
    strcat(payload, str_username);
    strcat(payload, ";password:");
    strcat(payload, str_password);
    strcat(payload, ";");

    write_socket(payload);
    return 0;
}

int req_contacts(char *username, gboolean (*callback)(gpointer))
{
    cb_req_contacts = callback; // reg callback

    char payload[1024];
    bzero(payload, sizeof(payload));

    strcat(payload, "/2:");
    strcat(payload, username);
    strcat(payload, "*");

    write_socket(payload);
    return 0;
}

/********************************************************************************
Description : Private_talk
Parameter   : char *msg, char *str_peer
Return      : int (0 == success, -1 == failed)
Side effect :
Author      : zhq & zyc
Date        : 2018.9.4
********************************************************************************/
int post_msg_unicast(char *str_peer, char *msg)
{
    char payload[1024];
    bzero(payload, sizeof(payload));

    strcat(payload, str_peer);
    strcat(payload, ":");
    strcat(payload, msg);

    write_socket(payload);
    return 0;
}

/********************************************************************************
Description : Public_talk
Parameter   : char *msg, unsigned int Group_Id
Return      : int (0 == success, -1 == failed)
Side effect :
Author      : zyc
Date        : 2018.9.4
********************************************************************************/
int post_msg_multicast(char *groupName, char *msg)
{
    char payload[1024];
    bzero(payload, sizeof(payload));

    strcat(payload, "$$");
    strcat(payload, groupName);
    strcat(payload, ":");
    strcat(payload, msg);

    write_socket(payload);
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
Author      : zhq & zyc
Date        : 2018.9.4
********************************************************************************/
int req_search_contacts(char *keyword, gboolean (*callback)(gpointer))
{
    cb_req_search_contacts = callback;

    char payload[1024];
    bzero(payload, sizeof(payload));

    strcat(payload, "/5:");
    strcat(payload, keyword);
    strcat(payload, "*");

    write_socket(payload);
    return 0;
}
/********************************************************************************
Description : Add_Contacts
Parameter   : char *username  char *contact_name void (*callback)(char *contacts_raw)
Return      : int (0 == success, -1 == failed)
Side effect :
Author      : zhq & zyc
Date        : 2018.9.4
********************************************************************************/

int req_add_contacts(char *username, char *contact_name, gboolean (*callback)(gpointer))
{
    cb_req_add_contacts = callback;

    char payload[1024];
    bzero(payload, sizeof(payload));

    strcat(payload, "/3:");
    strcat(payload, contact_name);
    strcat(payload, "*");

    // 构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + sizeof(payload)];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;
    pkg->len = strlen(payload);
    memcpy(pkg->payload, payload, sizeof(payload));

    //发送
    write(sockfd, pkg->payload, /*sizeof(Package) +*/ pkg->len);
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
    write(sockfd, pkg->payload, /*sizeof(Package) +*/ pkg->len);
    return 0;
}



int req_groups(char *username, gboolean (*callback)(gpointer))
{
    cb_req_groups = callback; // reg callback

    char payload[1024];
    bzero(payload, sizeof(payload));

    strcat(payload, "/9:");
    strcat(payload, username);
    strcat(payload, "*");

    write_socket(payload);
    return 0;
}

int req_search_group(char *keyword, gboolean (*callback)(gpointer))
{
    cb_req_search_group = callback;

    char payload[1024];
    bzero(payload, sizeof(payload));

    strcat(payload, "/a:");
    strcat(payload, keyword);
    strcat(payload, "*");

    write_socket(payload);
    return 0;
}

int req_create_group(char *username, char *groupName)
{
    //cb_req_create_groups = callback; // reg callback
    char payload[1024];
    bzero(payload, sizeof(payload));

    strcat(payload, "/b");
    strcat(payload, username);
    strcat(payload, ":");
    strcat(payload, groupName);
    strcat(payload, "*");

    write_socket(payload);
    return 0;
}

int req_quit_group(char *username, char *groupName)
{
    char payload[1024];
    bzero(payload, sizeof(payload));

    strcat(payload, "/q");
    strcat(payload, username);
    strcat(payload, ":");
    strcat(payload, groupName);
    strcat(payload, "*");

    write_socket(payload);
    return 0;
}

/********************************************************************************
Description : Get_Chat_record_Private
Author      : zhq
Date        : 2018.9.4
********************************************************************************/
int req_chat_record_unicast(char *peer, gboolean (*callback)(gpointer))
{
    cb_chat_record_unicast = callback;

    //构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;

    memcpy(pkg->payload, "/4", strlen("/4"));
    memcpy(pkg->payload + 2, peer, strlen(peer));
    pkg->len = 2 + strlen(peer);

    //发送
    write(sockfd, pkg->payload, /*sizeof(Package) +*/ pkg->len);
    return 0;
}


int req_chat_record_multicast(unsigned int groupId, gboolean (*callback)(gpointer))
{
    cb_chat_record_multicast = callback;

    //构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;

    memcpy(pkg->payload, "/4", strlen("/4"));
    memcpy(pkg->payload + 2, &groupId, sizeof(groupId));
    pkg->len = 2 + sizeof(groupId);

    //发送
    write(sockfd, pkg->payload, /*sizeof(Package) +*/ pkg->len);
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
    write(sockfd, pkg->payload, /*sizeof(Package) +*/ pkg->len);
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
    server.sin_port = htons(8371);
    //strcpy(buf,ip);
    inet_aton((char *)ip, &server.sin_addr);
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (connect(sd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("!!!!");
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

    char *pName;
    pName = strrchr(address_file, '/');
    if (pName == 0)
        pName = address_file;
    else
        pName++;
    write(sd, pName, strlen(pName));

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

// recieve file.
// zyc
static void *thread_recv_file(void *ip)
{
    sockaddr_in sin;
    int sin_len = 0;
    int sock_sender;

    while (1)
    {
        sock_sender = accept(sock_listen, (sockaddr *)&sin, (socklen_t *)&sin_len);

        char fileName[500];
        read(sock_sender, fileName, 500);

        FILE *fp = fopen(fileName, "w");
        int rn = 0;
        char buf[1030];
        while (1)
        {
            rn = read(sock_sender, buf, 1030);
            if (rn == 0)
                break;
            fwrite(buf, 1, rn, fp);
        }
        printf("finish file recieving");

        close(sock_sender);
        fclose(fp);
    }
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
    std::cout << "接收线程，启动！" << std::endl;

    //char msg[1024];
    int MAX_BUF = 1024;
    Package *pkg = (Package *)new char[sizeof(Package) + MAX_BUF];

    for (;;)
    {
        memset(pkg, 0, sizeof(pkg + MAX_BUF));
        int len = read(sockfd, pkg->payload, MAX_BUF);

        //断线重连
        if (len <= 0)
        {
            reconnect();
            continue;
        }
        //add \0 at the end
        pkg->payload[len] = '\0';
        //calc payload len
        pkg->len = strlen(pkg->payload);

        /* for debugging */
        char msg[1024];
        //memset(msg,0,sizeof(MAX_BUF));
        strcpy(msg, pkg->payload);

        int sizeofEntity = 38; // 不知为何sizeof(Entity)是40

        // 解包，PostMessage
        //Package *pkg = (Package *)msg;
        // /0 login
        if (pkg->payload[0] == '/' && pkg->payload[1] == '0')
        {

            if (pkg->len >= 4) //success
            {
                sscanf(msg + 3, "%s%d", self.nickname, &self.avatar_id);
                //strcpy(self.nickname, "myusername!");
                //self.avatar_id = 3;
                //事件：登陆结果;
                g_idle_add(cb_req_authentication, (void *)&self);
            }

            else
            {

                g_idle_add(cb_req_authentication, (void *)0);
            }
        }
        // /1 register
        else if (pkg->payload[0] == '/' && pkg->payload[1] == '1')
        {

            if (pkg->payload[2] == '1') //success
            //事件：注册结果
            {
                g_idle_add(cb_req_register, (void *)1);
            }
            else
            {
                g_idle_add(cb_req_register, (void *)0);
            }
        }
        // /2 contacts, /5 search , /3 add, /6 delete   ///////////////////未完成
        else if (pkg->payload[0] == '/' && (pkg->payload[1] == '2' || pkg->payload[1] == '3' || pkg->payload[1] == '5' || pkg->payload[1] == '6'))
        {

            int cnt = 0;
            int offset = 4, bytes_read = 0;
            sscanf(msg + offset, "%d%n", &cnt, &bytes_read);
            offset += bytes_read;
            //offset++;

            for (int i = 0; i < cnt; i++)
            {
                sscanf(msg + offset, "%s%d%n", contacts.content[i].nickname, &contacts.content[i].avatar_id, &bytes_read);
                offset += bytes_read;
                //offset++;
            }
            contacts.count = cnt;
            //事件：好友列表已更新
            switch(pkg->payload[1])
            {
                case '2': g_idle_add(cb_req_contacts,&contacts);break;
                case '3': g_idle_add(cb_req_add_contacts,&contacts);break;
                case '5': g_idle_add(cb_req_search_contacts,&contacts);break;
                case '6': g_idle_add(cb_req_delete_contacts,&contacts);break;
            }
        }
        // /9 grouplist, /a search, /b create, /q quit
        else if (pkg->payload[0] == '/' && (pkg->payload[1] == '9' || pkg->payload[1] == 'a' || pkg->payload[1] == 'b' || pkg->payload[1] == 'q'))
        {
            int cnt = 0;
            int offset = 4, bytes_read = 0;
            sscanf(msg + offset, "%d%n", &cnt, &bytes_read);
            offset += bytes_read;
            //offset++;

            for (int i = 0; i < cnt; i++)
            {
                sscanf(msg + offset, "%s%d%n", contacts.content[i].nickname, &contacts.content[i].avatar_id, &bytes_read);
                offset += bytes_read;
                //offset++;
            }
            contacts.count = cnt;
            //事件：group列表已更新
            switch(pkg->payload[1])
            {
                case '9': g_idle_add(cb_req_groups,&contacts);break;
                case 'a': g_idle_add(cb_req_search_group,&contacts);break;
                //case '5': g_idle_add(cb_search_contacts,&contacts);break;
                //case '6': g_idle_add(cb_req_delete_contacts,&contacts);break;
            }
        }
        //传文件的目标ip
        else if (pkg->payload[0] == '#')
        {
            memcpy(&target_ip, &pkg->payload[1], pkg->len - 1);
            pthread_t file_pthread;
            pthread_create(&file_pthread, NULL, &thread_send_file, (void *)target_ip);
            printf(" Done! \n");
        }
        //群聊消息
        else if (pkg->payload[0] == '$')
        {
            g_idle_add(cb_recv_multicast, &pkg->payload[1]); //风险操作：pkg生命周期？
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

/********************************************************************************
Description : raw write socket
Author      : zyc
Date        : 2018.9.6
********************************************************************************/
int write_socket(char *payload)
{
    // 构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;
    pkg->len = strlen(payload);
    memcpy(pkg->payload, payload, pkg->len);

    // 发包 返回
    write(sockfd, pkg->payload, /*sizeof(Package) +*/ pkg->len);
    delete pkg;
    return 0;
}

void reconnect()
{
    for (;;)
    {
        
        printf("[Error] Connection Lost\n");
            
        close(sockfd);
        sockfd = socket(AF_INET, SOCK_STREAM, 0); //tcp

        ioctl(sockfd, FIONBIO, 1); //1:非阻塞 0:阻塞
        int conn = connect(sockfd, (sockaddr *)&sin, sizeof(sin));

        if (conn == 0) //connected
        {
            ioctl(sockfd, FIONBIO, 0);
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
                printf("Connected.\n");

                break;
            }
        }
    }
}

/********************************************************************************
Description : Game
Parameter   : char *msg, char *username
Return      : int (0 == success , -1 == failed)
Side effect :
Author      : zhq
Date        : 2018.9.6
********************************************************************************/

int game(char *board, char *username)
{
    //转义;
    char escaped_username[1024];
    escape_string(username, escaped_username);
    //构造数据包
    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
    pkg->package_sequence = send_package_sequence++;
    pkg->ver = 0x1;
    strcat(board, ";");
    memcpy(pkg->payload, "*", strlen("*"));
    memcpy(pkg->payload + strlen("*"), board, strlen(board));
    memcpy(pkg->payload + strlen("*") + strlen("board"), escaped_username, strlen(escaped_username));
    pkg->len = strlen("*") + strlen(board) + strlen(escaped_username);
    //发送
    write(sockfd, pkg->payload, /*sizeof(Package) +*/ pkg->len);
    return 0;
}