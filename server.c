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
	char sendbuf[1024] = {0};
	
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
	//printf("%d\n", sizeof(whiteboard));
	//printf("%d\n", sizeof(*w));
	memset(w,0,sizeof(whiteboard));
	//printf("test\n");
	//Creo l'utente admin
	create_user(w->users, "admin\n", "admin\n");
	shmdt(w);
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
				whiteboard* w = (whiteboard*) shmat(shmid,(void*)0,0);
				int AUTH = -1; char* cmd;

				//TODO
				while (AUTH == -1) {
					char username[1024] = {0};
					char password[1024] = {0};
					send_cust(LOGIN_REQUSR, new_socket);
					printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, "Richiesta username");
					fflush(stdout);
					memset(username, 0, sizeof(username));		
					read( new_socket , username, 1024);
					printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Username for login received -> %s", PID,username);
					fflush(stdout);
					send_cust(ACK, new_socket);
					send_cust(LOGIN_REQPASS, new_socket);
					printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, "Richiesta password");
					memset(password, 0, sizeof(password));		
					read( new_socket , password, 1024);
					printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Password for login received -> %s", PID,password);
					fflush(stdout);
					AUTH = login(w->users, username, password);
					if (AUTH == -1) {
						send_cust(LOGIN_FAILED, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Login non effettuato\n");
						fflush(stdout);
					}
				}

				send_cust(LOGIN_SUCC, new_socket);
				printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Login effettuato\n");
				fflush(stdout);
				
				send_cust(BANNER, new_socket);
				printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Banner sent to client\n");
				memset(buffer, 0, sizeof(buffer));		
				read( new_socket , buffer, 1024);
				send_cust(ACK, new_socket);
				
				while(alive) {
					//admin
					if (AUTH == 0) {
						send_cust(COMMANDS_LIST_ADM, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Command list sent\n");
					}
					//no admin
					else {
						send_cust(COMMANDS_LIST, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Command list sent\n");
					}
												
					memset(buffer, 0, sizeof(buffer));		
					read( new_socket , buffer, 1024);
					//Prima parola inserita che equivale al comando					
					cmd= strtok(buffer, " ");
					printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Command received from client -> %s\n", PID,buffer );
					fflush(stdout);

					//EXIT
					if (strncmp(cmd, "exit", strlen("exit"))==0) {
						send_cust(EXIT, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Closing connection\n");
						fflush(stdout);
						alive=0;
						break;
					}
					
					//CREATE TOPIC
					else if (strncmp(cmd, "create", strlen("create"))==0) {
						int res;
						//printf("comando %s\n", cmd);
						cmd = strtok(NULL, " ");
						//printf("su %s\n", cmd);
						if (strncmp(cmd, "topic", strlen("topic")) == 0) {
							send_cust(ACK, new_socket);
							send_cust(ADDT_COMMANDS, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDT_COMMANDS);
							fflush(stdout);
							//leggo il nome del topic
							memset(buffer, 0, sizeof(buffer));		
							read( new_socket , buffer, 1024);
							printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Topicname to add received -> %s", PID,buffer );
							fflush(stdout);
							res = add_topic(w->topics, buffer, AUTH);
							//error
							if (res == -1) {
								send_cust(ADDT_ERROR, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDT_ERROR);
								fflush(stdout);
							}	
							//success						
							else {
								send_cust(ADDT_SUCCESS, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDT_SUCCESS);
								fflush(stdout);
							}
						}
						else {
							//comando errato
							send_cust(UNKNOWN, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
							fflush(stdout);
						}
					}

					//DELETE TOPIC
					else if (strncmp(cmd, "delete", strlen("delete"))==0) {
						int res;
						//printf("comando %s\n", cmd);
						cmd = strtok(NULL, " ");
						//printf("su %s\n", cmd);
						if (strncmp(cmd, "topic", strlen("topic")) == 0) {
							send_cust(ACK, new_socket);
							send_cust(DELTOP_CODE, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, DELTOP_CODE);
							fflush(stdout);
							//leggo il nome del topic
							memset(buffer, 0, sizeof(buffer));		
							read( new_socket , buffer, 1024);
							printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Topic to delete received -> %s", PID,buffer );
							fflush(stdout);
							//delete
							//add_topic(w->topics, buffer);
							res= delete_topic(w->topics, buffer, AUTH);
							if (res == 0) {	
								send_cust(DELTOP_SUCC, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, DELTOP_SUCC);
								fflush(stdout);
							}
							else if (res == -2) {
								send_cust(DELTOP_FAIL_NOPROP, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, DELTOP_FAIL_NOPROP);
								fflush(stdout);
							}
							else {
								send_cust(DELTOP_FAIL_NOEXT, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, DELTOP_FAIL_NOEXT);
								fflush(stdout);
							}
						}
						else {
							//comando errato
							send_cust(UNKNOWN, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
							fflush(stdout);
						}
					}

					//LIST TOPICS/MESSAGES
					else if (strncmp(cmd, "list", strlen("list"))==0) {
						int res;
						cmd = strtok(NULL, " ");
						//TOPICS
						if (strncmp(cmd, "topics", strlen("topics")) == 0) {
							memset(buf, 0, sizeof(buf));
							list_topics(w->topics, buf);
							send_cust(buf, new_socket); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Topic list sent\n");
							fflush(stdout);
						}
						//MESSAGES
						else if (strncmp(cmd, "messages", strlen("messages")) == 0) {
							//TODO
							send(new_socket , ACK , 1024, 0);
							send(new_socket , LTOP_SELECT_TOPIC , 1024, 0);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, LTOP_SELECT_TOPIC);
							fflush(stdout);
							memset(buffer, 0, sizeof(buffer));
							read( new_socket , buffer, 1024);
							int topicindex = atoi(buffer);
							memset(buf, 0, sizeof(buf));
							list_messages_from_topic(w, buf, topicindex);
							send(new_socket , buf , 1024, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Topic's messages listed\n");
							fflush(stdout);
						}
						//ERROR
						else {
							//comando errato print usage
							send_cust(UNKNOWN, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
							fflush(stdout);
						}
					}

					//ADD USER
					else if (strncmp(cmd, "add", strlen("add"))==0){
						int res;
						cmd = strtok(NULL, " ");
						//TOPICS
						if (strncmp(cmd, "user", strlen("user")) == 0) {
							if (AUTH != 0) {
								send_cust(UNAUTH, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNAUTH);
								fflush(stdout);
							}
							else {	
								//Send ack and send new istructions
								//get username
								send_cust(ACK, new_socket);
								//send(new_socket , ACK , 1024, 0);
								char username[1024] = {0};
								char password[1024] = {0};
								send_cust(ADDU_INSUSR, new_socket);
								//send(new_socket , ADDU_INSUSR , 1024, 0); 
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDU_INSUSR);
								fflush(stdout);
								read( new_socket , username, 1024);
								printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Username to add received -> %s", PID,username);
								//get password
								send_cust(ACK, new_socket);
								//send(new_socket , ACK , 1024, 0);
								send_cust(ADDU_INSPWD, new_socket);
								//send(new_socket , ADDU_INSPWD , 1024, 0); 
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDU_INSPWD);
								fflush(stdout);
								read( new_socket , password, 1024);
								printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Password to add received -> %s", PID,password);
								fflush(stdout);
								int res;
								//BUG
								//printf("%s %s nuovoutente\n", username, password);
								res = create_user(w->users, username, password);
								if (res == -1) printf("Errore in creazione\n");
								//list_topics(w->topics);
								send_cust(ADDU_SUCC, new_socket);
								//send(new_socket , ADDU_SUCC , 1024, 0); 
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDU_SUCC);
								fflush(stdout);
							}
						}
						else {
							//comando errato print usage
							send_cust(UNKNOWN, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
							fflush(stdout);
						}
					}

					//SHOW USERS
					else if (strncmp(cmd, "show", strlen("show"))==0){
						int res;
						cmd = strtok(NULL, " ");
						if (cmd != NULL) {
							if (strncmp(cmd, "users", strlen("users")) == 0) {
								if (AUTH != 0) {
									send_cust(UNAUTH, new_socket);
									printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNAUTH);
									fflush(stdout);
								}
								else {	
									memset(buf, 0, sizeof(buf));
									list_users(w->users, buf);
									send_cust(buf, new_socket);
									//send(new_socket , buf , 1024, 0); 
									printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Lista dei utenti inviata\n");
									fflush(stdout);
								}
							}
							else {
								//comando errato print usage
								send_cust(UNKNOWN, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
								fflush(stdout);
							}
						}
						else {
							//comando errato print usage
							send_cust(UNKNOWN, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
							fflush(stdout);
						}
					}

					//APPEND MESSAGE
					else if (strncmp(cmd, "append", strlen("append"))==0) {
						int res;
						cmd = strtok(NULL, " ");
						if (cmd != NULL) {
							if (strncmp(cmd, "message", strlen("message")) == 0) {
								send_cust(ACK, new_socket);
								//send(new_socket , ACK , 1024, 0);
								send_cust(ADDM_SELECT_TOPIC, new_socket);
								//send(new_socket , ADDM_SELECT_TOPIC , 1024, 0); 
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDM_SELECT_TOPIC);
								fflush(stdout);
								memset(buffer, 0, sizeof(buffer));		
								read( new_socket , buffer, 1024);
								int topicindex;
								topicindex = atoi(buffer);
								printf("%d\n", topicindex);
								printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Topic selected to add message -> %s", PID,buffer );
								fflush(stdout);
								send_cust(ACK, new_socket);
								//send(new_socket , ACK , 1024, 0);
								//Richiedi testo messaggio
								send_cust(ADDM_MESSAGE_TEXT, new_socket);
								//send(new_socket , ADDM_MESSAGE_TEXT , 1024, 0); 
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDM_MESSAGE_TEXT);
								fflush(stdout);
								memset(buffer, 0, sizeof(buffer));		
								read( new_socket , buffer, 1024);
								printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Message to post sent from client -> %s", PID,buffer );
								fflush(stdout);
								add_message_to_topic(w,buffer, AUTH, topicindex, 0);
								send_cust(ADDM_MESSAGE_POSTED, new_socket);
								//send(new_socket , ADDM_MESSAGE_POSTED , 1024, 0); 
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDM_MESSAGE_POSTED);
								fflush(stdout);
							}
							else {
								//comando errato print usage
								send_cust(UNKNOWN, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
								fflush(stdout);
							}
						}
						else {
							//comando errato print usage
							send_cust(UNKNOWN, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
							fflush(stdout);
						}
					}

					//REPLY MESSAGE
					else if (strncmp(cmd, "reply", strlen("reply"))==0) {
						int res;
						cmd = strtok(NULL, " ");
						if (cmd != NULL) {
							if (strncmp(cmd, "message", strlen("message")) == 0) {
								send_cust(ACK, new_socket);
								send_cust(REPLY_SELMES, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, REPLY_SELMES);
								fflush(stdout);
								memset(buffer, 0, sizeof(buffer));		
								read(new_socket , buffer, 1024);
								int msgindex, topicindex;
								get_indexes_from_id(&topicindex, &msgindex, buffer);
								//printf("%d\n", msgindex);
								printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Message to reply selected -> %s", PID,buffer );
								fflush(stdout);
								send_cust(ACK, new_socket);
								//send(new_socket , ACK , 1024, 0);
								//Richiedi testo messaggio
								send_cust(REPLY_TEXT, new_socket);
								//send(new_socket , ADDM_MESSAGE_TEXT , 1024, 0); 
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, "Text of the reply requested");
								fflush(stdout);
								memset(buffer, 0, sizeof(buffer));		
								read( new_socket , buffer, 1024);
								printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Message to post sent from client -> %s", PID,buffer );
								fflush(stdout);
								res = add_message_to_topic(w, buffer, AUTH, topicindex, msgindex);
								if (res == -1) {
									send_cust(REPLY_ERRNOEXT, new_socket);
									printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, REPLY_ERRNOEXT);
									fflush(stdout);
								}
								else {
									send_cust(REPLY_SUCC, new_socket);
									printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, REPLY_SUCC);
									fflush(stdout);
								}
							}
							else {
								//comando errato print usage
								send_cust(UNKNOWN, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
								fflush(stdout);
							}
						}
						else {
							//comando errato print usage
							send_cust(UNKNOWN, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
							fflush(stdout);
						}
					}
					
					//UNKNOWN
					else {
						send_cust(UNKNOWN, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
						fflush(stdout);
					}
					/*
					switch(command) {	
                        
						case ADD_MESSAGE_TO_TOPIC:
							send_cust(ACK, n
	printf("%s")ew_socket);
							//send(new_socket , ACK , 1024, 0);
							send_cust(ADDM_SELECT_TOPIC, new_socket);
							//send(new_socket , ADDM_SELECT_TOPIC , 1024, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDM_SELECT_TOPIC);
							fflush(stdout);
							memset(buffer, 0, sizeof(buffer));		
							read( new_socket , buffer, 1024);
							int topicindex;
							topicindex = atoi(buffer);
							printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Topic selected to add message -> %s", PID,buffer );
							fflush(stdout);
							send_cust(ACK, new_socket);
							//send(new_socket , ACK , 1024, 0);
							//Richiedi testo messaggio
							send_cust(ADDM_MESSAGE_TEXT, new_socket);
							//send(new_socket , ADDM_MESSAGE_TEXT , 1024, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDM_MESSAGE_TEXT);
							fflush(stdout);
							memset(buffer, 0, sizeof(buffer));		
							read( new_socket , buffer, 1024);
							printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Message to post sent from client -> %s", PID,buffer );
							fflush(stdout);
							add_message_to_topic(w,buffer, AUTH, topicindex);
							send_cust(ADDM_MESSAGE_POSTED, new_socket);
							//send(new_socket , ADDM_MESSAGE_POSTED , 1024, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDM_MESSAGE_POSTED);
							fflush(stdout);
							break;

					}*/
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
                fflush(stdout);
            }
        }
    }

	return 0; 
} 
