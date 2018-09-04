# Makefile for...
main: main.o connector.o
	g++ -g -o main $^ `pkg-config --cflags --libs gtk+-2.0`
main.o: src/example_main.cpp
	g++ -g -c $< `pkg-config --cflags --libs gtk+-2.0`
connector.o: src/connector.cpp
	g++ -g -c $<

clean:
	rm *.o main