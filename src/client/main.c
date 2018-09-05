#include "GUI.h"
#include "connector.h"

int main(int argv, char *argc[])
{
    char remoteIP[] = "192.168.160.130";
    short remotePort = 8889;
    init_connector(remoteIP, remotePort);

    gtk_init(&argv, &argc);
    // load_login_window();
		Entity *e = (Entity*) malloc(sizeof(Entity));
		memcpy(e->nickname, "heiheihei\0", sizeof(e->nickname));
		e->avatar_id = 0;
	    Entity *a = (Entity*) malloc(sizeof(Entity)), *b = (Entity*) malloc(sizeof(Entity));
	    a[0].avatar_id = 0;
	    memcpy(a[0].nickname, "test", sizeof(a[0].nickname));
	    b[0].avatar_id = 0;
	    memcpy(b[0].nickname, "test", sizeof(b[0].nickname));
	    load_main_window(e->nickname,e->avatar_id,a,1,b,1);
	    load_group_info(b, 1);
	    load_friend_info(a, 1);
    gtk_main();
}
