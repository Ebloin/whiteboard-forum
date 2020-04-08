  all: server.c,client.c
 	  gcc -g -Wall -o server server.c
 	  gcc -g -Wall -o client client.c

  clean: 
	  $(RM) server
	  $(RM) client
