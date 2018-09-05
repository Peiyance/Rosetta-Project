#include "GUI.h"
#include "connector.h"

int main(int argv, char *argc[])
{
    char remoteIP[] = "10.211.55.2";
    short remotePort = 8888;
    init_connector(remoteIP, remotePort);

    gtk_init(&argv, &argc);
    load_login_window();
    gtk_main();
}
