CC = gcc
CFLAGS = -g

project: 
	$(CC) $(CFLAGS) server.c struct.h struct.c -o server
	$(CC) $(CFLAGS) test-server.c struct.h struct.c -o test-server
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm server; rm client; rm test-server;
