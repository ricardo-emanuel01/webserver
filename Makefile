CC = gcc
CFLAGS = -g

tcps: tcpserver.o common.o routes.o
	$(CC) tcpserver.o common.o routes.o -o tcps

tcpserver.o: tcpserver.c common.h routes.h
	$(CC) $(CFLAGS) -c tcpserver.c

common.o: common.c common.h
	$(CC) $(CFLAGS) -c common.c

routes.o: routes.c routes.h
	$(CC) $(CFLAGS) -c routes.c
