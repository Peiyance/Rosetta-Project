#include "GUI.h"

int main(int argv,char *argc[])
{
    gtk_init(&argv,&argc);
    load_login_window();
    gtk_main();
}
