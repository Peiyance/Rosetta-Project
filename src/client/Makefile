# Makefile for...

main: connector.o gui
	g++ -g -o main *.o `pkg-config --cflags --libs gtk+-2.0`
gui: sungtk_interface.c  login_signup.c main.c callbacks.c list_ui.c settings.c chat_history_window.c friends_groups_settings.c
	gcc -g -c $^ `pkg-config --cflags --libs gtk+-2.0`
connector.o: connector.cpp
	g++ -g -c $^ `pkg-config --cflags --libs gtk+-2.0`

clean:
	rm *.o main
