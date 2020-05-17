// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#include "messages.h"
#define PORT 8000 

int main(int argc, char const *argv[]) 
{ 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char *buf= (char*) malloc(1024 * sizeof(char));
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	}
/*
	read( sock , buffer, 1024); 
	printf("%s",buffer);
	send(sock, "banner ricevuto\n", strlen("banner ricevuto\n"), 0);
*/
	while (1) {
		//printf("Send something to the server: ");
		memset(buffer, 0, sizeof(buffer));
		read( sock , buffer, 1024); 
		printf("%s",buffer );
		fflush(stdout);
		fgets(buf, 1024*sizeof(char), stdin);
		send(sock , buf , strlen(buf) , 0 );
		if (strncmp(buf, "5\n", strlen(buf))==0) {
			break;
		}
		memset(buffer, 0, sizeof(buffer));
		read( sock , buffer, 1024); 
		printf("%s",buffer );
		fflush(stdout);
	}
	return 0; 
} 
