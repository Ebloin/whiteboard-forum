#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM_TOPIC 256
#define NUM_USERS 256
#define NUM_MESSAGES 256
#define BUF_SIZE 1024

typedef struct User user;
struct User {
	char username[64];
	char password[64];
	int topic_subscriptions[NUM_TOPIC];
};

typedef struct Message message;
struct Message {
	char text[1024];
	int sender; //In realtà è un intero contestualizzato alla shd_mem
};

typedef struct Topic topic;
struct Topic {
	char name[64];
	message messages[256];

};

typedef struct Whiteboard whiteboard;
struct Whiteboard {
	user users[NUM_USERS];
	topic topics[NUM_TOPIC];
};

int add_topic(topic* tl, char* name) {
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
	return i;
}

void list_topics(topic* tl, char* buf) {
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
			sprintf(tmp, "%d)%s", i, current->name);
			strcat(buf, tmp);
		}	
	}
}

int authenticate(user* lu, char* username, char* password) {
	user* current;
	int i;
	for (i=0; i<NUM_USERS; i++) {
		current = &lu[i];
		if (strcmp(username, current->username)==0) {
			if (strcmp(password, current->password)==0) {
				return 1;
			}
		}
	}
	return 0;
}

int add_message_to_topic(whiteboard* w, char* text, int user_index, int topic_index) {
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

int addUser(whiteboard* w, char* username, char* password) {
	return 0;
}

void list_messages_from_topic(whiteboard* w, char* buf, int topic_index) {
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