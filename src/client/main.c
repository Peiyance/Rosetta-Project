#include "GUI.h"
#include "connector.h"

int main(int argv, char *argc[])
{
    char remoteIP[] = "192.168.0.100";
    short remotePort = 8000;
    init_connector(remoteIP, remotePort);

    gtk_init(&argv, &argc);
    load_login_window();
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
	 //    load_main_window();
	 //    load_group_info();
	 //    load_friend_info();
    gtk_main();
}
