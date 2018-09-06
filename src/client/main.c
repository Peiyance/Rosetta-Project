#include "GUI.h"
#include "connector.h"

int main(int argv, char *argc[])
{
    char remoteIP[] = "192.168.0.105";
    short remotePort = 10005;
    init_connector(remoteIP, remotePort);
    reg_cb_recv_unicast(on_recv_unicast_msg);
    reg_cb_recv_multicast(on_recv_multicast_msg);


    gtk_init(&argv, &argc);

    // *******  正常状态 *******
    load_login_window();
    
    // ******* 跳过登陆直接进入主面板，硬编码一个好友和一个群组 ******

	// Entity *e = (Entity*) malloc(sizeof(Entity));
	// memcpy(e->nickname, "heiheihei\0", sizeof(e->nickname));
	// e->avatar_id = 0;
	// myself = e;
 //    friendlist = (Entity*) malloc(sizeof(Entity));
 //    grouplist = (Entity*) malloc(sizeof(Entity));
 //    friendlist[0].avatar_id = 0;
 //    memcpy(friendlist[0].nickname, "test", sizeof(friendlist[0].nickname));
 //    grouplist[0].avatar_id = 0;
 //    memcpy(grouplist[0].nickname, "test", sizeof(grouplist[0].nickname));
 //    friend_cnt = 1;
 //    group_cnt = 1;
 //    f_msgbufs = (GtkTextBuffer**) malloc(sizeof(void*));
 //    f_msgbufs[0] = gtk_text_buffer_new(NULL);
 //    g_msgbufs = (GtkTextBuffer**) malloc(sizeof(void*));
 //    g_msgbufs[0] = gtk_text_buffer_new(NULL);
    
 //    load_main_window();
 //    load_group_info();
 //    load_friend_info();
    
    gtk_main();
}
