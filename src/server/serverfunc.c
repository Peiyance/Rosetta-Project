/********************************************************************************
 * Files         : serverfunc.c
 * Description   : the API functions of Rosetta server
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
int check_signup(const char* line)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    //making connection
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    
    //get name and password
    char user_name[20];
    char user_pw[20];
    int i =9,j=0;
    while (line[i]&&line[i]!=';')
    {
        user_name[j]=line[i];
        i++; j++;
    }
    user_name[j]=0;
    i+=10; j=0;
    while (line[i]&&line[i]!=';')
    {
        user_pw[j]=line[i];
        i++; j++;
    }
    user_pw[j]=0;
    
    //check if the username exit
    char comm[1024];
    int ans=0;
    sprintf(comm,"select * from alluser where username = '%s';",user_name);
    if (mysql_query(conn,comm))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        ans++;                            //remember the output number!!
    }
    if(ans!=0)                            //if user_name exist!
    {
        //printf("username exist!\n");
        mysql_free_result(res);
        mysql_close(conn);
        return 0;
    }
    else  //start insertion
    {
        char comm1[1024] ="\0";
        sprintf(comm1,"insert into alluser values ('%s','%s');",user_name,user_pw);
        if (mysql_query(conn,comm1))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            //exit(1);
        }
        char comm2[1024]="\0";
        sprintf(comm2,"insert into avator values ('%s',0)",user_name);
        if (mysql_query(conn,comm2));
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            //exit(1);
        }
        mysql_free_result(res);
        mysql_close(conn);
        return 1;
    }
}

/********************************************************************************
 Description : login in database
 Parameter    : Function is not completed.
 Return      : int (1 == success, 0 == failed)
 Side effect :
 Author      : zpy
 Date        : 2018.9.2
 ********************************************************************************/
int check_login(const char* line)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    
    //get name and password
    char user_name[20];
    char user_pw[20];
    int i=9,j=0;
    while (line[i]&&line[i]!=';')
    {
        user_name[j]=line[i];
        i++; j++;
    }
    user_name[j]=0;
    i+=10; j=0;
    while (line[i]&&line[i]!=';')
    {
        user_pw[j]=line[i];
        i++; j++;
    }
    user_pw[j]=0;
    
    //select data
    char comm[1024]="\0";
    int ans=0;
    sprintf(comm,"select * from alluser where username = '%s' and password = '%s';",
            user_name,user_pw);
    if (mysql_query(conn,comm))
    {
        ///fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        ans++;
        //printf("%s %s\n", row[0],row[1]); //remember the output number!!
    }
    if(ans==0)
    {
        mysql_free_result(res);
        mysql_close(conn);
        return 0;
    }
    
    //ending
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}

/********************************************************************************
 Description : check if the user exist
 Prameter    : Function done
 Return      : int (0=no,1=exist)
 Side effect : none
 Author      : zpy
 Date        : 2018.9.2
 ********************************************************************************/
int if_user_exist(const char* line)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    
    //start deletion
    char comm[1024] ="\0";
    sprintf(comm,"select * from alluser where username = '%s';",line);
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    int ans=0;
    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        ans++;
        //printf("%s %s\n", row[0],row[1]); //remember the output number!!
    }
    if(ans==0)
    {
        mysql_free_result(res);
        mysql_close(conn);
        return 0;
    }
    
    //ending
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}

/********************************************************************************
 Description : check if the group exist
 Prameter    : Function done
 Return      : int (0=no,1=exist)
 Side effect : none
 Author      : zpy
 Date        : 2018.9.3
 ********************************************************************************/
int if_group_exist(const char* line)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    
    //start deletion
    char comm[1024] ="\0";
    sprintf(comm,"select * from allgroup where groupname = '%s';",line);
    
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    int ans=0;
    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        ans++;
        //printf("%s %s\n", row[0],row[1]); //remember the output number!!
    }
    if(ans==0)
    {
        mysql_free_result(res);
        mysql_close(conn);
        return 0;
    }
    
    //ending
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}


/********************************************************************************
 Description : search avator from db
 Prameter    : Function done
 Return      : int (numbers of avator)        considering
 Side effect : none
 Author      : zpy
 Date        : 2018.9.3
 ********************************************************************************/
int search_avator_from_db(const char* username)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    
    //start deletion
    char comm[1024] ="\0";
    sprintf(comm,"select number from avator where username = '%s';",username);
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    int ans;
    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        ans = atoi(row[0]);
        //printf("%s %s\n", row[0],row[1]); //remember the output number!!
    }
    mysql_free_result(res);
    mysql_close(conn);
    return ans;
}

/********************************************************************************
 Description : change default avator in db
 Prameter    : Function done           can be expanded
 Return      : void
 Side effect : none
 Author      : zpy
 Date        : 2018.9.3
 ********************************************************************************/
void change_avator_in_db(const char* username,int id)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    
    //start deletion
    char comm[1024] ="\0";
    sprintf(comm,"update avator set number = %d where username = '%s';",id,username);
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    mysql_close(conn);
    return;
}

/********************************************************************************
 Description : add friend in database
 Prameter    : Function done
 Return      : void
 Side effect : none
 Author      : zpy
 Date        : 2018.9.2
 ********************************************************************************/
void insert_friend_into_database(const char* name1,const char* name2)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        // fprintf(stderr, "%s\n", mysql_error(conn));
        // exit(1);
    }
    
    //check if the username exit
    char comm[1024];
    int ans=0;
    sprintf(comm,"select * from friend_list where p1 = '%s' and p2 = '%s';",name1,name2);
    if (mysql_query(conn,comm))
    {
        // fprintf(stderr, "%s\n", mysql_error(conn));
        // exit(1);
    }
    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        //printf("%s %s\n", row[0],row[1]); //remember the output number!!
        ans++;
    }
    if(ans!=0)                            //if user_name exist!
    {
        mysql_free_result(res);
        mysql_close(conn);
        return ;
    }
    else  //start insertion
    {
        char comm1[1024] ="\0";
        sprintf(comm1,"insert into friend_list values ('%s','%s');",name1,name2);
        if (mysql_query(conn,comm1))
        {
            //fprintf(stderr, "%s\n", mysql_error(conn));
            //exit(1);
        }
        char comm2[1024] ="\0";
        sprintf(comm2,"insert into friend_list values ('%s','%s');",name2,name1);
        if (mysql_query(conn,comm2))
        {
            // fprintf(stderr, "%s\n", mysql_error(conn));
            //exit(1);
        }
    }
    mysql_free_result(res);
    mysql_close(conn);
    return;
}




/*int get_friend_list_from_db(const char* name,char* list)
 {
 //making connection
 MYSQL *conn;
 MYSQL_RES *res;
 MYSQL_ROW row;
 conn = mysql_init(NULL);
 if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
 {
 // fprintf(stderr, "%s\n", mysql_error(conn));
 //exit(1);
 }
 
 //check if the username exit
 char comm[1024]="\0",line[2000]="\0";
 //memset(line,0,sizeof(line));
 int ans=0;
 sprintf(comm,"select p2 from friend_list where p1 = '%s';",name);
 if (mysql_query(conn,comm))
 {
 //fprintf(stderr, "%s\n", mysql_error(conn));
 //exit(1);
 }
 res = mysql_use_result(conn);
 while ((row = mysql_fetch_row(res)) != NULL)
 {
 //printf("%s\n", row[0]); //remember the output number!!
 ans++;
 strcat(line,row[0]);
 strcat(line,"\n");
 }
 //strcat(line,0);
 strcpy(list,line);
 mysql_free_result(res);
 mysql_close(conn);
 return ans;
 }*/

/********************************************************************************
 Description : get group users from db
 Prameter    : Function done
 Return      : int(number of group members)
 Side effect : none
 Author      : zpy
 Date        : 2018.9.2
 ********************************************************************************/
int get_group_users_from_db(const char* groupname, char list[][100])
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        // fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    
    char comm[1024]="\0",line[2000]="\0";
    //memset(line,0,
    int ans=0;
    
    sprintf(comm,"select username from alluser a, group_list g where g.p2 = 'g1' and g.p1 = a.username ;",groupname);
    
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        //printf("%s %s\n", row[0]); //remember the output number!!
        ans++;
        strcpy(list[ans-1],row[0]);
    }
    //strcat(line,0);
    //strcpy(list,line);
    mysql_free_result(res);
    mysql_close(conn);
    return ans;
}

/********************************************************************************
 Description : get friend list from db
 Prameter    : Function done
 Return      : int (number of friends)
 Side effect : none
 Author      : zpy
 Date        : 2018.9.2
 ********************************************************************************/
int get_friend_list_from_db(const char* name,char* list)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        // fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    
    //check if the username exit
    char comm[1024]="\0",line[2000]="\0";
    //memset(line,0,sizeof(line));
    int ans=0;
    sprintf(comm,"select friend_list.p2,avator.number from friend_list,avator where friend_list.p2=avator.username and p1 = '%s';",name);
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        //printf("%s %s\n", row[0]); //remember the output number!!
        ans++;
        strcat(line,row[0]);
        strcat(line," ");
        strcat(line,row[1]);
        strcat(line," ");
        //strcat(line,row[2]);
        //strcat(line," ");

    }
    //strcat(line,0);
    strcpy(list,line);
    mysql_free_result(res);
    mysql_close(conn);
    return ans;
}

/********************************************************************************
 Description : get group list from db
 Prameter    : Function done
 Return      : int (number of groups)
 Side effect : none
 Author      : zpy
 Date        : 2018.9.3
 ********************************************************************************/
int get_group_list_from_db(const char* name,char* list)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        // fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    
    //check if the username exit
    char comm[1024]="\0",line[2000]="\0";
    //memset(line,0,sizeof(line));
    int ans=0;
    sprintf(comm,"select group_list.p2, group_avator.number from group_list, group_avator where group_list.p2=group_avator.groupname and group_list.p1 = '%s';",name);
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        //printf("%s %s\n", row[0]); //remember the output number!!
        ans++;
        strcat(line,row[0]);
        strcat(line,"\n");
        strcat(line,row[1]);
        strcat(line,"\n");
    }
    //strcat(line,0);
    strcpy(list,line);
    mysql_free_result(res);
    mysql_close(conn);
    return ans;
}
/********************************************************************************
 Description : search user from db
 Prameter    : Function done
 Return      : int (number of users)（due to the fuzzy search, there may be more than one  targets)
 Side effect : none
 Author      : zpy
 Date        : 2018.9.3
 ********************************************************************************/
int search_user_from_db(const char* name,char* list)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        // fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    
    //check if the username exit
    char comm[1024]="\0",line[2000]="\0";
    //memset(line,0,sizeof(line));
    int ans=0;
    sprintf(comm,"select * from avator where username like '%%%s%%';",name);  //fuzzy search
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    res = mysql_use_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        //printf("%s %s\n", row[0],row[0]); //remember the output number!!
        ans++;
        strcat(line,row[0]);
        strcat(line,"\n");
        strcat(line,row[1]);
        strcat(line,"\n");
    }
    //strcat(line,0);
    strcpy(list,line);
    mysql_free_result(res);
    mysql_close(conn);
    return ans;
}
/********************************************************************************
 * Description : int to string
 * Prameter    : int n:a in number   const char* str:destination
 * Return      : void
 * Side effect : Function is not completed.
 * Author      : zpy
 * Date        : 2018.9.1
 ********************************************************************************/
void itoa(int n,char*str)
{
    str[0]=(n/1000)+'0';
    n%=1000;
    str[1]=(n/100)+'0';
    n%=100;
    str[2]=(n/10)+'0';
    str[3]=n%10+'0';
    str[4]=0;
}void create_group_into_database(const char* name1,const char* name2)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        // fprintf(stderr, "%s\n", mysql_error(conn));
        // exit(1);
    }
    
    //check if the username exit
    //char comm[1024];
    int ans=0;
   // sprintf(comm,"select * from friend_list where p1 = '%s' and p2 = '%s';",name1,name2);
   // if (mysql_query(conn,comm))
   // {
        // fprintf(stderr, "%s\n", mysql_error(conn));
        // exit(1);
   // }
   // res = mysql_use_result(conn);
   // while ((row = mysql_fetch_row(res)) != NULL)
   // {
        //printf("%s %s\n", row[0],row[1]); //remember the output number!!
    //    ans++;
   // }
   // if(ans!=0)                            //if user_name exist!
    //{
    //    mysql_free_result(res);
    //    mysql_close(conn);
    //    return ;
   // }
   // else  //start insertion
   // {
        char comm1[1024] ="\0";
        sprintf(comm1,"insert into group_list values ('%s','%s');",name1,name2);
        if (mysql_query(conn,comm1))
        {
            //fprintf(stderr, "%s\n", mysql_error(conn));
            //exit(1);
        }
        char comm2[1024] ="\0";
        sprintf(comm2,"insert into allgroup values ('%s');",name2);
        if (mysql_query(conn,comm2))
        {
            // fprintf(stderr, "%s\n", mysql_error(conn));
            //exit(1);
        }
   // }
   // mysql_free_result(res);
    mysql_close(conn);
    return;
}

void quit_group_into_database(const char* name1,const char* name2)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        // fprintf(stderr, "%s\n", mysql_error(conn));
        // exit(1);
    }
    
    //check if the username exit
    //char comm[1024];
    int ans=0;
    // sprintf(comm,"select * from friend_list where p1 = '%s' and p2 = '%s';",name1,name2);
    // if (mysql_query(conn,comm))
    // {
    // fprintf(stderr, "%s\n", mysql_error(conn));
    // exit(1);
    // }
    // res = mysql_use_result(conn);
    // while ((row = mysql_fetch_row(res)) != NULL)
    // {
    //printf("%s %s\n", row[0],row[1]); //remember the output number!!
    //    ans++;
    // }
    // if(ans!=0)                            //if user_name exist!
    //{
    //    mysql_free_result(res);
    //    mysql_close(conn);
    //    return ;
    // }
    // else  //start insertion
    // {
    char comm1[1024] ="\0";
    sprintf(comm1,"delete from group_list where p1='%s';",name1);
    if (mysql_query(conn,comm1))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    /*char comm2[1024] ="\0";
    sprintf(comm2,"insert into allgroup values ('%s');",name2);
    if (mysql_query(conn,comm2))
    {
        // fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }*/
    // }
    // mysql_free_result(res);
    mysql_close(conn);
    return;
}


/********************************************************************************
 * Description : ；判断用户是否在线
 * Prameter    : char*,the name which you want to check
 * Return      : -1               :not online
 non-negative int  :the position in the array online_user
 * Side effect : Function is not completed.
 * Author      : zpy
 * Date        : 2018.9.1
 ********************************************************************************/
int if_user_online(const char *name)
{
    int i;
    for(i=0;i<MAX_USER_NUM;i++)
        {
	if(strcmp(online_user[i],name)==0)
            break;
printf("___+++++____%s\n",online_user[i]);
	}	
    if(i==MAX_USER_NUM)
        return -1;
    return i;
}
