// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#include "messages.h"
#define PORT 8000 
#define BUF_SIZE 16384

static void send_cust(char* text, int socket) {
	char sendbuf[BUF_SIZE] = {0};
	strncpy(sendbuf, text, strlen(text));
	send(socket, sendbuf, BUF_SIZE, 0);
}

int main(int argc, char const *argv[]) 
{ 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char *buf= (char*) malloc(BUF_SIZE * sizeof(char));
	char buffer[BUF_SIZE] = {0}; 
	char sendbuf[BUF_SIZE] = {0};
	char* cmd;
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

	while (1) {
		//printf("Send something to the server: ");
		memset(buffer, 0, sizeof(buffer));
		read( sock , buffer, BUF_SIZE); 
		printf("%s",buffer );
		fflush(stdout);
		memset(buf, 0, sizeof(buf));
		fgets(buf, BUF_SIZE*sizeof(char), stdin);
		strtok(buf, "\n");
		send_cust(buf, sock);
		printf("\e[1;1H\e[2J");
		if (strncmp(buf, "exit", strlen("exit"))==0) {
			break;
		}
		memset(buffer, 0, sizeof(buffer));
		read( sock , buffer, BUF_SIZE);
		printf("%s",buffer );
		fflush(stdout);
	}
	return 0; 
} 
