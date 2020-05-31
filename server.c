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
	char buffer[BUF_SIZE] = {0};
	char buf[BUF_SIZE] = {0};
	char sendbuf[BUF_SIZE] = {0};
	
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
	memset(w,0,sizeof(whiteboard));
	create_user(w->users, "admin", "admin");
	create_user(w->users, "test", "test");
	add_topic(w->topics, "WHITEBOARD RULES\0", 0);
	subscribe_to_topic(w, 1, 1);
	add_message_to_topic(w, "Welcome to whiteboard forum\0", 0, 1, 0);
	shmdt(w);

	//Semafori
	key_t semkey = IPC_PRIVATE;
	int mutex;
	semclean(semkey);
	if ((mutex = initsem(semkey)) < 0) {
		perror("initsem");
		exit(1);
	}

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
					char username[BUF_SIZE] = {0};
					char password[BUF_SIZE] = {0};
					send_cust(LOGIN_REQUSR, new_socket);
					printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, "Richiesta username");
					fflush(stdout);
					memset(username, 0, sizeof(username));		
					read( new_socket , username, BUF_SIZE);
					printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Username for login received -> %s\n", PID,username);
					fflush(stdout);
					send_cust(ACK, new_socket);
					send_cust(LOGIN_REQPASS, new_socket);
					printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, "Richiesta password");
					memset(password, 0, sizeof(password));		
					read( new_socket , password, BUF_SIZE);
					printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Password for login received -> %s\n", PID,password);
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
				read( new_socket , buffer, BUF_SIZE);
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
					read( new_socket , buffer, BUF_SIZE);
					cmd = buffer;
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
					else if (strncmp(cmd, "create topic", strlen("create topic"))==0) {
						int res;
						send_cust(ACK, new_socket);
						send_cust(ADDT_COMMANDS, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDT_COMMANDS);
						fflush(stdout);
						//leggo il nome del topic
						memset(buffer, 0, sizeof(buffer));		
						read( new_socket , buffer, BUF_SIZE);
						printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Topicname to add received -> %s\n", PID,buffer );
						fflush(stdout);
						Pwait(mutex);
						res = add_topic(w->topics, buffer, AUTH);
						Vpost(mutex);
						//error
						if (res == -1) {
							send_cust(ADDT_ERROR, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDT_ERROR);
							fflush(stdout);
						}	
						else if (res == -2) {
							send_cust(ADDT_ERROR_NULL, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDT_ERROR_NULL);
							fflush(stdout);
						}
						else if (res == -3) {
							send_cust(ADDT_ERROR_TOOLONG, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDT_ERROR_TOOLONG);
							fflush(stdout);
						}
						//success						
						else {
							send_cust(ADDT_SUCCESS, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDT_SUCCESS);
							fflush(stdout);
							Pwait(mutex);
							subscribe_to_topic(w, res, AUTH);
							Vpost(mutex);
						}
					}

					//DELETE TOPIC
					else if (strncmp(cmd, "delete topic", strlen("delete topic"))==0) {
						int res;
						send_cust(ACK, new_socket);
						send_cust(DELTOP_CODE, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, DELTOP_CODE);
						fflush(stdout);
						memset(buffer, 0, sizeof(buffer));		
						read( new_socket , buffer, BUF_SIZE);
						printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Topic to delete received -> %s\n", PID,buffer );
						fflush(stdout);
						Pwait(mutex);
						res= delete_topic(w->topics, buffer, AUTH);
						Vpost(mutex);
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

					//DELETE USER
					else if (strncmp(cmd, "delete user", strlen("delete user"))==0) {
						int res;
						if (AUTH != 0) {
							send_cust(UNAUTH, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNAUTH);
							fflush(stdout);
						}
						else {
							send_cust(ACK, new_socket);
							send_cust(DELUSR_INS_USR, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, DELUSR_INS_USR);
							fflush(stdout);
							//leggo il nome del topic
							memset(buffer, 0, sizeof(buffer));		
							read( new_socket , buffer, BUF_SIZE);
							printf("\033[1;33m[S <-- %d] CLIENT:\033[0m User to delete received -> %s\n", PID,buffer );
							fflush(stdout);
							int u=atoi(buffer);
							//delete
							//add_topic(w->topics, buffer);
							Pwait(mutex);
							res= delete_user(w, u);
							Vpost(mutex);

							if (res == -1) {
								send_cust(DELUSR_ERR_ADM, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, DELUSR_ERR_ADM);
								fflush(stdout);
							}

							else if (res == -2) {
								send_cust(DELUSR_ERR_NOEXT, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, DELUSR_ERR_NOEXT);
								fflush(stdout);
							}

							else {	
								send_cust(DELUSR_OK, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, DELUSR_OK);
								fflush(stdout);
							}
						}
					}

					//LIST AVAILABLE TOPICS
					else if (strncmp(cmd, "list available topics", strlen("list available topics"))==0) {
						memset(buf, 0, sizeof(buf));
						Pwait(mutex);
						list_topics(w->topics, buf);
						Vpost(mutex);
						send_cust(buf, new_socket); 
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Topic list sent\n");
						fflush(stdout);
					}

					//LIST SUBSCRIPTED TOPICS
					else if (strncmp(cmd, "list subscripted topics", strlen("list subscripted topics"))==0) {
						memset(buf, 0, sizeof(buf));
						Pwait(mutex);
						list_subscripted_topics(w, w->topics, AUTH, buf);
						Vpost(mutex);
						send_cust(buf, new_socket); 
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Topic list sent\n");
						fflush(stdout);
					}

					//LIST MESSAGES
					else if (strncmp(cmd, "list messages", strlen("list messages"))==0) {
						send_cust(ACK, new_socket);
						send_cust(LTOP_SELECT_TOPIC, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, LTOP_SELECT_TOPIC);
						fflush(stdout);
						memset(buffer, 0, sizeof(buffer));
						read( new_socket , buffer, BUF_SIZE);
						int topicindex;// = atoi(buffer);
						get_topic_index(&topicindex, buffer);
						//printf("DEBUG: topic index->%d\n", topicindex);
						memset(buf, 0, sizeof(buf));
						Pwait(mutex);
						list_messages_from_topic(w, buf, topicindex, AUTH);
						Vpost(mutex);
						send_cust(buf, new_socket);
						//send(new_socket , buf , BUF_SIZE, 0); 
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Topic's messages listed\n");
						fflush(stdout);
					}

					//ADD USER
					else if (strncmp(cmd, "add user", strlen("add user"))==0) {
						if (AUTH != 0) {
							send_cust(UNAUTH, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNAUTH);
							fflush(stdout);
						}
						else {	
							//Send ack and send new istructions
							//get username
							send_cust(ACK, new_socket);
							//send(new_socket , ACK , BUF_SIZE, 0);
							char username[BUF_SIZE] = {0};
							char password[BUF_SIZE] = {0};
							send_cust(ADDU_INSUSR, new_socket);
							//send(new_socket , ADDU_INSUSR , BUF_SIZE, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDU_INSUSR);
							fflush(stdout);
							read( new_socket , username, BUF_SIZE);
							printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Username to add received -> %s\n", PID,username);
							//get password
							send_cust(ACK, new_socket);
							//send(new_socket , ACK , BUF_SIZE, 0);
							send_cust(ADDU_INSPWD, new_socket);
							//send(new_socket , ADDU_INSPWD , BUF_SIZE, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDU_INSPWD);
							fflush(stdout);
							read( new_socket , password, BUF_SIZE);
							printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Password to add received -> %s\n", PID,password);
							fflush(stdout);
							int res;
							//BUG
							//printf("%s %s nuovoutente\n", username, password);
							Pwait(mutex);
							res = create_user(w->users, username, password);
							Vpost(mutex);
							//list_topics(w->topics);

							if (res==-2) {
								send_cust(ADDU_ERR_USR, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDU_ERR_USR);
								fflush(stdout);
							}

							else if (res==-3) {
								send_cust(ADDU_ERR_PAS, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDU_ERR_PAS);
								fflush(stdout);
							}

							else if (res==-1) {
								send_cust(ADDU_ERR_LIM, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDU_ERR_LIM);
								fflush(stdout);
							}

							else {
								send_cust(ADDU_SUCC, new_socket);
								printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDU_SUCC);
								fflush(stdout);
							}
						}
					}

					//SHOW USERS
					else if (strncmp(cmd, "show users", strlen("show users"))==0) {
						if (AUTH != 0) {
							send_cust(UNAUTH, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNAUTH);
							fflush(stdout);
						}
						else {	
							memset(buf, 0, sizeof(buf));
							Pwait(mutex);
							list_users(w->users, buf);
							Vpost(mutex);
							send_cust(buf, new_socket);
							//send(new_socket , buf , BUF_SIZE, 0); 
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Lista dei utenti inviata\n");
							fflush(stdout);
						}
					}

					//APPEND MESSAGE
					else if (strncmp(cmd, "create thread", strlen("create thread"))==0) {
						int res;
						send_cust(ACK, new_socket);
						//send(new_socket , ACK , BUF_SIZE, 0);
						send_cust(ADDM_SELECT_TOPIC, new_socket);
						//send(new_socket , ADDM_SELECT_TOPIC , BUF_SIZE, 0); 
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDM_SELECT_TOPIC);
						fflush(stdout);
						memset(buffer, 0, sizeof(buffer));		
						read( new_socket , buffer, BUF_SIZE);
						int topicindex;
						//topicindex = atoi(buffer);
						get_topic_index(&topicindex, buffer);
						//printf("%d\n", topicindex);
						printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Topic selected to add message -> %s\n", PID,buffer );
						fflush(stdout);
						send_cust(ACK, new_socket);
						//send(new_socket , ACK , BUF_SIZE, 0);
						//Richiedi testo messaggio
						send_cust(ADDM_MESSAGE_TEXT, new_socket);
						//send(new_socket , ADDM_MESSAGE_TEXT , BUF_SIZE, 0); 
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDM_MESSAGE_TEXT);
						fflush(stdout);
						memset(buffer, 0, sizeof(buffer));		
						read( new_socket , buffer, BUF_SIZE);
						printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Message to post sent from client -> %s\n", PID,buffer );
						fflush(stdout);
						Pwait(mutex);
						res = add_message_to_topic(w,buffer, AUTH, topicindex, 0);
						Vpost(mutex);

						if (res == -2) {
							send_cust(ADDM_ERROR_NOSUBSC, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDM_ERROR_NOSUBSC);
							fflush(stdout);
						}

						else if (res == -4) {
							send_cust(ADDM_ERROR_TEXT, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDM_ERROR_TEXT);
							fflush(stdout);
						}

						else if (res == -3) {
							send_cust(ADDM_ERROR_NOEXT, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDM_ERROR_NOEXT);
							fflush(stdout);
						}

						else {
							send_cust(ADDM_MESSAGE_POSTED, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDM_MESSAGE_POSTED);
							fflush(stdout);
						}
					}

					//REPLY MESSAGE
					else if (strncmp(cmd, "reply message", strlen("reply message"))==0) {
						int res;
						send_cust(ACK, new_socket);
						send_cust(REPLY_SELMES, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, REPLY_SELMES);
						fflush(stdout);
						memset(buffer, 0, sizeof(buffer));		
						read(new_socket , buffer, BUF_SIZE);
						int msgindex, topicindex;
						get_indexes_from_id(&topicindex, &msgindex, buffer);
						//printf("%d\n", msgindex);
						printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Message to reply selected -> %s\n", PID,buffer );
						fflush(stdout);
						send_cust(ACK, new_socket);
						//send(new_socket , ACK , BUF_SIZE, 0);
						//Richiedi testo messaggio
						send_cust(REPLY_TEXT, new_socket);
						//send(new_socket , ADDM_MESSAGE_TEXT , BUF_SIZE, 0); 
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, "Text of the reply requested");
						fflush(stdout);
						memset(buffer, 0, sizeof(buffer));		
						read( new_socket , buffer, BUF_SIZE);
						printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Message to post sent from client -> %s\n", PID,buffer );
						fflush(stdout);
						Pwait(mutex);
						res = add_message_to_topic(w, buffer, AUTH, topicindex, msgindex);
						Vpost(mutex);
						if (res == -1 || res == -3) {
							send_cust(REPLY_ERRNOEXT, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, REPLY_ERRNOEXT);
							fflush(stdout);
						}

						else if (res == -4) {
							send_cust(ADDM_ERROR_TEXT, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, ADDM_ERROR_TEXT);
							fflush(stdout);
						}

						else {
							send_cust(REPLY_SUCC, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, REPLY_SUCC);
							fflush(stdout);
						}
					}

					//SUBSCRIBE TO TOPIC
					else if (strncmp(cmd, "subscribe to topic", strlen("subscribe to topic"))==0) {
						int res;
						send_cust(ACK, new_socket);
						send_cust(TOPSUB_NAME, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, ADDT_COMMANDS);
						fflush(stdout);
						//leggo il nome del topic
						memset(buffer, 0, sizeof(buffer));		
						read( new_socket , buffer, BUF_SIZE);
						printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Topic code to subscribe received -> %s\n", PID,buffer );
						fflush(stdout);
						int topic_index = atoi(buffer);
						//res = add_topic(w->topics, buffer, AUTH);
						Pwait(mutex);
						res = subscribe_to_topic(w, topic_index, AUTH);
						Vpost(mutex);
						//error
						if (res == -1) {
							send_cust(TOPSUB_FAIL_NOEXT, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, TOPSUB_FAIL_NOEXT);
							fflush(stdout);
						}	

						else if (res == -2) {
							send_cust(TOPSUB_FAIL_ALREADY, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, TOPSUB_FAIL_ALREADY);
							fflush(stdout);
						}	

						//success						
						else {
							send_cust(TOPSUB_SUCC, new_socket);
							printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, TOPSUB_SUCC);
							fflush(stdout);
						}	
					}

					//STATUS MESSAGE
					else if (strncmp(cmd, "message status", strlen("message status"))==0) {
						int res;
						send_cust(ACK, new_socket);
						send_cust(STAT_MSGCODE, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s\n", PID, STAT_MSGCODE);
						fflush(stdout);
						memset(buffer, 0, sizeof(buffer));		
						read( new_socket , buffer, BUF_SIZE);
						int topicindex, msgindex;
						get_indexes_from_id(&topicindex, &msgindex, buffer);
						//printf("%d\n", topicindex);
						printf("\033[1;33m[S <-- %d] CLIENT:\033[0m Message code sent from client -> %s\n", PID,buffer );
						fflush(stdout);
						memset(buf, 0, sizeof(buf));
						Pwait(mutex);
						print_status_message(w, topicindex, msgindex, buf, AUTH);
						Vpost(mutex);
						send_cust(buf, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, "Message status sent\n");
						fflush(stdout);
					}

					//UNKNOWN
					else {
						send_cust(UNKNOWN, new_socket);
						printf("\033[1;32m[S --> %d] SERVER:\033[0m %s", PID, UNKNOWN);
						fflush(stdout);
					}
				}
				//endtest
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
