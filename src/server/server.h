
/********************************************************************************
 * Files         : serverc.h
 * Description   : the declearation functions of Rosetta server
 * Author        : zpy
 * Last Modified : 2018.9.1
 ********************************************************************************/
#include <sys/types.h>
#include <mysql/mysql.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<pthread.h>
#include<sys/shm.h>
#include<time.h>
#include"database.h"
//#include<gtk/gtk.h>
#define MYPORT    10005                          //server's listen port
#define PORT      8081                          //the begin of srever's chat port
#define CLPORT    8079                          //client's chat port
#define MYKEY   12345
#define SIZE    10240
#define MAX_USER_NUM 10
//int client_sockfd;
int _2_server_sockfd;
int userNum;
char online_user[MAX_USER_NUM][10];
int  ary_sockfd[MAX_USER_NUM];                         //arrar for clients' sockfd
int global_current_sockfd;

/********************************************************************************
 Description : sign up in database
 Prameter    : Function is not completed.
 Return      : int (1 == success, 0 == failed)
 Side effect :
 Author      : zpy
 Date        : 2018.9.2
 ********************************************************************************/
int check_signup(const char* line);
/********************************************************************************
 Description : login in database
 Parameter    : Function is not completed.
 Return      : int (1 == success, 0 == failed)
 Side effect :
 Author      : zpy
 Date        : 2018.9.2
 ********************************************************************************/
int check_login(const char* line);
/********************************************************************************
 Description : check if the user exist
 Prameter    : Function done
 Return      : int (0=no,1=exist)
 Side effect : none
 Author      : zpy
 Date        : 2018.9.2
 ********************************************************************************/
int if_user_exist(const char* line);
/********************************************************************************
 Description : check if the group exist
 Prameter    : Function done
 Return      : int (0=no,1=exist)
 Side effect : none
 Author      : zpy
 Date        : 2018.9.3
 ********************************************************************************/
int if_group_exist(const char* line);
/********************************************************************************
 Description : search avator from db
 Prameter    : Function done
 Return      : int (numbers of avator)        considering
 Side effect : none
 Author      : zpy
 Date        : 2018.9.3
 ********************************************************************************/
int search_avator_from_db(const char* username);
/********************************************************************************
 Description : change default avator in db
 Prameter    : Function done           can be expanded
 Return      : void
 Side effect : none
 Author      : zpy
 Date        : 2018.9.3
 ********************************************************************************/
void change_avator_in_db(const char* username,int id);
/********************************************************************************
 Description : add friend in database
 Prameter    : Function done
 Return      : void
 Side effect : none
 Author      : zpy
 Date        : 2018.9.2
 ********************************************************************************/
void insert_friend_into_database(const char* name1,const char* name2);
/********************************************************************************
 Description : get group users from db
 Prameter    : Function done
 Return      : int(number of group members)
 Side effect : none
 Author      : zpy
 Date        : 2018.9.2
 ********************************************************************************/
int get_group_users_from_db(const char* groupname, char list[][100]);
/********************************************************************************
 Description : get friend list from db
 Prameter    : Function done
 Return      : int (number of friends)
 Side effect : none
 Author      : zpy
 Date        : 2018.9.2
 ********************************************************************************/
int get_friend_list_from_db(const char* name,char* list);
/********************************************************************************
 Description : get group list from db
 Prameter    : Function done
 Return      : int (number of groups)
 Side effect : none
 Author      : zpy
 Date        : 2018.9.3
 ********************************************************************************/
int get_group_list_from_db(const char* name,char* list);
/********************************************************************************
 Description : search user from db
 Prameter    : Function done
 Return      : int (number of users)（due to the fuzzy search, there may be more than one  targets)
 Side effect : none
 Author      : zpy
 Date        : 2018.9.3
 ********************************************************************************/
int search_user_from_db(const char* name,char* list);
/********************************************************************************
 * Description : int to string
 * Prameter    : int n:a in number   const char* str:destination
 * Return      : void
 * Side effect : Function is completed.
 * Author      : zpy
 * Date        : 2018.9.1
 ********************************************************************************/
void itoa(int n,char*str);
/********************************************************************************
 * Description : create a group
 * Prameter    : user_name   group_name
 * Return      : void
 * Side effect : Function is completed.
 * Author      : zpy
 * Date        : 2018.9.6
 ********************************************************************************/
void create_group_into_database(const char* name1,const char* name2);
/********************************************************************************
 * Description : quit a group
 * Prameter    : user_name   group_name
 * Return      : void
 * Side effect : Function is completed.
 * Author      : zpy
 * Date        : 2018.9.6
 ********************************************************************************/
void quit_group_into_database(const char* name1,const char* name2);
/********************************************************************************
 * Description : ；判断用户是否在线
 * Prameter    : char*,the name which you want to check
 * Return      : -1               :not online
 non-negative int  :the position in the array online_user
 * Side effect : Function is completed.
 * Author      : zpy
 * Date        : 2018.9.1
 ********************************************************************************/
int if_user_online(const char *name);
/********************************************************************************
 * Description : 用于处理收到的命令，只处理，不发送
 * Prameter    : (1)指向对面用户 sockfd 的指针，不向对面发送消息的时候赋值为0
 *             : (2)当前用户在 online_user 数组中的下标，用来知道是谁发的命令
 *             : (3)指向发给自己的消息的指针
 *             : (4)指向发给对面用户消息的指针
 *             : (5)原始消息
 * Return      :无意义
 * Side effect : 可能会修改opp_sockfd
 * Author      : zpy
 * Date        : 2018.9.2
 ********************************************************************************/
int process_command(int* p_to_sockfd,int current_userID,char* p_msg_to_slef,char*p_msg_to_opp,const char*original_msg);
/********************************************************************************
 * Description : 用于处理收到的消息，只处理，不发送
 * Prameter    : 参考 process_command
 * Return      : 无意义
 * Side effect : Function is completed.
 * Author      : zpy
 * Date        : 2018.9.2
 ********************************************************************************/
int process_msg(int* p_to_sockfd,int current_userID,char* p_msg_to_slef,char*p_msg_to_opp,const char* original_msg);
int process_gmsg(int* p_to_sockfd,int current_userID,char* p_msg_to_slef,char*p_msg_to_opp,const char*original_msg);
/********************************************************************************
 * Description : 处理文件消息 P2P
 * Prameter    : 参考 process_command
 * Return      : 无意义
 * Side effect : 参考 process_command
 * Author      : zpy
 * Date        : 2018.9.2
 ********************************************************************************/
int process_file(int* p_to_sockfd,int current_userID,char *p_msg_to_opp,const char*original_msg,int len,char*add);
/********************************************************************************
 * Description : 用于开启子线程 只收发 处理在子函数中进行
 * Prameter    : void* 赋值 NULL 即可
 * Return      : void* 当用户登出或掉线时函数返回，结束线程，返还资源
 * Side effect : 子线程在登出时必须关闭子线程
 * Author      : zpy
 * Date        : 2018.9.2
 ********************************************************************************/
void* _pthread_entrance(void* p);
