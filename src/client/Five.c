/**************************************************/
/*名称：Five.c
/*描述：五子棋小游戏客户端部分
/*作成日期： 2018-9-5
/*作者：程苗苗
/***************************************************/

#include "GUI.h"
#include <stdlib.h>
int SIZE = 20;
GtkWidget* g_btn[20][20];
int five_row[20][20];
int round = 0;
GtkWidget *five_window;
GtkWidget *dialog;      	//声明一个对话框
int gameOver = 0;		//标志游戏是否结束


//int game(char *board,char *username)
//{
//    //转义;
//  char escaped_username[1024];
//    escape_string(username, escaped_username);//
//构造数据包
//    Package *pkg = (Package *)new char[sizeof(Package) + 1024];
//    pkg->package_sequence = send_package_sequence++;
//   pkg->ver = 0x1;
//    strcat(board,";");
//    memcpy(pkg->payload, "*", strlen("*"));
//    memcpy(pkg->payload + strlen("*"), board, strlen(board));
//   memcpy(pkg->payload + strlen("*") + strlen("board"), escaped_username, strlen(escaped_username));
//    pkg->len = strlen("*") + strlen(board)+strlen(escaped_username);
//    //发送
//    //write(sockfd, pkg, sizeof(Package) + pkg->len);
//    return 0;
//
//}
/**************************************************/
/*名称：get_five_information
/*描述：加载从对方玩家收到五子棋信息
/*作成日期： 
/*参数：参数1：gtn、char类型二维数组、输入、需要传递的信息
/*返回值：void
/*作者：程苗苗
/***************************************************/
void get_five_information(char gbtn[][20])
{
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++)
		{
			char lab[10]="O";
			lab[0] = gbtn[i][j];
			gtk_button_set_label(GTK_BUTTON(g_btn[i][j]),lab); 
		}
		gtk_widget_show_all(five_window);
	}
}
/**************************************************/
/*名称：send_information
/*描述：建五子棋本地的信息传给对方玩家
/*作成日期： 2018-9-5
/*参数：参数1：gtn、char类型二维数组、输入、需要传递的信息
/*返回值：void
/*作者：程苗苗
/***************************************************/
void send_information(char gbtn[][20])
{
	char tmp[401];
	for(int i=0;i<20;i++)
	{
		for(int j=0;j<20;j++)
		{
			tmp[i*20+j]=gbtn[i][j];
		}
	}
}
/**************************************************/
/*名称：btnClick
/*描述：游戏点击button的回调函数，传递信息与判定是否胜利
/*作成日期：2018-9-5
/*参数：参数1：widget 、GtkWidget 指针类型、输入、button控件
		参数2：data、gpointer 类型、输入、这里是不必要的 
/*返回值：void
/*作者：程苗苗
/***************************************************/
void btnClick(GtkWidget* widget,gpointer data){
	if(gtk_button_get_label(GTK_BUTTON(widget))!=NULL)return;

	if(NULL == gtk_button_get_label(GTK_BUTTON(widget)) && gameOver == 0)
	{
		if(0 == round)
			gtk_button_set_label(GTK_BUTTON(widget),"0");
		else return;
		//异或变0/1;
		round = round ^ 1;    

		int row = 0;
		int col = 0;
		gboolean isFind = FALSE;
		for(row=0;row<SIZE;row++){
			for(col=0;col<SIZE;col++){
				if(data == g_btn[row][col]){
					isFind = TRUE;
					break;
				}
			}
			if(isFind) break;
		}

		//判断胜利
		int fg = isSucess(row,col);
		if( fg == 1)
		{
			gameOver = 1;
			msgbox("胜利!");
			return;
		}
		char tmp[20][20];
		for(int i=0;i<SIZE;i++)
		{
			for(int j=0;j<SIZE;j++)
			{
				tmp[i][j] = five_row[i][j]=='X'?'0':'X';
				if(five_row[i][j] == 0)
					tmp[i][j] =0;
			}
		}
        //send_information(tmp);

		gtk_widget_show_all(five_window);
	}
}
/**************************************************/
/*名称：isSucess
/*描述：判定某个位置周围的棋子时候胜利
/*作成日期：2018-9-5 
/*参数：参数1：row 、int 指针类型、输入、行位置
		参数2：col、int 类型、输入、列位置 
/*返回值：int 1 代表胜利 0代表不胜利
/*作者：程苗苗
/***************************************************/
int isSucess(int row,int col)
{
	GtkWidget* g = g_btn[row][col];
	int i;
	int k = 1;
	gchar* lable1;
	gchar* lable2 = (gchar*)gtk_button_get_label(GTK_BUTTON(g));
	
	for(i=1;i<=4;i++)
	{		//左比
		if(row - i >= 0)
		{
			lable1 = (gchar*)gtk_button_get_label(GTK_BUTTON(g_btn[row-i][col]));
			if(lable1 != NULL)
			{
				if( *lable1 == *lable2)
				{
					k++;
					if(k == 5) return 1;
				}
			}else break;
		}else break;

	}
	for(i=1;i<=4;i++)
	{			//右比
		if(row + i < SIZE)
		{
			lable1 = gtk_button_get_label(GTK_BUTTON(g_btn[row+i][col]));
			if(lable1 != NULL)
			{
				if( *lable1 == *lable2)
				{
					k++;
					if(k == 5) return 1;
				}
			} else break;
		}else break;
	}

	k = 1;
	//纵向比较
	for(i=1;i<=4;i++)
	{		//上比
		if(col - i >= 0)
		{
			lable1 = (gchar*)gtk_button_get_label(GTK_BUTTON(g_btn[row][col - i]));
			if(lable1 != NULL)
			{
				if( *lable1 == *lable2)
				{
					k++;
					if(k == 5) return 1;
				}
			}else break;
		}else break;

	}
	for(i=1;i<=4;i++)
	{			//下比
		if(col + i < SIZE)
		{
			lable1 = gtk_button_get_label(GTK_BUTTON(g_btn[row][col + i]));
			if(lable1 != NULL)
			{
				if( *lable1 == *lable2)
				{
					k++;
					if(k == 5) return 1;
				}
			} else break;
		}else break;
	}

	k = 1;
	for(i=1;i<=4;i++)
	{		
		if(col - i >= 0 && row - i >= 0)
		{
			lable1 = (gchar*)gtk_button_get_label(GTK_BUTTON(g_btn[row-i][col-i]));
			if(lable1 != NULL)
			{
				if( *lable1 == *lable2)
				{
					k++;
					if(k == 5) return 1;
				}
			}else break;
		}else break;

	}
	for(i=1;i<=4;i++)
	{			//下比
		if(col + i < SIZE && row + i < SIZE)
		{
			lable1 = gtk_button_get_label(GTK_BUTTON(g_btn[row+i][col+i]));
			if(lable1 != NULL)
			{
				if( *lable1 == *lable2)
				{
					k++;
					if(k == 5) return 1;
				}
			} else break;
		}else break;
	}

	return 0;
}
/**************************************************/
/*名称：load_five_window
/*描述：加载五子棋游戏界面
/*作成日期： 2018-9-5
/*参数：无
/*返回值：void
/*作者：程苗苗
/***************************************************/
void load_five_window()
{
	GtkWidget* table;
	memset(five_row,0,sizeof five_row);
	gameOver = 0;
    printf("nosd");
	five_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(five_window),400,400);
	gdk_window_set_title(GTK_WINDOW(five_window),"five_in_a_row");
	table = gtk_table_new(SIZE,SIZE,TRUE);
	int row,col;
	for(row=0;row<SIZE;row++){
		for(col=0;col<SIZE;col++){
			g_btn[row][col] = gtk_button_new_with_label(NULL);
			gtk_table_attach_defaults(GTK_TABLE(table),g_btn[row][col],row,row+1,col,col+1);
			g_signal_connect(g_btn[row][col],"clicked",G_CALLBACK(btnClick),g_btn[row][col]);
		}
	}
	gtk_container_add(GTK_CONTAINER(five_window),table);

	gtk_widget_show_all(five_window);

}
