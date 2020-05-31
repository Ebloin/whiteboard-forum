#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#define NUM_TOPIC 32
#define NUM_USERS 128
#define NUM_MESSAGES 256
#define BUF_SIZE 16384

typedef struct User user;
struct User {
	char username[32];
	char password[32];
	int subscriptions[NUM_TOPIC];
};

typedef struct Message message;
struct Message {
	char text[1024];
	int sender;
	int in_reply_to;
	int replies[NUM_MESSAGES];
	char id[9];
	int visualized;
	int visualizations[NUM_USERS]; //0->visualizzato, 1->da_visualizzare
};

typedef struct Topic topic;
struct Topic {
	char id[5];
	char name[32];
	message messages[256];
	int owner;
	int subscripteds[NUM_USERS];
};

typedef struct Whiteboard whiteboard;
struct Whiteboard {
	user users[NUM_USERS];
	topic topics[NUM_TOPIC];
};

//Check if a user exists from the user index, returns:
//- 0: user does not exists
//- 1: user exists
int check_utente_exists(whiteboard* w, int userindex);

//Check if a message has been visualized from all users, returns:
//- 0: someone didn't visualize the message
//- 1: all users visualized the message
int check_visualized(message* m);

//Recoursive function that prints a message replies indented of one tab
//Function modify the parameter buf with side effect
void print_replies(message* ml, message* m, char* buf, int topic_index, int level, int userindex);

//Check if a user is subscripted to a topic
//- 1: subscripted
//- 0: not subscripted
int check_subscription(topic* t, int userindex);

//Create a topic in the whiteboard structure
//returns topic index if success or
// -1 topic limit reached
// -2 name null
// -3 too long name (>32)
int add_topic(topic* tl, char* name, int user_creator);

//Delete a topic in the whiteboard structure, only if the requester is the owner of the topic
// -2 not owner
// -1 topic not found
// -0 ok
int delete_topic(topic* tl, char* id, int requester);

//List all topics availables in the whiteboard
//Prints all topics in buf doing side effect on the string buf
void list_topics(topic* tl, char* buf);

//List all topics subscriptions for an user
//Prints all subscripted topics in buf doing side effect on the string buf
void list_subscripted_topics(whiteboard* w, topic* tl, int userindex, char* buf);

//Append a message to the root of a topic
// -4 void message or too long message
// -3 no existent topic
// -2 not subscribed to topic
// -1 message limit reached
// 0 ok
int add_message_to_topic(whiteboard* w, char* text, int user_index, int topic_index, int inreplyto);

//List all messages in a topic, only if the user is subscribed
//Prints all subscripted topics in buf doing side effect on the string buf
void list_messages_from_topic(whiteboard* w, char* buf, int topic_index, int userindex);

//Create a new user (Reserved to the administrator)
// -1 other user with same username
// -2 username requrements not satisfied
// -3 password requrements not satisfied
int create_user(user* ul, char* username, char* password);

//delete user (only admin)
// -1 trying to delte admin
// -2 no existent user
// 0 ok
int delete_user(whiteboard* w, int u);

//List all the users in the whiteboard (Reserved to the administrator)
// prints users in the string buf doing side effect
void list_users(user* ul, char* buf);

//Login to the whiteboard
// -1 login error
// 0 logged in
int login(user* ul, char* username, char* password);

//Subscribe a user to a topic
// -1 no existent topic
// -2 already subscripted
// else ok
int subscribe_to_topic(whiteboard* w, int topicindex, int userindex);

//Custom function to send data over the socket, pad the sent string to BUF_SIZE bytes
void send_cust(char* text, int socket);

//Get the topic_index and Message_index from a message ID
//MESSAGE ID = XXXXYYYY where XXXX->topic_index and YYYY->message_index padded with zeroes
void get_indexes_from_id(int* topicindex, int* msgindex, char* id);

//Get the topic index from topic ID
void get_topic_index(int* topicindex, char* topic_id);

//Prints the status of a message, a message is visualized when all users subscribed to the topic
//when the message was sent visualized the message.
int print_status_message(whiteboard* w, int topic_index, int msg_index, char* buf, int requester);

// semaphores
int initsem (key_t semkey);
int Pwait (int semid);
int Vpost (int semid);
void semclean(key_t semkey);