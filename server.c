#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include "struct.h"
#include "messages.h"
#define PORT 8000

int main(int argc, char const *argv[]) { 
    
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0};
	char buf[1024] = {0};
	char *hello = "Hello from server"; 
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) { 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	}

	//INIZIALIZE SHARED MEMORY
	//creo la chiave
	key_t key = ftok("shmfile",65); 
	//Creo la shared memory (key, size, perm)
	int shmid = shmget(key,sizeof(whiteboard),0666|IPC_CREAT); 
	//Mi attacco alla shared memory
	whiteboard* w = (whiteboard*) shmat(shmid,(void*)0,0);
	//setto a 0 la memoria
	memset(w,0,sizeof(whiteboard));
	shmdt(w);

    //listening loop
	/*
	AD OGNI CLIENT CONNESSO ATTRAVERSO LA ACCEPT() VIENE FORKATO UN PROCESSO
	CHE SI OCCUPA DI GESTIRLO, IL PROCESSO È IN LOOP INFINITO E RICEVE 
	COMANDI DAL CLIENT, SE IL CLIENT INVIA IL COMANDO QUIT IL PROCESSO CHIUDE
	LA CONNESSIONE E MUORE.
	*/
    int newchild=0;
    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }
        else {
            //Creo processo
            newchild = fork();
            if (newchild == 0) {
				//New client da gestire	
				int PID = getpid(), command, alive=1;
				//Apro la shm
				whiteboard* w = (whiteboard*) shmat(shmid,(void*)0,0);
				int AUTH = 0;
				send(new_socket , BANNER , strlen(BANNER) , 0 );
				printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Banner sent to client\n");
				read( new_socket , buffer, 1024);
				printf("\033[1;33m[S <-- %d] CLIENT:\033[0m %s", PID,buffer );

				//TODO
				int cur_user_index = 0; //Insert login function
				//END TODO

				//test shared-memory
				/*
				whiteboard* w = (whiteboard*) shmat(shmid,(void*)0,0);
				printf("listo i topic dal figlio\n");
				list_topics(w->topics);
				add_topic(w->topics, "topicdafiglio");
				shmdt(w);
				*/

				//Client loop
				while(alive) {
					//Get command
					send(new_socket , COMMANDS_LIST , strlen(COMMANDS_LIST) , 0 );
					printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Command list sent\n");							
					memset(buffer, 0, sizeof(buffer));		
					read( new_socket , buffer, 1024);
					command = atoi(buffer);
					printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Command received from client -> %s", PID,buffer );
					fflush(stdout);
					switch(command) {
						case QUIT:
							send(new_socket , "Closing connection\n" , 1024, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Closing connection\n");
							fflush(stdout);
							alive=0;
							break;

						case LIST_TOPICS: 
							//printf("Devo listare i topic\n");
							memset(buf, 0, sizeof(buf));
							list_topics(w->topics, buf);
							send(new_socket , buf , 1024, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Lista dei topic inviata\n");
							fflush(stdout);
							break;

						case ADD_TOPIC:
							//Send ack and send new istructions
							send(new_socket , ACK , 1024, 0);
							//ADDT
							send(new_socket , ADDT_COMMANDS , 1024, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDT_COMMANDS);
							fflush(stdout);
							//leggo il nome del topic
							memset(buffer, 0, sizeof(buffer));		
							read( new_socket , buffer, 1024);
							printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Topicname to add received -> %s", PID,buffer );
							fflush(stdout);
							add_topic(w->topics, buffer);
							//list_topics(w->topics);
							send(new_socket , ADDT_SUCCESS , 1024, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDT_SUCCESS);
							fflush(stdout);
							break;

						case ADD_MESSAGE_TO_TOPIC:
							send(new_socket , ACK , 1024, 0);
							send(new_socket , ADDM_SELECT_TOPIC , 1024, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDM_SELECT_TOPIC);
							fflush(stdout);
							memset(buffer, 0, sizeof(buffer));		
							read( new_socket , buffer, 1024);
							int topicindex;
							topicindex = atoi(buffer);
							printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Topic selected to add message -> %s", PID,buffer );
							fflush(stdout);
							send(new_socket , ACK , 1024, 0);
							//Richiedi testo messaggio
							send(new_socket , ADDM_MESSAGE_TEXT , 1024, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDM_MESSAGE_TEXT);
							fflush(stdout);
							memset(buffer, 0, sizeof(buffer));		
							read( new_socket , buffer, 1024);
							printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Message to post sent from client -> %s", PID,buffer );
							fflush(stdout);
							add_message_to_topic(w,buffer, cur_user_index, topicindex);
							send(new_socket , ADDM_MESSAGE_POSTED , 1024, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDM_MESSAGE_POSTED);
							fflush(stdout);
							break;

						default:
							send(new_socket , UNKNOWN , 1024, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
							fflush(stdout);
							break;
					}
				}
				close(new_socket);
				printf("\033[1;31mConnection close with client %d\n\033[0m", PID);
				//stacclo la shm
				shmdt(w);
                return 0;
            }

            else {
                //sono il padre
                printf("\033[1;31mNew process handler started with PID: %d\n\033[0m", newchild);
            }
        }
    }

	return 0; 
} 
