#include<gtk/gtk.h>
#include<string.h>
#include<stdlib.h>
#include"sungtk_interface.h"


/*****************************************************
* 函数功能:设置背景图
* 参数类型:	widget: 主窗口
* 				w, h:   图片的大小
* 				path：  图片路径
* 返回值:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
******************************************************/
int sungtk_background_set_picture(GtkWidget *widget, const gchar *path, const int w, const int h)
{
	if(widget==NULL || path==NULL)
		return -1;
	gtk_widget_set_app_paintable(widget, TRUE); 	//允许窗口可以绘图
	gtk_widget_realize(widget); 
	
	/* 更改背景图时，图片会重叠
	 * 这时要手动调用下面的函数，让窗口绘图区域失效，产生窗口重绘制事件（即 expose 事件）。
	 */
	gtk_widget_queue_draw(widget);
	
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(path, NULL);	// 创建图片资源对象
	// w, h是指定图片的宽度和高度
	GdkPixbuf *dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR);

	GdkPixmap *pixmap = NULL;
	
	/* 创建pixmap图像; 
	 * NULL：不需要蒙版; 
	 * 123： 0~255，透明到不透明
	 */
	gdk_pixbuf_render_pixmap_and_mask(dst_pixbuf, &pixmap, NULL, 128);
	// 通过pixmap给widget设置一张背景图，最后一个参数必须为: FASLE
	gdk_window_set_back_pixmap(widget->window, pixmap, FALSE);
	
	// 释放资源
	g_object_unref(src_pixbuf);
	g_object_unref(dst_pixbuf);
	g_object_unref(pixmap);
	return 0;
}



/**************************************************
 * 函数功能:给创建好的image重新设计一张图片
 * 参数类型:
 *				image:已创建好的image控件
 * 				file_path:图片路径
 *				 w、h:图片长度和宽带，如w==0&&h==0,则使用图片默认大小
 * 返回类型:成功返回0,失败返回-1
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
int sungtk_image_load_picture(GtkWidget *image, const char *file_path, const int w, const int h )
{
	GdkPixbuf *src_pixbuf = NULL;
	GdkPixbuf *dest_pixbuf = NULL;
	if(image==NULL || file_path==NULL)
		return -1;
	
	gtk_image_clear( GTK_IMAGE(image) );        // 清除图像
	src_pixbuf = gdk_pixbuf_new_from_file(file_path, NULL);	// 创建图片资源
	if(w==0 && h==0){
		dest_pixbuf = src_pixbuf;
	}else{
		dest_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR); // 指定大小
	}
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), dest_pixbuf); // 图片控件重新设置一张图片(pixbuf)

	if(w!=0 || h!=0)
		g_object_unref(dest_pixbuf);	// 释放资源
		
	g_object_unref(src_pixbuf);		// 释放资源
	return 0;
}

/**************************************************
 * 函数功能:创建一个带图片的image控件
 * 参数类型:
 *				image:已创建好的image控件
 * 				file_path:图片路径
 *				 w、h:图片长度和宽带，如w==0&&h==0,则使用图片默认大小
 * 返回类型:成功返回0,失败返回-1
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
GtkWidget *sungtk_image_new_from_file(const char *file_path, const int w, const int h)
{
	GtkWidget *image = gtk_image_new_from_pixbuf(NULL);	
	sungtk_image_load_picture(image, file_path, w, h);
	return image;
}


/**************************************************
 * 函数功能:根据图片路径创建一个新按钮，同时指定图片大小
 * 参数类型:
 * 				file_path:图片路径
 * 				w、h:图片长度和宽带，如w==0&&h==0,则使用图片默认大小
 * 返回类型:成功返回创建button地址
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
GtkWidget *sungtk_button_new_with_image(const char *file_path, const int w, const int h)
{
	GtkWidget *temp_image = gtk_image_new_from_pixbuf(NULL);
	sungtk_image_load_picture(temp_image, file_path, w, h);
	
	GtkWidget *button = gtk_button_new(); 					// 先创建空按钮
	gtk_button_set_image(GTK_BUTTON(button), temp_image);	// 给按钮设置图标
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);	// 按钮背景色透明
	
	return button;
}

/**************************************************
 * 函数功能:重新设置按钮的照片
 * 参数类型:button:要设置的按钮控件
 * 				file_path:图片路径
 * 				w、h:图片长度和宽带，如w==0&&h==0,则使用图片默认大小
 * 返回类型:
  * 维护记录:2013-12-18 by lihuibo
 **************************************************/
int sungtk_button_set_image(GtkWidget *button, const gchar *file_path, const int w, const int h)
{
	if(button==NULL || file_path==NULL)
		return -1;
	GtkWidget *image = gtk_button_get_image(GTK_BUTTON(button)); // 获得按钮上面的图标
	sungtk_image_load_picture(image, file_path, w, h);
	return 0;
}


GtkWidget *sungtk_button_new_with_label_and_image(const char *file, const char *label, const int w, const int h)
{
	GtkWidget *temp_image = gtk_image_new_from_pixbuf(NULL);
	sungtk_image_load_picture(temp_image, file, w, h);
	
	GtkWidget *button = gtk_button_new_with_label(label); 					// 先创建空按钮
	gtk_button_set_image(GTK_BUTTON(button), temp_image);	// 给按钮设置图标
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);	// 按钮背景色透明
	gtk_button_set_image_position(GTK_BUTTON(button), GTK_POS_TOP);
	return button;
}



/**************************************************
 * 函数功能: 设置控件字体大小
 * 				widget: 需要改变字体的控件
 * 				size: 字体大小
 * 参数类型:
 *				is_button: TRUE代表控件为按钮，FALSE为其它控件
 * 返回类型:成功返回0，失败返回-1
  * 维护记录:2013-12-18 by lihuibo
 **************************************************/
int sungtk_widget_set_font_size(GtkWidget *widget, int size, gboolean is_button)
{
	GtkWidget *labelChild;  
	PangoFontDescription *font;  
	gint fontSize = size;  
	if(widget == NULL)
		return -1;
  
	font = pango_font_description_from_string("Sans");          //"Sans"字体名   
	pango_font_description_set_size(font, fontSize*PANGO_SCALE);//设置字体大小   
	
	if(is_button){
		labelChild = gtk_bin_get_child(GTK_BIN(widget));//取出GtkButton里的label  
	}else{
		labelChild = widget;
	}
	
	//设置label的字体，这样这个GtkButton上面显示的字体就变了
	gtk_widget_modify_font(GTK_WIDGET(labelChild), font);
	pango_font_description_free(font);

	return 0;
}

/***********************************************
* 函数功能:获取一个GdkColor类型
* 参数类型:color_buf:设置字体颜色，如"red"、"blue"
				color:存放GdkColor类型地址
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
int sungtk_color_get(const char *color_buf, GdkColor *color)
{
	gdk_color_parse(color_buf, color);
	return 0;
}

/***********************************************
* 函数功能:设置窗口显示字体颜色
* 参数类型:widget:需要改变颜色的控件
				color_buf:设置字体颜色，如"red"、"blue"
				is_button:是否为button
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
int sungtk_widget_set_font_color(GtkWidget *widget, const char *color_buf, gboolean is_button)
{
	if(widget == NULL && color_buf==NULL)
		return -1;
	
	GdkColor color;
	GtkWidget *labelChild = NULL;
	sungtk_color_get(color_buf, &color);
	if(is_button == TRUE){
		labelChild = gtk_bin_get_child(GTK_BIN(widget));//取出GtkButton里的label  
		gtk_widget_modify_fg(labelChild, GTK_STATE_NORMAL, &color);
		gtk_widget_modify_fg(labelChild, GTK_STATE_SELECTED, &color);
		gtk_widget_modify_fg(labelChild, GTK_STATE_PRELIGHT, &color);
	}else{
		gtk_widget_modify_fg(widget, GTK_STATE_NORMAL, &color);
	}
	return 0;
}

/***********************************************
* 函数功能:创建一个滚动窗口
* 参数类型:
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
GtkWidget *sungtk_scrolled_window_new(void)
{
	GtkObject *adjust_h = NULL;
	GtkObject *adjust_v = NULL;

	adjust_h = gtk_adjustment_new(0, 0, 1, 1, 1, 1);
	adjust_v = gtk_adjustment_new(0, 0, 1, 1, 1, 1);
	GtkWidget *s_window = gtk_scrolled_window_new(GTK_ADJUSTMENT(adjust_h), GTK_ADJUSTMENT(adjust_v));
	return s_window;
}

/***********************************************
* 函数功能:设置滚动窗口的水平值
* 参数类型:widget:滚动窗口地址
				add_val:原基础之上进行相加，正负值滚动相反方向
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
int sungtk_scrolled_window_set_hadjustment_value(GtkWidget *s_window, const gdouble add_val)
{
	GtkAdjustment *adjust_h = NULL;
	adjust_h = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(s_window));
	gdouble temp_val = gtk_adjustment_get_value(GTK_ADJUSTMENT(adjust_h));
	//printf("temp_val=====%lf\n", temp_val);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(adjust_h), temp_val+add_val);
	return 0;
}

/***********************************************
* 函数功能:设置滚动窗口的垂直方向值
* 参数类型:widget:滚动窗口地址
				add_val:原基础之上进行相加，正负值滚动相反方向
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
int sungtk_scrolled_window_set_vadjustment_value(GtkWidget *s_window, const gdouble add_val)
{
	GtkAdjustment *adjust_v = NULL;
	adjust_v = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(s_window));
	gdouble temp_val = gtk_adjustment_get_value(GTK_ADJUSTMENT(adjust_v));
	//printf("temp_val=====%lf\n", temp_val);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(adjust_v), temp_val+add_val);
	return 0;
}






/*********************自定义列表实现*************************************************/


static void sungtk_clist_set_label_color(GtkWidget *label, const char *color_buf)
{
	GdkColor color;
	gdk_color_parse(color_buf, &color);
	gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
}

static void sungtk_clist_set_label_size(GtkWidget *label, const gint size)
{
	PangoFontDescription *font;  
	font = pango_font_description_from_string("Sans");          //"Sans"字体名   
	pango_font_description_set_size(font, size*PANGO_SCALE);//设置字体大小   
	gtk_widget_modify_font(GTK_WIDGET(label), font);
	pango_font_description_free(font);
}

int sungtk_clist_set_foreground(SunGtkCList *clist, const gchar *color_buf)
{
	if(clist == NULL)
		return -1;
	int i = 0;
	CListLink *temp_head = clist->head;
	memset(clist->font_color, 0, sizeof(clist->font_color));
	strncpy(clist->font_color, color_buf, sizeof(clist->font_color));
	while(temp_head)
	{
		sungtk_clist_set_label_color(temp_head->label, color_buf);
		temp_head = temp_head->next;
		++i;
	}
	return i;
}

int sungtk_clist_set_row_color(SunGtkCList *clist, gint row, const gchar *color_buf)
{
	if(clist == NULL)
		return -1;
	int i = 0;
	CListLink *temp_head = clist->head;
	while(temp_head)
	{
		if(i==row){
			sungtk_clist_set_label_color(temp_head->label, color_buf);
			break;
		}
		temp_head = temp_head->next;
		++i;
	}
	return i;
}


int sungtk_clist_select_row(SunGtkCList *clist, gint row, const gchar *color_buf)
{
	if(clist == NULL)
		return -1;
	int i = 0;
	CListLink *temp_head = clist->head;
	while(temp_head)
	{
		sungtk_clist_set_label_color(temp_head->label, clist->font_color);
		if(i==row){
			clist->select_row = i;
			sungtk_clist_set_label_color(temp_head->label, color_buf);
		}
		temp_head = temp_head->next;
		++i;
	}
	return i;
}

int sungtk_clist_set_row_height(SunGtkCList *clist, gint height)
{
	if(clist == NULL)
		return -1;
	int i = 0;
	CListLink *temp_head = clist->head;
	clist->row_height = height;
	while(temp_head)
	{
		gtk_widget_set_size_request(temp_head->event_box, clist->col_width, clist->row_height);
		temp_head = temp_head->next;
		++i;
	}
	return i;
}

int sungtk_clist_set_col_width(SunGtkCList *clist, gint width)
{
	if(clist == NULL)
		return -1;
	int i = 0;
	CListLink *temp_head = clist->head;
	clist->col_width = width;
	while(temp_head)
	{
		gtk_widget_set_size_request(temp_head->event_box, clist->col_width, clist->row_height);
		temp_head = temp_head->next;
		++i;
	}
	return i;
}

int sungtk_clist_set_text_size(SunGtkCList *clist, int size)
{
	if(clist == NULL)
		return -1;
	int i = 0;
	CListLink *temp_head = clist->head;
	clist->font_size = size;
	while(temp_head)
	{
		sungtk_clist_set_label_size(temp_head->label, size);
		temp_head = temp_head->next;
		++i;
	}
	return i;
}

const gchar *sungtk_clist_get_row_data(SunGtkCList *clist, gint row)
{
	if(clist == NULL)
		return NULL;
	int i = 0;
	CListLink *temp_head = clist->head;
	while(temp_head)
	{
		if(row == i)
			return gtk_label_get_text(GTK_LABEL(temp_head->label));
		temp_head = temp_head->next;
		++i;
	}
	return NULL;
}

int sungtk_clist_set_row_data(SunGtkCList *clist, gint row, const gchar *text)
{
	if(clist == NULL || text==NULL)
		return -1;
	int i = 0;
	CListLink *temp_head = clist->head;
	while(temp_head)
	{
		if(row == i){
			gtk_label_set_text(GTK_LABEL(temp_head->label), text);
			break;
		}
		temp_head = temp_head->next;
		++i;
	}
	return -1;
}


int sungtk_clist_set_select_row_signal(SunGtkCList *clist, const gchar *detailed_signal, SunGtkSelectRow callback)
{
	if(clist == NULL)
		return -1;

	clist->callback = callback;
	strncpy(clist->sig_str, detailed_signal, sizeof(clist->sig_str));
	return 0;
}

void sungtk_clist_set_position_in_fixed(SunGtkCList *clist, gint x, gint y)
{
	gtk_fixed_move(GTK_FIXED(clist->fixed), clist->vbox, x, y);
}


int sungtk_clist_append(SunGtkCList *clist, const gchar *text)
{
	if(clist==NULL || text==NULL)
		return -1;
	CListLink *temp = (CListLink *)malloc(sizeof(CListLink));
	if(temp==NULL)
		return -1;

	temp->event_box = gtk_event_box_new();
	gtk_event_box_set_visible_window(GTK_EVENT_BOX(temp->event_box), FALSE);
	gtk_widget_set_size_request(temp->event_box, clist->col_width, clist->row_height);
	gtk_container_add(GTK_CONTAINER(clist->vbox), temp->event_box);
	
	
	temp->label = gtk_label_new(text);
	gtk_misc_set_alignment(GTK_MISC(temp->label), 0, 0.5);//居中
	sungtk_clist_set_label_size(temp->label, clist->font_size);
	sungtk_clist_set_label_color(temp->label, clist->font_color);
	gtk_container_add(GTK_CONTAINER(temp->event_box), temp->label);
	temp->next = NULL;

	if(clist->callback != NULL){
		g_signal_connect(temp->event_box, clist->sig_str, G_CALLBACK(clist->callback), (void *)clist->total_num);
	}
	clist->total_num++;
	
	if(clist->head == NULL){
		clist->head = temp;
		return 0;
	}
	CListLink *temp_head = clist->head;
	CListLink *pf = NULL;
	while(temp_head)
	{
		pf = temp_head;
		temp_head = temp_head->next;
	}
	pf->next = temp;

	
	return 0;
}


SunGtkCList *sungtk_clist_new()
{
	SunGtkCList *clist = (SunGtkCList *)malloc(sizeof(SunGtkCList));
	if(clist==NULL)
		return NULL;
	memset(clist, 0, sizeof(SunGtkCList));
	clist->fixed = gtk_fixed_new();
	clist->vbox = gtk_vbox_new(TRUE, 0);
	gtk_fixed_put(GTK_FIXED(clist->fixed), clist->vbox, 0, 0);

	clist->font_size = 8;
	strncpy(clist->font_color, "black", sizeof(clist->font_color));
	clist->row_height = 30;
	clist->col_width = 100;
	clist->select_row = 0;
	clist->total_num = 0;
	return clist;
}

int sungtk_clist_unref(SunGtkCList *clist)
{
	if(clist == NULL)
		return-1;
	int i = 0;
	CListLink *temp_head = clist->head;
	CListLink *pf = NULL;
	while(temp_head)
	{
		pf = temp_head;
		temp_head = temp_head->next;
		gtk_widget_destroy(pf->label);
		gtk_widget_destroy(pf->event_box);
		free(pf);
		++i;
	}
	gtk_widget_destroy(clist->fixed);
	free(clist);
	return 0;
}
/**********************************************************************/




/********************自定义滑动锁****************************/

static inline int get_press_image_num(SungtkMotionLock *lock, double press_x, double press_y)
{
	int i, j;
	int n = 0;
	int temp = lock->space_size + lock->img_size;
	//printf("press_x==%lf, press_y==%lf\n", press_x, press_y);
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if((press_x > lock->space_size+j*temp) && 
				(press_x < (j+1)*temp) &&
				(press_y > lock->space_size+i*temp) && 
				(press_y < (i+1)*temp)){
				return n;
			}
			n++;
		}
	}
	return -1;
}

static inline void sungtk_motion_lock_show_button(SungtkMotionLock *lock, int num)
{
	char *image[2] = {NULL, NULL};
	image[1] = lock->press_pic;
	image[0] = lock->unpress_pic;
	sungtk_image_load_picture(lock->image[num], image[lock->pressed[num]], lock->img_size, lock->img_size);
}


static inline void sungtk_motion_lock_show_board(SungtkMotionLock *lock)
{
	int i;
	char *image[2] = {NULL, NULL};
	image[1] = lock->press_pic;
	image[0] = lock->unpress_pic;
	for(i=0;i<MOTIONKEY_NUM;i++){
		sungtk_image_load_picture(lock->image[i], image[lock->pressed[i]], lock->img_size, lock->img_size);
	}
}

static inline void sungtk_remember_password(SungtkMotionLock *lock, int num)
{
	int i;
	for(i=0;i<MOTIONKEY_NUM;i++){
		if(lock->password[i] == 0)
			break;
	}
	lock->password[i] = num+1;
}


void sungtk_motion_lock_get_password(SungtkMotionLock *lock, char *pwd, int size)
{
	int i;
	if(pwd == NULL)
		return;

	memset(pwd, 0, size);
	for(i=0;i<MOTIONKEY_NUM && i<size;i++){
		if(lock->password[i] != 0){
			pwd[i] = lock->password[i] + '0';
		}
	}
}

static inline gboolean callback_lock_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	SungtkMotionLock *lock = (SungtkMotionLock *)data;
	int num = get_press_image_num(lock, event->x, event->y);
	//printf("num===%d, ===%d\n", num, FALSE);
	memset(lock->password, 0, sizeof(lock->password));
	memset(lock->pressed, 0, sizeof(lock->pressed));
	if(num != -1){
		lock->start_flag = TRUE;
		lock->pressed[num] = TRUE;
		sungtk_motion_lock_show_button(lock, num);
		sungtk_remember_password(lock, num);
	}
	return TRUE;
}


static inline gboolean callback_lock_motion_event(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	SungtkMotionLock *lock = (SungtkMotionLock *)data;
	if(lock->start_flag == FALSE)
		return FALSE;

	int num = get_press_image_num(lock, event->x, event->y);
	if(num != -1){
		if(lock->pressed[num] == FALSE){
			lock->pressed[num] = TRUE;
			sungtk_motion_lock_show_button(lock, num);
			sungtk_remember_password(lock, num);
		}
	}
	return TRUE;
}

static void sungtk_motion_lock_board_init(SungtkMotionLock *lock)
{
	int i,j;
	int n = 1;
	int temp = lock->space_size + lock->img_size;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			lock->image[n-1] = sungtk_image_new_from_file(lock->unpress_pic, lock->img_size, lock->img_size);
			gtk_fixed_put(GTK_FIXED(lock->fixed), lock->image[n-1], 
				lock->space_size+j*temp, lock->space_size+i*temp);
			n++;
		}
	}
} 

void sungtk_motion_lock_release_deal(SungtkMotionLock *lock)
{
	lock->start_flag = FALSE;
	memset(lock->pressed, 0, sizeof(lock->pressed));
	memset(lock->password, 0, sizeof(lock->password));
	sungtk_motion_lock_show_board(lock);
}

void sungtk_motion_lock_signal_release(SungtkMotionLock *lock, SunGtkMotionLockRelease callback_release)
{
	g_signal_connect(lock->event_box, "button-press-event", G_CALLBACK(callback_lock_press_event), lock);
	g_signal_connect(lock->event_box, "button-release-event", G_CALLBACK(callback_release), lock);
	g_signal_connect(lock->event_box, "motion-notify-event", G_CALLBACK(callback_lock_motion_event), lock);
} 

SungtkMotionLock *sungtk_motion_lock_new(gint lock_size, gint img_size, const gchar *press_pic, const gchar *unpress_pic)
{
	SungtkMotionLock *lock = (SungtkMotionLock *)malloc(sizeof(SungtkMotionLock));
	if(lock == NULL)
		return NULL;
	memset(lock, 0, sizeof(SungtkMotionLock));
	lock->lock_size = lock_size;
	lock->img_size = img_size;
	lock->space_size = (lock_size - 3*img_size)/4;
	
	lock->press_pic = (gchar *)malloc(strlen(press_pic)+1);
	memset(lock->press_pic, 0, strlen(press_pic)+1);
	strcpy(lock->press_pic, press_pic);

	lock->unpress_pic = (gchar *)malloc(strlen(unpress_pic)+1);
	memset(lock->unpress_pic, 0, strlen(unpress_pic)+1);
	strcpy(lock->unpress_pic, unpress_pic);

	lock->event_box = gtk_event_box_new();
	gtk_event_box_set_visible_window(GTK_EVENT_BOX(lock->event_box), FALSE);
	gtk_widget_set_size_request(lock->event_box, lock_size, lock_size);

	lock->fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(lock->event_box), lock->fixed);
	sungtk_motion_lock_board_init(lock);

	return lock;
}

void sungtk_motion_lock_unref(SungtkMotionLock *lock)
{
	int i;
	if(lock == NULL)
		return;
	for(i=0;i<MOTIONKEY_NUM;i++){
		gtk_widget_destroy(lock->image[i]);
	}
	gtk_widget_destroy(lock->fixed);
	gtk_widget_destroy(lock->event_box);
	free(lock->press_pic);
	free(lock->unpress_pic);
	free(lock);
}


