
/********************************************************************************
 * Files         : server.c
 * Description   : the server mode of Rosetts
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
#include"server.h"

#ifndef database_h
#define database_h
#endif
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

struct Package
{
    int package_sequence;
    int ver;
    int len;
    char payload[0];
};


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
int process_command(int* p_to_sockfd,int current_userID,char* p_msg_to_slef,char*p_msg_to_opp,const char*original_msg)
{
    char opp_name[100];
    char self_name[100];
    char msg_to_opp[100];
    char temp[100];
    char temp1[100];
    char temp2[100];//  using in case '2'
    int i;
    int opp_userID;
    int len;
    int friend_num;
    int group_num;
    int trash;
    int avatorID;
    FILE* p_offline_msg=NULL;
    printf("!!!!!!!!!!\n");
    if(original_msg[1]=='0')                     //log in
    {
        printf("~\n");
        if(check_login(original_msg+3))                   //log in success
        {
            *p_to_sockfd=0;
            strcpy(p_msg_to_slef,"/0");
            int gt;
            char lin[100];
            int i=12;
            int j=0;
            while (original_msg[i]&&original_msg[i]!=';')
            {
                lin[j]=original_msg[i];
                i++; j++;
            }
            printf("Are you OK!\n");
            lin[j]='\0';
            gt=search_avator_from_db(lin);
            char gt1[1];
            gt1[0]=gt+'0';
            printf("%c\n",gt1);
            printf("Are you OK!\n");
            strcat(p_msg_to_slef,lin);
            printf("%s\n",p_msg_to_slef);
            strcat(p_msg_to_slef," ");
            strcat(p_msg_to_slef,gt1);
            printf("Are you OK!\n");
            for(i=0;i<80&&original_msg[i+12]!=';';i++)                         //add name into online_user
                self_name[i]=original_msg[i+12];
            self_name[i]=0;
            strcpy(online_user[current_userID],self_name);
            
            printf("%s had log in %d \n",self_name,current_userID);
        }
        else
        {
            *p_to_sockfd=0;
            strcpy(p_msg_to_slef,"/0-1");
        }
    }
    else if(original_msg[1]=='1')                //sign up
    {
        if(check_signup(original_msg+3))          //sign up success
        {
            *p_to_sockfd='\0';
            strcpy(p_msg_to_slef,"/10");
            for(i=0;i<80&&original_msg[i+12]!=';';i++)
                self_name[i]=original_msg[i+12];
            self_name[i]='\0';
            
            strcpy(temp,"offline_msg/");
            strcat(temp,self_name);
          // p_offline_msg=fopen("p_offline_mgs","w");
          // fprintf(p_offline_msg,"%s\n",p_msg_to_opp);
          // fclose(p_offline_msg);
            
            printf("%s sign up\n",self_name);
        }
        else
        {
            *p_to_sockfd=0;
            strcpy(p_msg_to_slef,"/1-1");
        }
    }
    else if(original_msg[1]=='2')                //get friend list
    {
        *p_to_sockfd=0;
        //char *friend_list[10];
        strcpy(temp,original_msg+3);
        len=strlen(temp);
        temp[len-1]=0;
printf("--------%s\n",temp);
        friend_num = get_friend_list_from_db(temp,temp1);
        
        printf("-----%d\n",friend_num);
        strcpy(p_msg_to_slef,"/2* ");
        
        
        itoa(friend_num,temp2);
        strcat(p_msg_to_slef,temp2);
        
        strcat(p_msg_to_slef," ");
        strcat(p_msg_to_slef,temp1);
        
        printf("%s",temp1);
    }
    else if(original_msg[1]=='3')                //add friend
    {
        /*if(original_msg[3]=='0')
        {
            strcpy(opp_name,original_msg+4);
            len = strlen(opp_name);
            opp_name[len-1]=0;
            
            if((opp_userID=if_user_online(opp_name))==-1)         //Friend not online
            {
                *p_msg_to_opp=0;
                
                strcpy(temp,"offline_msg/");
                strcat(temp,opp_name);
            //    p_offline_msg=fopen(temp,"a");
                
                strcpy(p_msg_to_opp,"/3:2,");
                strcat(p_msg_to_opp,online_user[current_userID]);
                strcat(p_msg_to_opp,"*");
             //   fprintf(p_offline_msg,"%s\n",p_msg_to_opp);
                
               // fclose(p_offline_msg);
            }
            else
            {
                *p_to_sockfd=ary_sockfd[opp_userID];
                
                strcpy(p_msg_to_opp,"/3:2,");
                strcat(p_msg_to_opp,online_user[current_userID]);
                
                strcat(p_msg_to_opp,"*");
            }
        }
        else if(original_msg[3]=='1')
        {
            strcpy(opp_name,original_msg+4);
            len = strlen(opp_name);
            opp_name[len-1]=0;
            
            insert_friend_into_database(online_user[current_userID],opp_name);
            
            if((opp_userID=if_user_online(opp_name))==-1)         //Friend not online
            {
                *p_msg_to_opp=0;
                
                strcpy(temp,"offline_msg/");
                strcat(temp,opp_name);
              //  p_offline_msg=fopen(temp,"a");
                
                strcpy(p_msg_to_opp,"/3:3,");
                strcat(p_msg_to_opp,online_user[current_userID]);
                strcat(p_msg_to_opp,"*");
              //  fprintf(p_offline_msg,"%s\n",p_msg_to_opp);
                
              //  fclose(p_offline_msg);
            }
            else
            {
                *p_to_sockfd=ary_sockfd[opp_userID];
                
                strcpy(p_msg_to_opp,"/3:3,");
                strcat(p_msg_to_opp,online_user[current_userID]);
                strcat(p_msg_to_opp,"*");
            }
        }
        else if(original_msg[3]=='2')
        {
            
        }
        else if(original_msg[3]=='3')
        {}*/
       // else
       // {
		
            strcpy(opp_name,original_msg+3);
            len=strlen(opp_name);
            opp_name[len-1]=0;
	printf("%s\n",opp_name);
            /*if((opp_userID=if_user_online(opp_name))==-1)         //Friend not online
            {
                *p_msg_to_opp=0;
                
                strcpy(temp,"offline_msg/");
                strcat(temp,opp_name);
         //       p_offline_msg=fopen(temp,"a");
                
                strcpy(p_msg_to_opp,"/3:");
                strcat(p_msg_to_opp,online_user[current_userID]);
                strcat(p_msg_to_opp,"*");
                
           //     fprintf(p_offline_msg,"%s\n",p_msg_to_opp);
             //   fclose(p_offline_msg);
            }*/
            //else
           // {
printf("jinmeijin\n");
                *p_to_sockfd=ary_sockfd[opp_userID];
                
                strcpy(p_msg_to_opp,"/3:");
               // strcat(p_msg_to_opp,online_user[current_userID]);
strcat(p_msg_to_opp,opp_name);
                strcat(p_msg_to_opp,"*");
          //  }
       // }
    }
    else if(original_msg[1]=='4')                //get offline msg
    {
        *p_to_sockfd=0;
        
        strcpy(temp,"offline_msg/");
        strcat(temp,online_user[current_userID]);
        p_offline_msg=fopen(temp,"r");
        
        p_msg_to_slef[0]=0;
        while(fgets(temp,99,p_offline_msg))
        {
            strcat(p_msg_to_slef,temp);
        }
        fclose(p_offline_msg);
        printf("%s",p_msg_to_slef);
        
        strcpy(temp,"offline_msg/");
        strcat(temp,online_user[current_userID]);
        p_offline_msg=fopen(temp,"w");
        fclose(p_offline_msg);
    }
    else if(original_msg[1]=='5')                //if friend exist
    {
        *p_to_sockfd=0;
        strcpy(temp,original_msg+3);
        len=strlen(temp);
        temp[len-1]=0;
        
        friend_num = search_user_from_db(temp,temp1);
        
        
        strcpy(p_msg_to_slef,"/5* ");
        
        itoa(friend_num,temp2);
        strcat(p_msg_to_slef,temp2);
        
        strcat(p_msg_to_slef," ");
        strcat(p_msg_to_slef,temp1);
    }
    else if(original_msg[1]=='6')                //get history msg from server
    {
        
    }
    else if(original_msg[1]=='7')                //set my avator in DB
    {
        *p_to_sockfd=0;
        avatorID=original_msg[3]-'0';
        change_avator_in_db(online_user[current_userID],avatorID);
        strcpy(p_msg_to_slef,"/7*");
        
    }
    else if(original_msg[1]=='8')                //search avator from DB
    {
        *p_to_sockfd=0;
        avatorID=search_avator_from_db(online_user[current_userID]);
        strcpy(p_msg_to_slef,"/8:0*");
        p_msg_to_slef[3]=avatorID+'0';
        
    }
    else if(original_msg[1]=='9')
    {
        *p_to_sockfd=0;
        //char *friend_list[10];
        strcpy(temp,original_msg+3);
        len=strlen(temp);
        temp[len-1]=0;
        group_num = get_group_list_from_db(temp,temp1);
        
        strcpy(p_msg_to_slef,"/9* ");
        
        itoa(group_num,temp2);
        strcat(p_msg_to_slef,temp2);
        
        strcat(p_msg_to_slef," ");
        strcat(p_msg_to_slef,temp1);
        
        printf("%s",temp1);
    }

	else if(original_msg[1]=='b')
    {
        *p_to_sockfd=0;
        //char *friend_list[10];
        int kk;
        for(kk=2;kk<strlen(original_msg)&&original_msg[kk]!=':';kk++)
        {
            temp[kk-2]=original_msg[kk];
        }
        kk++;
        *p_msg_to_slef=0;
        int gg=kk;
        for(;gg<strlen(original_msg)&&original_msg[gg]!='*';gg++)
        {
            temp1[gg-kk]=original_msg[gg];
        }
    
        
        create_group_into_database(temp,temp1);
    }
    else if(original_msg[1]=='q')
    {
        *p_to_sockfd=0;
        //char *friend_list[10];
        int kk;
        for(kk=2;kk<strlen(original_msg)&&original_msg[kk]!=':';kk++)
        {
            temp[kk-2]=original_msg[kk];
        }
        kk++;
        *p_msg_to_slef=0;
        int gg=kk;
        for(;gg<strlen(original_msg)&&original_msg[gg]!='*';gg++)
        {
            temp1[gg-kk]=original_msg[gg];
        }
        
        
        quit_group_into_database(temp,temp1);
    }
}
/********************************************************************************
 * Description : 用于处理收到的消息，只处理，不发送
 * Prameter    : 参考 process_command
 * Return      : 无意义
 * Side effect : Function is not completed.
 * Author      : zpy
 * Date        : 2018.9.2
 ********************************************************************************/
int process_msg(int* p_to_sockfd,int current_userID,char* p_msg_to_slef,char*p_msg_to_opp,const char* original_msg)
{
    char opp_name[100];
    char msg_to_opp[100];
    char temp[100];
    int i;
    int opp_userID;
    int len;
    FILE* p_offline_msg=NULL;
    
    printf("%s\n",original_msg);                           // delete later;
    if(original_msg[0]!='*')
    {
        for(i=0;i<100&&original_msg[i]!=':';i++)
            opp_name[i]=original_msg[i];
        opp_name[i]='\0';
       char sour_name[100];
i++;
int jj=i;
for(;jj<100&&original_msg[jj]!=':';jj++)
	{
	sour_name[jj-i]=original_msg[jj];
	}
	sour_name[jj-i]='\0';
	printf("!!!!!!!!!!!!%s\n",sour_name);
        if(if_user_exist(opp_name)==0)
        {
            strcpy(p_msg_to_slef,"User not exist\n");
            *p_msg_to_opp=0;
            printf("%s\n",p_msg_to_slef);
            printf("%s\n",original_msg);
            return 0;
        }
        else
        {
		printf("%s opp_name\n",opp_name);
		//opp_userID=if_user_online(opp_name);
		opp_userID =!current_userID;
		printf("&&&&&&&&&&&%d\n",opp_userID);
           //for(i=0;i<100&&original_msg[i]!=':';i++)
           //     opp_name[i]=original_msg[i];
           // opp_name[i]='\0';
            
           /* if((opp_userID=if_user_online(opp_name))==-1)         //Friend not online
            {
                strcpy(p_msg_to_slef,"User not online\n");
              //  *p_msg_to_opp=0;
                
                strcpy(temp,"offline_msg/");
                strcat(temp,opp_name);
    //            p_offline_msg=fopen(temp,"a");
                
                //strcpy(p_msg_to_opp,online_user[current_userID]);
                strcat(p_msg_to_opp,":");
                /*len=strlen(online_user[current_userID])+1;
                strcat(p_msg_to_opp,original_msg+len);*/
               // printf("%s\n",p_msg_to_opp);
      //          fprintf(p_offline_msg,"%s\n",p_msg_to_opp);
        //        fclose(p_offline_msg);
            //}*/
            //else
           // {
                *p_to_sockfd=ary_sockfd[opp_userID];
                printf("^^^^^^%d\n",opp_userID);
              //  strcpy(p_msg_to_opp,online_user[current_userID]);
		strcpy(p_msg_to_opp,sour_name);
                strcat(p_msg_to_opp,":");
                len=strlen(opp_name)+strlen(sour_name)+2;
                strcat(p_msg_to_opp,original_msg+len);
                
                strcpy(p_msg_to_slef,"get\n");
           // }
        }
        
    }
    else
    {
        int tlen;
        tlen=strlen(original_msg);
        for(i=402;i<tlen;i++)
            opp_name[i-402]=original_msg[i];
        opp_name[i-402]='\0';
        char tmsg[450];
        for(int tt=0;original_msg[tt]!=';';tt++)
            tmsg[tt]=original_msg[tt];
        if((opp_userID=if_user_online(opp_name))==-1)         //Friend not online
        {
            strcpy(p_msg_to_slef,"User not online\n");
            *p_msg_to_opp=0;
            
            strcpy(temp,"offline_msg/");
            strcat(temp,opp_name);
      //      p_offline_msg=fopen(temp,"a");
            
           // strcpy(p_msg_to_opp,online_user[current_userID]);
            //strcat(p_msg_to_opp,":");
            //len=strlen(online_user[current_userID])+1;
            strcat(p_msg_to_opp,tmsg);
            printf("%s\n",p_msg_to_opp);
        //    fprintf(p_offline_msg,"%s\n",p_msg_to_opp);
         //   fclose(p_offline_msg);
        }
        else
        {
            *p_to_sockfd=ary_sockfd[opp_userID];
            
            //strcpy(p_msg_to_opp,online_user[current_userID]);
            //strcat(p_msg_to_opp,":");
            //len=strlen(opp_name)+1;
            strcat(p_msg_to_opp,tmsg);
            
            strcpy(p_msg_to_slef,"get\n");
        }
    }
    
    return 1;
}






int process_gmsg(int* p_to_sockfd,int current_userID,char* p_msg_to_slef,char*p_msg_to_opp,const char*original_msg)
{
    char group_name[100];
    char msg_to_opp[100];
    char temp[100];
    int i;
    int opp_userID;
    int len;
    FILE* p_offline_msg=NULL;

    for(i=2;i<100&&original_msg[i]!=':';i++)
        group_name[i-2]=original_msg[i];

    group_name[i-2]=0;

    if(if_group_exist(group_name)==0)
    {
        strcpy(p_msg_to_slef,"User not exist\n");
        *p_msg_to_opp=0;
        return 0;
    }
    else
    {
        /*
         for(i=2;i<100&&original_msg[i]!=':';i++)
         group_name[i]=original_msg[i];
         group_name[i]=0;
         */
        char opp_names[100][100] = {0};


        int user_num = 0;

        user_num = get_group_users_from_db(group_name, opp_names);

        printf("%d", user_num);

        for(int i=0;i<user_num;i++)
        {
            printf("%s\n", opp_names[i]);
        }

        for(int i=0;i<user_num;i++)
        {
            //if(opp_names[i] == )
            //memset(opp_userID, 0, 100);
         /*   if((opp_userID=if_user_online(opp_names[i]))==-1)         //Friend not online
            {
                strcpy(p_msg_to_slef,"User not online\n");
                *p_msg_to_opp=0;

                strcpy(temp,"offline_msg/");
                strcat(temp,opp_names[i]);
       //         p_offline_msg=fopen(temp,"a");

                strcpy(p_msg_to_opp, "$");
                strcat(p_msg_to_opp, group_name);
                strcat(p_msg_to_opp,":");
                strcat(p_msg_to_opp,online_user[current_userID]);
                strcat(p_msg_to_opp,":");
                len=strlen(online_user[current_userID])+1;
                strcat(p_msg_to_opp,original_msg+len);
                //printf("%s\n",p_msg_to_opp);
        //        fprintf(p_offline_msg,"%s\n",p_msg_to_opp);//todo:加入标志来让client确定群消息
          //      fclose(p_offline_msg);
            }*/
           // else
           // {
                
	for(int i=0;i<=1;i++)
	{	 
		//opp_userID=if_user_online(opp_names[i]);
		//if(i == current_userID)	continue;		
		printf("%d oppuserID\n", opp_userID);
               // printf("%s\n", original_msg);
                *p_to_sockfd=ary_sockfd[i];

                strcpy(p_msg_to_opp, "$");
                strcat(p_msg_to_opp, group_name);
                strcat(p_msg_to_opp,":");
                strcat(p_msg_to_opp,online_user[current_userID]);
                strcat(p_msg_to_opp,":");
                len=strlen(group_name)+3;
                strcat(p_msg_to_opp,original_msg+len);

                strcpy(p_msg_to_slef,"get\n");

                if(*p_to_sockfd>0)
                {

                    printf("@@@@@@@@@@@@@%s\n", p_msg_to_opp);
                    send(*p_to_sockfd, p_msg_to_opp, strlen(p_msg_to_opp)+1, 0);
                }
		}
           // }

            usleep(1000000);
        }
    }

}
/********************************************************************************
 * Description : 处理文件消息，发布版本已改为 P2P
 * Prameter    : 参考 process_command
 * Return      : 无意义
 * Side effect : 参考 process_command
 * Author      : zpy
 * Date        : 2018.9.2
 ********************************************************************************/
int process_file(int* p_to_sockfd,int current_userID,char *p_msg_to_opp,const char*original_msg,int len,char*add)
{
    char str[100];
    char temp[100];
    char opp_name[100];
    char file_name[100];
    int i;
    int len1;
    int opp_userID;
    
    /*for(i=0;i<90&&original_msg[i+9];i++)
     opp_name[i]=original_msg[i+9];
     opp_name[i]=0;
     
     for(i=0;i<90&&original_msg[i+1];i++)
     file_name[i]=original_msg[i+1];
     file_name[7]=0;*/
    
    sscanf(original_msg+1,"%s %s",file_name,opp_name);
    
    opp_userID=if_user_online(opp_name);
    
    *p_to_sockfd=ary_sockfd[opp_userID];
    
    
    memset(p_msg_to_opp,' ',15);
    len1=strlen(add);
    
    /*p_msg_to_opp[0]='#';
     strcpy(p_msg_to_opp+1,file_name);
     len1=strlen(p_msg_to_opp);
     p_msg_to_opp[len1]=' ';
     
     strcpy(p_msg_to_opp+9,online_user[current_userID]);
     len1=strlen(p_msg_to_opp);
     p_msg_to_opp[len1]=' ';
     p_msg_to_opp[14]=0;*/
    p_msg_to_opp[0]='#';
    p_msg_to_opp[1]='#';
    strcpy(p_msg_to_opp+2,add);
    //int tmplen;
    //tmplen=strlen(p_msg_to_opp);
    //p_msg_to_opp[tmplen]=len1-'0';
    
    /*
     for(i=0;i<len;i++)
     memcpy(p_msg_to_opp+i+15,original_msg+i+15,1);
     */
    return 1;
}


/********************************************************************************
 * Description : 用于开启子线程 只收发 处理在子函数中进行
 * Prameter    : void* 赋值 NULL 即可
 * Return      : void* 当用户登出或掉线时函数返回，结束线程，返还资源
 * Side effect : 子线程在登出时必须关闭子线程
 * Author      : zpy
 * Date        : 2018.9.2
 ********************************************************************************/
void* _pthread_entrance(void* p)
{
	//return NULL;
    char buf[100];
    char temp[100];
    char msg_to_self[100];
    char msg_to_opp[100];
    char to_who[100];
    int recvbytes;
    int i;
    int current_user_ID;
    int current_user_sockfd=global_current_sockfd;
    int opposite_user_sockfd;
    
    for(i=0;i<MAX_USER_NUM;i++)
        if(current_user_sockfd==ary_sockfd[i])
            break;
    current_user_ID=i;

    char* try = (char*)p;
    
    while(1)
    {
        memset(buf,0,100);
        recvbytes=recv(current_user_sockfd, buf, 100, 0);    //reserve log request
        
        printf("%s\n",buf);
        if(recvbytes<=0)
        {
            userNum--;
            memset(online_user[current_user_ID],0,10);
            ary_sockfd[current_user_ID]=0;
            close(current_user_sockfd);
            return NULL;
        }
        else
        {
            printf("%s\n",buf);
            if(buf[0]=='/')
            {
                process_command(&opposite_user_sockfd,current_user_ID,msg_to_self,msg_to_opp,buf);
printf("falemei\n");
                send(current_user_sockfd,msg_to_self,strlen(msg_to_self)+1,0);
printf("%s\n",msg_to_self);
printf("falemei\n");
                if(opposite_user_sockfd>0)
                {
                    send(opposite_user_sockfd,msg_to_opp,strlen(msg_to_opp)+1,0);
		printf("%s\n",msg_to_opp);
		printf("HAHA\n");
                }
            }
            else if(buf[0]=='#')
            {
                process_file(&opposite_user_sockfd,current_user_ID,msg_to_opp,buf,recvbytes,try);
                if(opposite_user_sockfd>0)
                {
                    send(opposite_user_sockfd,msg_to_opp,recvbytes,0);
                }
            }
            else if(buf[0]=='$'&&buf[1]=='$')
            {
                process_gmsg(&opposite_user_sockfd, current_user_ID, msg_to_self, msg_to_opp,buf);
                
            }
            else
            {
               printf("cccccccccccc\n");
                process_msg(&opposite_user_sockfd,current_user_ID,msg_to_self,msg_to_opp,buf);
                //send(current_user_sockfd,msg_to_self,strlen(msg_to_self)+1,0);
                if(opposite_user_sockfd>0)
                {
                    send(opposite_user_sockfd,msg_to_opp,strlen(msg_to_opp)+1,0);
                }
                
            }
        }
        
    }
}
/********************************************************************************
 * Description : The entrance of whole soft
 * Prameter    : boid
 * Return      : 0
 * Side effect :
 * Author      : zpy
 * Date        : 2018.9.2
 ********************************************************************************/
int main()
{
    int i;
    
    char buf[100];
    int listen_sockfd;                           //server's listen socketfd
    int client_sockfd[10];                          //client's listen socketfd
int cnt=0;    
// int _2_server_sockfd;                           //chat socketfd
    socklen_t listen_len;
    socklen_t client_len;
    socklen_t server_len;
    struct sockaddr_in listen_sockaddr;
    struct sockaddr_in client_sockaddr;
    struct sockaddr_in server_sockaddr;
    //the number of current online user
    struct in_addr client_ip;                    //the client's address information
    pthread_t ptid[MAX_USER_NUM];
    
    char try[100];
    
    printf("\n======================server initialization======================\n");
    listen_sockfd = socket(AF_INET,SOCK_STREAM, 0);                        // 定义套接字类型
    listen_sockaddr.sin_family = AF_INET;
    listen_sockaddr.sin_port = htons(MYPORT);
    listen_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   
    listen_len = sizeof(listen_sockaddr);
   int on=1;
    setsockopt(listen_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if(bind(listen_sockfd,(struct sockaddr *)&listen_sockaddr,listen_len)==-1)
    {
        perror("bind");
        exit(1);
    }
    else
    {
        printf("A new socket bind success\n");
    }
    
    if(listen(listen_sockfd,5) == -1)
    {
        perror("listen");
        exit(1);
    }
    else printf("listen success!\n");
    
    while(1)
    {
        struct in_addr sin_addr;
        int ne_sockfd;
	struct sockaddr_in ne_sockaddr;
        printf("zzxx\n");
        fflush(stdout);
   //     int on = 1;                              //允许重复使用本地地址和套接字绑定
        
        
        
        
        client_len = sizeof(struct sockaddr);
        //listen_len = sizeof(listen_sockaddr);
        printf("shizhende\n");
        if((client_sockfd[cnt++]=accept(listen_sockfd,(struct sockaddr *)&client_sockaddr,&client_len))==-1)
        {
		printf("tuichu\n");
            perror("accept error");
            exit(1);
        }
        else                                     //connect success
        {  
		
            printf("That's it!\n");
            sin_addr=client_sockaddr.sin_addr;
            itoa(PORT+userNum,buf);
            send(client_sockfd[cnt-1],buf,strlen(buf),0);
            //userNum++;
        }
        /*for(i=0;i<MAX_USER_NUM;i++)
            if(ary_sockfd[i]==0)
                break;*/
        //ary_sockfd[i]=client_sockfd;
        //global_current_sockfd=client_sockfd;
        
        //pthread_create(&ptid[i],NULL,_pthread_entrance,NULL);
        
        strcpy(try,inet_ntoa(client_sockaddr.sin_addr));
        
        /*_2_server_sockfd = socket(AF_INET,SOCK_STREAM, 0); // 定义套接字类型
        server_sockaddr.sin_family = AF_INET;
        server_sockaddr.sin_port = htons(PORT+userNum);
        server_sockaddr.sin_addr=sin_addr;
        server_len = sizeof(server_sockaddr);
        bzero(&(server_sockaddr.sin_zero),sizeof(server_sockaddr.sin_zero));*/
        sleep(0.1);
        for(i=0;i<MAX_USER_NUM;i++)
            if(ary_sockfd[i]==0)
        	{
			ary_sockfd[i]=client_sockfd[cnt-1];
		printf("%d is sockfd\n %d",client_sockfd[cnt-1]);
			break;		
		}
        global_current_sockfd=client_sockfd[cnt-1];
        
        printf("Maybe\n");
        /*if (connect(_2_server_sockfd, (struct sockaddr *)&server_sockaddr,sizeof(struct sockaddr_in)) == -1)
        {
            printf("Maybe\n");
            perror("connect");
            exit(1);
        }
        else
        {
            userNum++;
        }*/
        userNum++;
		//("ttee\n");
        pthread_create(&ptid[userNum],NULL,_pthread_entrance,&try);
	/*switch(fork()){
		case 0:
			(*_pthread_entrance)(&try);
			break;
		case -1:
			printf("error");
			break;
	}*/
printf("ttee\n");
	fflush(stdout);
        //close(listen_sockfd);
    }
	close(listen_sockfd);
}
