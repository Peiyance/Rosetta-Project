#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>
static int SIZE = 20;
static GtkWidget* g_btn[20][20];
static int round = 0;
static GtkWidget *five_window;
static GtkWidget *dialog;      //声明一个对话框
static int gameOver = 0;		//标志游戏是否结束

static void btnClick(GtkWidget* widget,gpointer data){
	if(NULL == gtk_button_get_label(GTK_BUTTON(widget)) && gameOver == 0){
		if(0 == round){
			gtk_button_set_label(GTK_BUTTON(widget),"0");
		}else{
			gtk_button_set_label(GTK_BUTTON(widget),"X");
		}
		round = round ^ 1;    //异或变0/1；

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
		if( fg == 1){
			gameOver = 1;

			//使用gtk_message_dialog_new()函数新建一个消息对话框
			dialog = gtk_message_dialog_new(GTK_WINDOW(five_window),                  //没有父窗口
					GTK_DIALOG_DESTROY_WITH_PARENT,               //跟随父窗口关闭
					GTK_MESSAGE_WARNING,                          //显示警告图标
					GTK_BUTTONS_OK,                               //显示OK按钮
					"胜利");        //提示信息的内容

			gtk_window_set_title(GTK_WINDOW(dialog),"游戏结果"); //对话框的标题
			gtk_dialog_run(GTK_DIALOG(dialog));                     //运行对话框
			gtk_widget_destroy(dialog);                             //删除对话框

			gtk_main();//主函数循环，等待消息
		}
	}
}

//判断胜利
int isSucess(int row,int col){
	GtkWidget* g = g_btn[row][col];
	int i;
	int k = 1;
	gchar* lable1;
	gchar* lable2 = (gchar*)gtk_button_get_label(GTK_BUTTON(g));
	//横向比较
	for(i=1;i<=4;i++){		//左比
		if(row - i >= 0){
			lable1 = (gchar*)gtk_button_get_label(GTK_BUTTON(g_btn[row-i][col]));
			if(lable1 != NULL){
				if( *lable1 == *lable2){
					k++;
					if(k == 5) return 1;
				}
			}else break;
		}else break;

	}
	for(i=1;i<=4;i++){			//右比
		if(row + i < SIZE){
			lable1 = gtk_button_get_label(GTK_BUTTON(g_btn[row+i][col]));
			if(lable1 != NULL){
				if( *lable1 == *lable2){
					k++;
					if(k == 5) return 1;
				}
			} else break;
		}else break;
	}

	k = 1;
	//纵向比较
	for(i=1;i<=4;i++){		//上比
		if(col - i >= 0){
			lable1 = (gchar*)gtk_button_get_label(GTK_BUTTON(g_btn[row][col - i]));
			if(lable1 != NULL){
				if( *lable1 == *lable2){
					k++;
					if(k == 5) return 1;
				}
			}else break;
		}else break;

	}
	for(i=1;i<=4;i++){			//下比
		if(col + i < SIZE){
			lable1 = gtk_button_get_label(GTK_BUTTON(g_btn[row][col + i]));
			if(lable1 != NULL){
				if( *lable1 == *lable2){
					k++;
					if(k == 5) return 1;
				}
			} else break;
		}else break;
	}

	k = 1;
	//斜向比较
	for(i=1;i<=4;i++){		//左上比
			if(col - i >= 0 && row - i >= 0){
				lable1 = (gchar*)gtk_button_get_label(GTK_BUTTON(g_btn[row-i][col-i]));
				if(lable1 != NULL){
					if( *lable1 == *lable2){
						k++;
						if(k == 5) return 1;
					}
				}else break;
			}else break;

		}
		for(i=1;i<=4;i++){			//下比
			if(col + i < SIZE && row + i < SIZE){
				lable1 = gtk_button_get_label(GTK_BUTTON(g_btn[row+i][col+i]));
				if(lable1 != NULL){
					if( *lable1 == *lable2){
						k++;
						if(k == 5) return 1;
					}
				} else break;
			}else break;
		}

	return 0;
}
int main( int   argc,
		char *argv[] )
{
	GtkWidget *five_window;
	GtkWidget* table;

	//初始化
	gtk_init (&argc, &argv);
	//创建窗口
	five_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	//设置窗口大小
	gtk_window_set_default_size(GTK_WINDOW(five_window),400,400);
	//	gtk_window_set_resizable(GTK_WINDOW(five_window),FALSE);
	//设置窗口标题
	//gdk_window_set_title(GTK_WINDOW(five_window),"hello word");
	//窗口关闭，程序退出
	g_signal_connect (five_window,"destroy",gtk_main_quit,NULL);
	//创建表格布局
	table = gtk_table_new(SIZE,SIZE,TRUE);

	//添加按钮
	int row,col;
	for(row=0;row<SIZE;row++){
		for(col=0;col<SIZE;col++){
			g_btn[row][col] = gtk_button_new_with_label(NULL);
			gtk_table_attach_defaults(GTK_TABLE(table),g_btn[row][col],row,row+1,col,col+1);

			//			gpointer g = gtk_widget_get_pointer((GTK_WIDGET(g_btn[row][col]),row,col));
			g_signal_connect(g_btn[row][col],"clicked",G_CALLBACK(btnClick),g_btn[row][col]);
		}
	}
//	gtk_button_set_label(GTK_BUTTON(g_btn[1][0]),"0");
//	gtk_button_set_label(GTK_BUTTON(g_btn[2][0]),"0");

	//组装窗口并现实
	gtk_container_add(GTK_CONTAINER(five_window),table);

	gtk_widget_show_all(five_window);

	gtk_main ();

	return 0;
}
