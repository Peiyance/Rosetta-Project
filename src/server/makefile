server: serverfunc.o server.o
	gcc -o server server.o serverfunc.o -g -pthread --std=c99 -lmysqlclient
server.o:server.c
	gcc -c server.c -g -pthread --std=c99 -lmysqlclient
serverfunc.o:serverfunc.c
	gcc -c serverfunc.c -g -pthread --std=c99 -lmysqlclient
