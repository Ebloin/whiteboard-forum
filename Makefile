CC = gcc
CFLAGS = -g

project: 
	$(CC) $(CFLAGS) server.c -o server
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm server; rm client;