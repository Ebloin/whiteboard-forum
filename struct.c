#include "struct.h"
#define RED "\033[0;31m"    
#define BOLD_RED "\033[1;31m"	
#define GREEN "\033[0;32m"	
#define BOLD_GREEN  "\033[1;32m"	
#define YELLOW "\033[0;33m"	
#define BOLD_YELLOW "\033[1;33m"	
#define BLUE "\033[0;34m"	
#define BOLD_BLUE "\033[1;34m"	
#define MAGENTA "\033[0;35m"	
#define BOLD_MAGENTA "\033[1;35m"	
#define CYAN "\033[0;36m"	
#define BOLD_CYAN "\033[1;36m"	
#define RESET "\033[0m"   	
#define BACK_GREEN "\033[1;42m"
#define BACK_RED "\033[1;41m"
#define BACK_BLUE "\033[1;44m"

int check_utente_exists(whiteboard* w, int userindex) {
	if (strcmp(w->users[userindex].username, "")==0) {
		return 0;
	}
	else {
		return 1;
	}
}

int check_visualized(message* m) {
	int i;
	for (i=0; i<NUM_USERS; i++) {
		if (m->visualizations[i] == 1) {
			return 0;
		}
	}
	return 1;
}

void print_replies(message* ml, message* m, char* buf, int topic_index, int level, int userindex, whiteboard* w) {
	int i=0,l=0;
	char tmp[BUF_SIZE+100]={0};
	message* current;
	for (i=0; i<NUM_MESSAGES; i++) {
		if (m->replies[i] != 0) {
			//il messaggio con indice il numero dentro replies
			current = &ml[m->replies[i]];
			for (l=0; l<level; l++) {
				strcat(buf, "\t");
			}
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, BOLD_BLUE"[#%s - %s]" RESET " %s\n", current->id, w->users[current->sender].username, current->text);
			strcat(buf, tmp);
			if (current->visualized == 0) {
				//se l'utente deve visualizzare
				if (current->visualizations[userindex] == 1) {
					//setto il visualizzato
					current->visualizations[userindex] = 0;
					current->visualized = check_visualized(current);
				}
			}

			if (current->replies[0] != 0) {
				print_replies(ml, current, buf, topic_index, level+1, userindex, w);
			}
		}
	}
}

int check_subscription(topic* t, int userindex) {
	int i;
	for (i=0; i<NUM_USERS; i++) {
		if (t->subscripteds[i] == userindex) {
			return 1;
		}
	}
	return 0;
}

int add_topic(topic* tl, char* name, int user_creator) {
	//null name
	if (strcmp(name, "\n")==0 || strlen(name)==0) {
		return -2;
	}
	//too long name
	else if (strlen(name) >= 32) {
		return -3;
	}
	int i =0;
	topic* current;
	for (i; i<=NUM_TOPIC; i++) {
		if (i == NUM_TOPIC) {
			//printf("Topic limit reached\n");
			return -1;
		}
		current = &tl[i];
		if (strcmp(current->name, "") == 0 && i != 0) {
			break;
		}
	}
	strcpy(current->name, name);
	char id[5];
	sprintf(id, "%04d", i);
	//printf("%s\n", id);
	strcpy(current->id, id);
	current->owner = user_creator;
	return i;
}

int delete_topic(topic* tl, char* id, int requester) {
	int i=0;
	topic* current;
	for (i; i<NUM_TOPIC; i++) {
		if (i == NUM_TOPIC) {
			return -1;
		}
		current = &tl[i];
		if (strncmp(current->id, id, 4)==0) {
			if (current->owner == requester) {
				memset(current, 0, sizeof(topic));
				return 0;
			}
			//Unauthorized
			else return -2;
		}	
	}
}

void list_topics(topic* tl, char* buf) {
	strcpy(buf, BACK_BLUE"AVAILABLE TOPICS:                                                 \n"RESET);
	char tmp[BUF_SIZE]={0};
	int i=0;
	topic* current;
	for (i; i<NUM_TOPIC; i++) {
		//TODO
		//DECIDI SE SCORRERLA TUTTA O FERMARTI AL PRIMO NULL
		if (i == NUM_TOPIC) {
			return;
		}
		current = &tl[i];
		if (strcmp(current->name, "")!=0) {
			sprintf(tmp, BOLD_BLUE"[%s] %s\n"RESET, current->id, current->name);
			strcat(buf, tmp);
		}	
	}
	strcat(buf, "\n");
}

void list_subscripted_topics(whiteboard* w, topic* tl, int userindex, char* buf) {
	strcpy(buf, BACK_BLUE"SUBSCRIPTED TOPICS:                                                 \n"RESET);
	char tmp[BUF_SIZE]={0};
	user* u = &w->users[userindex];
	int i=0, anysubscription=0;
	topic* current;
	for (i; i<NUM_TOPIC; i++) {
		//TODO
		//DECIDI SE SCORRERLA TUTTA O FERMARTI AL PRIMO NULL

		if(u->subscriptions[i] != 0) {
			current = &tl[u->subscriptions[i]];
			sprintf(tmp, BOLD_BLUE"[%s] %s\n"RESET, current->id, current->name);
			strcat(buf, tmp);
			anysubscription++;
		}
	}
	if (anysubscription == 0) {
		strcat(buf, BOLD_RED"ERROR: User is not subscribed to any topic!\n"RESET);
	}
}

int add_message_to_topic(whiteboard* w, char* text, int user_index, int topic_index, int inreplyto) {
	if (strcmp(text, "\n")==0 || strlen(text)==0 || strlen(text)>=63) {
		return -4;
	}

	if (topic_index >= NUM_TOPIC ||strcmp(w->topics[topic_index].name, "") == 0) {
		return -3;
	}

	if (!check_subscription(&w->topics[topic_index], user_index)) {
		//not subscribed
		return -2;
	}
	message* ml = w->topics[topic_index].messages;
	if (inreplyto!=0 && strcmp(ml[inreplyto].text, "")==0) {
		//reply to non existent message
		return -5;
	}
	message* current;
	int i,l,x;
	for (i=0; i<=NUM_MESSAGES; i++) {
		if (i == NUM_MESSAGES) {
			return -1;
		}
		current = &ml[i];
		if (strcmp(current->text, "") == 0 && i!=0) {
			break;
		}
	}
	strcpy(current->text, text);
	current->sender = user_index;
	current->in_reply_to = inreplyto;
	//aggiunta id
	char id[9];
	sprintf(id, "%04d%04d", topic_index, i);
	strcpy(current->id, id);

	//aggiorno per le replies
	message* reply_to;
	if (inreplyto != 0) {
		reply_to = &ml[inreplyto];
		for (l=0; l<NUM_MESSAGES; l++) {
			if (reply_to->replies[l] == 0) {
				reply_to->replies[l] = i;
				break;
			}
		}
	}

	//Creo l'array delle visualizzazioni
	//per ogni utente
	for (x=1; x<NUM_USERS; x++) {
		//Se l'utente è sottoscritto
		if (check_utente_exists(w, x)) {
			//da visualizzare
			if (check_subscription(&w->topics[topic_index], x) && x!= user_index) {
				current->visualizations[x] = 1;
			}
			//non sottoscritto
			else {
				current->visualizations[x] = 2;
			}
		}
	}
	return 0;
}

void list_messages_from_topic(whiteboard* w, char* buf, int topic_index, int userindex) {
	if (topic_index>=NUM_TOPIC) {
		strcat(buf, BOLD_RED"ERROR: Selected topic doesn't exists!\n"RESET);
		return;
	}

	else if (strcmp(w->topics[topic_index].name, "") ==0 || topic_index ==0 || topic_index>=NUM_TOPIC) {
		strcat(buf, BOLD_RED"ERROR: Selected topic doesn't exists!\n"RESET);
		return;
	}

	message* ml = w->topics[topic_index].messages;
	if (!check_subscription(&w->topics[topic_index], userindex)) {
		//Scrivi sul buf che non puoi leggere
		strcat(buf, BOLD_RED"ERROR: You are not subscribed to the topic, subscribe to read messages!\n"RESET);
		return;
	}

	sprintf(buf, BACK_BLUE"[%04d] %s                                                 \n"RESET, topic_index, w->topics[topic_index].name);
	message* current;
	char tmp[BUF_SIZE+100]={0};
	int i;
	for (i=0; i<NUM_MESSAGES; i++) {
		if (i == NUM_MESSAGES) {
			return;
		}
		current = &ml[i];
		if (strcmp(current->text, "") != 0) {
			if (current->in_reply_to == 0) {
				sprintf(tmp, BOLD_BLUE"[#%s - %s]" RESET " %s\n", current->id, w->users[current->sender].username, current->text);
				strcat(buf, tmp);
				//se non ho visualizzato, visualizza
				if (current->visualized == 0) {
					//se l'utente deve visualizzare
					if (current->visualizations[userindex] == 1) {
						//setto il visualizzato
						current->visualizations[userindex] = 0;
						current->visualized = check_visualized(current);
					}
				}

				//controllo se ci stanno replies
				if (current->replies[0] != 0) {
					print_replies(ml, current, buf, topic_index, 1, userindex, w);
				}
			}
		}
	}
}

int create_user(user* ul, char* username, char* password) {
	int i=0;
	user* current;
	if (strcmp(username, "\n")==0 || strlen(username)==0 || strlen(username)>=32) {
		//check username
		return -2;
	}

	if (strcmp(password, "\n")==0 || strlen(password)==0 || strlen(password)>=32) {
		//check username
		return -3;
	}

	for (i; i<=NUM_USERS; i++) {
		if (i == NUM_USERS) {
			//printf("Users limit reached\n");
			return -1;
		}
		current = &ul[i];
		if (strcmp(current->username, username) == 0) {
			return -4;
		}
		if (strcmp(current->username, "") == 0) {
			break;
		}
	}
	strcpy(current->username, username);
	strcpy(current->password, password);
	//printf("user: %s, pass:%s\n", current->username, current->password);
	return i;
}

int delete_user(whiteboard* w, int u) {
	if (u == 0) {
		//cannot delete admin
		return -1;
	}
	else if (u<0 ||u >= NUM_USERS || strcmp(w->users[u].username, "")==0) {
		//no existent user
		return -2;
	}
	else {
		user* utente = &w->users[u];
		memset(utente, 0, sizeof(user));
		return 0;
	}
}

void list_users(user* ul, char* buf) {
	strcpy(buf, BACK_BLUE"USERS REGISTERED:                                                 \n"RESET);
	char tmp[BUF_SIZE]={0};
	int i=0;
	user* current;
	for (i; i<NUM_USERS; i++) {
		//TODO
		//DECIDI SE SCORRERLA TUTTA O FERMARTI AL PRIMO NULL
		if (i == NUM_USERS) {
			return;
		}
		current = &ul[i];
		if (strcmp(current->username, "")!=0) {
			sprintf(tmp, BOLD_BLUE"%d) %s\n"RESET, i, current->username);
			strcat(buf, tmp);
		}	
	}
}

int login(user* ul, char* username, char* password) {
	char tmp[BUF_SIZE]={0};
	int i=0;
	user* current;
	for (i; i<NUM_USERS; i++) {
		current = &ul[i];
		if (strcmp(current->username, username)==0 && strcmp(current->password, password)==0) {
			return i;
		}
	}
	return -1;
}

int subscribe_to_topic(whiteboard* w, int topicindex, int userindex) {
	int i;

	if (topicindex >= NUM_TOPIC || strcmp(w->topics[topicindex].name, "") == 0) {
		return -1;
	}

	//Aggiungi il topic alle subscriptions dell'utente
	user* u = &w->users[userindex];
	for (i=0; i<NUM_TOPIC; i++) {
		if (u->subscriptions[i] == topicindex) {
			return -2;
		}
		if (u->subscriptions[i] == 0) {
			u->subscriptions[i] = topicindex;
			break;
		}
	}

	//aggiungi l'utente ai subscripted del topic
	topic* t = &w->topics[topicindex];
	for (i=0; i<NUM_USERS; i++) {
		if (t->subscripteds[i] == 0) {
			t->subscripteds[i] = userindex;
			break;
		}
	}
	return 0;
}

void send_cust(char* text, int socket) {
	char sendbuf[BUF_SIZE] = {0};
	strncpy(sendbuf, text, strlen(text));
	send(socket, sendbuf, BUF_SIZE, 0);
}

void get_indexes_from_id(int* topicindex, int* msgindex, char* id) {
	char top[5]={0}, msg[5]={0};
	memcpy(top, &id[0], 4);
	memcpy(msg, &id[4], 4);
	*topicindex = atoi(top);
	*msgindex = atoi(msg);
	//printf("%d -> topic, %d -> message\n", *topicindex, *msgindex);
	return;
}

void get_topic_index(int* topicindex, char* topic_id) {
	char topic[5] = {0};
	memcpy(topic, &topic_id[0], 4);
	*topicindex = atoi(topic);
}

int print_status_message(whiteboard* w, int topic_index, int msg_index, char* buf, int requester) {
	if (topic_index >= NUM_TOPIC || msg_index >= NUM_MESSAGES) {
		strcat(buf, BOLD_RED"ERROR: Selected message doesn't exists\n"RESET);
		return -1;
	}
	
	message* m = &(w->topics[topic_index].messages[msg_index]);
	if (strcmp(m->text, "")==0) {
		strcat(buf,BOLD_RED "ERROR: Selected message doesn't exists\n"RESET);
		return -1;
	}
	else if (m->sender != requester) {
		strcat(buf, BOLD_RED"ERROR: Only the sender can see the status of a message\n"RESET);
		return -1;
	}

	char tmp[BUF_SIZE]={0};
	int i;
	sprintf(tmp, BACK_BLUE"STATUS OF MESSAGE [#%04d%04d]:                             \n"RESET, topic_index, msg_index);
	strcpy(buf, tmp);
	strcat(buf, BOLD_BLUE"Message delivered:\n"RESET);
	for (i=1; i<NUM_USERS; i++) {
		if (check_utente_exists(w, i) && i != requester) {
			if (m->visualizations[i] == 1) {
				sprintf(tmp, " -> %s\n", w->users[i].username);
				strcat(buf, tmp);
			}
		}
	}

	sprintf(tmp, BOLD_BLUE"Message read:\n"RESET);
	strcat(buf, tmp);
	for (i=1; i<NUM_USERS; i++) {
		if (check_utente_exists(w, i) && i!= requester) {
			if (m->visualizations[i] == 0) {
				sprintf(tmp, " -> %s\n", w->users[i].username);
				strcat(buf, tmp);
			}
		}
	}

	return 0;
}

int initsem (key_t semkey)
{
    int status = 0, semid;
    union semun {
      int val;
      struct semid_ds *stat;
    } ctl_arg;

    if ((semid = semget (semkey, 1, 0600 | IPC_CREAT)) > 0) {
      ctl_arg.val = 1;
      status = semctl(semid, 0, SETVAL, ctl_arg);
    }

    if (semid < 0 || status < 0) {
      perror ("initsem");
      return (-1);
    }
    else return (semid);
}

int Pwait (int semid)
{

    struct sembuf p_buf;

    p_buf.sem_num = 0;
    p_buf.sem_op = -1;
    p_buf.sem_flg = 0;

    if (semop (semid, &p_buf, 1) == -1) {
      perror ("Pwait(semid) failed");
      return (-1);
    }
    else {
      return (0);
    }
}

int Vpost (int semid)
{

    struct sembuf v_buf;

    v_buf.sem_num = 0;
    v_buf.sem_op = 1;
    v_buf.sem_flg = 0;

    if (semop (semid, &v_buf, 1) == -1) {
      perror ("Vpost(semid) failed");
      return (-1);
    }
    else {
      return (0);
    }
}

void semclean(key_t semkey){
  int semid;
  union semun {
    int val;
    struct semid_ds *stat;
  } ctl_arg;

  if ((semid = semget(semkey, 1, IPC_CREAT|0666)) > 0) {
    ctl_arg.val = 1;
    semctl(semid, 0, SETVAL, ctl_arg);
  }

  semctl(semid, 0, IPC_RMID);
}