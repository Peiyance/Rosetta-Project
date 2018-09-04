# Makefile for...

main: connector.o gui
	g++ -o main *.o `pkg-config --cflags --libs gtk+-2.0`
gui: sungtk_interface.c  login_signup.c main.c callbacks.c
	gcc -c $^ `pkg-config --cflags --libs gtk+-2.0`
connector.o: connector.cpp
	g++ -c $^ `pkg-config --cflags --libs gtk+-2.0`

clean:
	rm *.o main