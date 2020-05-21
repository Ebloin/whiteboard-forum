#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> 
#define NUM_TOPIC 256
#define NUM_USERS 256
#define NUM_MESSAGES 256
#define BUF_SIZE 1024

typedef struct User user;
struct User {
	char username[32];
	char password[32];
	int topic_subscriptions[NUM_TOPIC];
	int sent_messages[NUM_MESSAGES];
};

typedef struct Message message;
struct Message {
	char text[1024];
	int sender; //In realtà è un intero contestualizzato alla shd_mem
	int in_reply_to;
	int replies[NUM_MESSAGES];
};

typedef struct Topic topic;
struct Topic {
	char id[5];
	char name[32];
	message messages[256];
	int owner;
};

typedef struct Whiteboard whiteboard;
struct Whiteboard {
	user users[NUM_USERS];
	topic topics[NUM_TOPIC];
};

static int add_topic(topic* tl, char* name, int user_creator) {
	int i =0;
	topic* current;
	for (i; i<NUM_TOPIC; i++) {
		if (i == NUM_TOPIC) {
			printf("Topic limit reached\n");
			return -1;
		}
		current = &tl[i];
		if (strcmp(current->name, "") == 0) {
			break;
		}
	}
	strcpy(current->name, name);
	char id[5];
	sprintf(id, "%04d", i);
	printf("%s\n", id);
	strcpy(current->id, id);
	current->owner = user_creator;
	return i;
}

static int delete_topic(topic* tl, char* id, int requester) {
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

static void list_topics(topic* tl, char* buf) {
	strcpy(buf, "Lista dei topic presenti nella whiteboard:\n");
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
			sprintf(tmp, "%s] %s", current->id, current->name);
			strcat(buf, tmp);
		}	
	}
}

static int add_message_to_topic(whiteboard* w, char* text, int user_index, int topic_index) {
	message* ml = w->topics[topic_index].messages;
	message* current;
	int i;
	for (i=0; i<NUM_MESSAGES; i++) {
		if (i == NUM_MESSAGES) {
			return -1;
		}
		current = &ml[i];
		if (strcmp(current->text, "") == 0) {
			break;
		}
	}
	strcpy(current->text, text);
	current->sender = user_index;
	return 0;
}

static void list_messages_from_topic(whiteboard* w, char* buf, int topic_index) {
	message* ml = w->topics[topic_index].messages;
	sprintf(buf, "Lista dei messaggi presenti sul topic [%d]%s", topic_index, w->topics[topic_index].name);
	message* current;
	char tmp[BUF_SIZE]={0};
	int i;
	for (i=0; i<NUM_MESSAGES; i++) {
		if (i == NUM_MESSAGES) {
			return;
		}
		current = &ml[i];
		if (strcmp(current->text, "") != 0) {
			sprintf(tmp, "%d)%s", i, current->text);
			strcat(buf, tmp);
		}
	}
}

static int create_user(user* ul, char* username, char* password) {
	int i=0;
	user* current;
	for (i; i<NUM_USERS; i++) {
		if (i == NUM_USERS) {
			printf("Users limit reached\n");
			return -1;
		}
		current = &ul[i];
		if (strcmp(current->username, username) == 0) {
			//TODO differenziazione errori
			return -1;
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

static void list_users(user* ul, char* buf) {
	strcpy(buf, "Lista degli utenti presenti nella whiteboard:\n");
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
			sprintf(tmp, "%d)%s", i, current->username);
			strcat(buf, tmp);
		}	
	}
}

static int login(user* ul, char* username, char* password) {
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

static void send_cust(char* text, int socket) {
	char sendbuf[BUF_SIZE] = {0};
	strncpy(sendbuf, text, strlen(text));
	send(socket, sendbuf, BUF_SIZE, 0);
}

/*
int main() {
	//printf("Prevista: %ld	Ottenuta:%ld	Test:%ld\n", WHITEBOARD_SIZE, size, TOPICS_LIST_SIZE+USERS_LIST_SIZE);
	whiteboard* w = malloc(sizeof(whiteboard));
	memset(w,0,sizeof(whiteboard));
	topic* tl = w->topics;
	char*s = "topic1";
	add_topic(tl, s);
	list_topics(tl);
	add_topic(tl, "vaffanculo");
	list_topics(tl);
	add_topic(tl, "stotopicdimerda");
	add_topic(tl, "mefaschifo");
	list_topics(tl);
}
*/