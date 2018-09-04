# Makefile for...
main: main.o connector.o
	g++ -g -o main $^ `pkg-config --cflags --libs gtk+-2.0`
#example_main.o: src/client/example_main.cpp
#	g++ -g -c $< `pkg-config --cflags --libs gtk+-2.0`
connector.o: src/client/connector.cpp 
	g++ -g -c $< `pkg-config --cflags --libs gtk+-2.0`
main.o: src/client/GUI.h src/client/sungtk_interface.c src/client/sungtk_interface.h src/client/login_signup.c src/client/main.c src/client/callbacks.c
	g++ $^ -o main `pkg-config --cflags --libs gtk+-2.0`

clean:
	rm *.o main