#ifndef _SUNGTK_INTERFACE_H_
#define _SUNGTK_INTERFACE_H_

#include<gtk/gtk.h>


#define GET_WIDGET_WIDTH(widget)	widget->allocation.width	//widget宽度
#define GET_WIDGET_HEIGHT(widget)	widget->allocation.height	//widget高度

/*****************************************************
* 函数功能:设置背景图
* 参数类型:	widget: 主窗口
* 				w, h:   图片的大小
* 				path：  图片路径
* 返回值:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
******************************************************/
extern int sungtk_background_set_picture(GtkWidget *widget, const gchar *path, const int w, const int h);


/**************************************************
 * 函数功能:给创建好的image重新设计一张图片
 * 参数类型:
 *				image:已创建好的image控件
 * 				file_path:图片路径
 *				 w、h:图片长度和宽带，如w==0&&h==0,则使用图片默认大小
 * 返回类型:成功返回0,失败返回-1
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
extern int sungtk_image_load_picture(GtkWidget *image, const char *file_path, const int w, const int h );

/**************************************************
 * 函数功能:创建一个带图片的image控件
 * 参数类型:
 *				image:已创建好的image控件
 * 				file_path:图片路径
 *				 w、h:图片长度和宽带，如w==0&&h==0,则使用图片默认大小
 * 返回类型:成功返回0,失败返回-1
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
GtkWidget *sungtk_image_new_from_file(const char *file_path, const int w, const int h);


/**************************************************
 * 函数功能:根据图片路径创建一个新按钮，同时指定图片大小
 * 参数类型:
 * 				file_path:图片路径
 * 				w、h:图片长度和宽带，如w==0&&h==0,则使用图片默认大小
 * 返回类型:成功返回创建button地址
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
extern GtkWidget *sungtk_button_new_with_image(const char *file_path, const int w, const int h);

/**************************************************
 * 函数功能:重新设置按钮的照片
 * 参数类型:button:要设置的按钮控件
 * 				file_path:图片路径
 * 				w、h:图片长度和宽带，如w==0&&h==0,则使用图片默认大小
 * 返回类型:
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
extern int sungtk_button_set_image(GtkWidget *button, const gchar *file_path, const int w, const int h);


extern GtkWidget *sungtk_button_new_with_label_and_image(const char *file, const char *label, const int w, const int h);


/**************************************************
 * 函数功能: 设置控件字体大小
 * 参数类型:
 * 				widget: 需要改变字体的控件
 * 				size: 字体大小
 *				is_button: TRUE代表控件为按钮，FALSE为其它控件
 * 返回类型:成功返回0，失败返回-1
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
extern int sungtk_widget_set_font_size(GtkWidget *widget, int size, gboolean is_button);

/***********************************************
* 函数功能:获取一个GdkColor类型
* 参数类型:color_buf:设置字体颜色，如"red"、"blue"
				color:存放GdkColor类型地址
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_color_get(const char *color_buf, GdkColor *color);

/***********************************************
* 函数功能:设置控件字体颜色
* 参数类型:widget:需要改变颜色的控件
				color_buf:设置字体颜色，如"red"、"blue"
				is_button:是否为button
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_widget_set_font_color(GtkWidget *widget, const char *color_buf, gboolean is_button);

/***********************************************
* 函数功能:创建一个滚动窗口
* 参数类型:
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
extern GtkWidget *sungtk_scrolled_window_new(void);

/***********************************************
* 函数功能:设置滚动窗口的水平值
* 参数类型:widget:滚动窗口地址
				add_val:原基础之上进行相加，正负值滚动相反方向
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_scrolled_window_set_hadjustment_value(GtkWidget *s_window, const gdouble add_val);

/***********************************************
* 函数功能:设置滚动窗口的垂直方向值
* 参数类型:widget:滚动窗口地址
				add_val:原基础之上进行相加，正负值滚动相反方向
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_scrolled_window_set_vadjustment_value(GtkWidget *s_window, const gdouble add_val);



/************************自定义列表************************************/
//每一行点击时的回调函数类型
typedef gboolean (*SunGtkSelectRow)(GtkWidget *widget, GdkEventButton *event, gpointer data);

//为列表创建的链表的结构体
typedef struct _clist_link
{
	GtkWidget *event_box;	//每一行都有一个事件盒子，用于支持点击
	GtkWidget *label;		//将label放置在事件盒子中，用于显示文字
	struct _clist_link *next;
}CListLink;

//整个列表的结构体
typedef struct _sungtk_clist
{
	GtkWidget *fixed;		//整个列表是一个固定布局
	GtkWidget *vbox;		//利用垂直布局，可以动态追加行
	gint select_row;		//当前选中行
	gint font_size;			//字体大小
	gchar font_color[30];	//字体颜色
	gint row_height;		//每行的高度
	gint col_width;			//每行宽度
	gint total_num;			//行总数
	gchar sig_str[30];		//信号
	SunGtkSelectRow callback;//行点击回调函数
	CListLink *head;
}SunGtkCList;

#define SUNGTK_CLIST(x) (SunGtkCList *)x	

//自定义列表创建
extern SunGtkCList *sungtk_clist_new();

//列表释放资源
extern int sungtk_clist_unref(SunGtkCList *clist);

//追加一行
extern int sungtk_clist_append(SunGtkCList *clist, const gchar *text);

//设置前景颜色
extern int sungtk_clist_set_foreground(SunGtkCList *clist, const gchar *color_buf);

//设置其中一行的颜色
extern int sungtk_clist_set_row_color(SunGtkCList *clist, gint row, const gchar *color_buf);

//选中某一行
extern int sungtk_clist_select_row(SunGtkCList *clist, gint row, const gchar *color_buf);

//设置行高度
extern int sungtk_clist_set_row_height(SunGtkCList *clist, gint height);

//设置列宽度
extern int sungtk_clist_set_col_width(SunGtkCList *clist, gint width);

//设置字体大小
extern int sungtk_clist_set_text_size(SunGtkCList *clist, int size);

//获取某一行内容
extern const gchar *sungtk_clist_get_row_data(SunGtkCList *clist, gint row);

//设置某一行内容
extern int sungtk_clist_set_row_data(SunGtkCList *clist, gint row, const gchar *text);

//设置点击行的信号
extern int sungtk_clist_set_select_row_signal(SunGtkCList *clist, const gchar *detailed_signal, SunGtkSelectRow callback);

//设置整个列表行在固定布局的位置
extern void sungtk_clist_set_position_in_fixed(SunGtkCList *clist, gint x, gint y);

/**************************************************************************/




/*********************自定义滑动锁***************************************/

#define MOTIONKEY_NUM 9	//锁的个数

//滑动锁抬起时回调函数的的类型
typedef gboolean (*SunGtkMotionLockRelease)(GtkWidget *widget, GdkEventButton *event, gpointer data);

//整个滑动锁的结构体
typedef struct _motion_lock
{
	GtkWidget *event_box;	//滑动事件盒子
	GtkWidget *fixed;		//事件盒子添加固定布局
	GtkWidget *image[MOTIONKEY_NUM];	//图片控件
	gint password[MOTIONKEY_NUM];		//密码记录
	gint pressed[MOTIONKEY_NUM];		//每个图片是否以按下
	gint start_flag;		//第一次按下标志位
	
	gint lock_size;			//整个滑动锁的大小
	gint img_size;			//每张图片大小
	gint space_size;		//图片间的间距
	gchar *press_pic;		//按下照片路径
	gchar *unpress_pic;		//未按下照片路径
}SungtkMotionLock;

//滑动锁抬起时的处理
extern void sungtk_motion_lock_release_deal(SungtkMotionLock *lock);

//设置滑动锁抬起时执行的回调函数， 实际就是事件盒子抬起事件
extern void sungtk_motion_lock_signal_release(SungtkMotionLock *lock, SunGtkMotionLockRelease callback_release);

//获取滑动锁按下的值
extern void sungtk_motion_lock_get_password(SungtkMotionLock *lock, char *pwd, int size);

//滑动锁的创建，需要指定大小，和图片路径名
extern SungtkMotionLock *sungtk_motion_lock_new(gint lock_size, gint img_size, const gchar *press_pic, const gchar *unpress_pic);

//滑动锁资源释放
extern void sungtk_motion_lock_unref(SungtkMotionLock *lock);
/**************************************************************************/


#endif


