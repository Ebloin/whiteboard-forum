#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct User user;
struct User {
    char* username;
    char* password;
    user* next;
};

typedef struct Users_list users_list;
struct Users_list {
    user* head;
    user* tail;
};

typedef struct Message message;
struct Message {
    char* text;
    user* sender;
    message* next;
};

typedef struct Messages_list messages_list;
struct Messages_list {
    message* head;
    message* tail;
};

typedef struct Topic topic;
struct Topic {
    char* name;
    messages_list* messages;
    topic* next;
};

typedef struct Topics_list topics_list;
struct Topics_list {
    topic* head;
    topic* tail;
};

typedef struct Whiteboard whiteboard;
struct Whiteboard {
    topics_list* topics;
    users_list* users;
};

whiteboard* init_whiteboard() {
    whiteboard* w = (whiteboard*) malloc(sizeof(whiteboard));
    topics_list* tl = (topics_list*) malloc(sizeof(topics_list));
    tl->head= NULL;
    tl->tail= NULL;
    users_list* ul = (users_list*) malloc(sizeof(topics_list));
    ul->head= NULL;
    ul->tail= NULL;
    w->topics = tl;
    w->users = ul;
    return w;
}

void init_whiteboard_shm(void* mem) {
    whiteboard* w = (whiteboard*) mem;
    topics_list* tl = (topics_list*) malloc(sizeof(topics_list));
    tl->head= NULL;
    tl->tail= NULL;
    users_list* ul = (users_list*) malloc(sizeof(topics_list));
    ul->head= NULL;
    ul->tail= NULL;
    w->topics = tl;
    w->users = ul;
}

int destroy_whiteboard(whiteboard* w) {
    //TODO
    free(w);
    return 0;
}

void list_topics(whiteboard* w) {
    topics_list* tl = w->topics;
    topic* current = tl->head;
    while (current != NULL) {
        printf("%s\n", current->name);
        current = current->next;
    }
}

int add_topic(whiteboard* w, char* title) {
    //TODO:
    //check se il topic già esiste
    topics_list* tl = w->topics;
    topic* newtopic = (topic*) malloc(sizeof(topic));
    newtopic->name= title;
    newtopic->messages= NULL;
    newtopic->next= NULL;

    if (tl->head==NULL && tl->tail==NULL) {
        tl->head = newtopic;
        tl->tail = newtopic;
    }
    else {
        tl->tail->next= newtopic;
        tl->tail= newtopic;
    }

    return 0;
}

topic* get_topic(whiteboard* w, char* name) {
    topics_list* tl = w->topics;
    topic* current = tl->head;
    while(current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        else {
            current = current->next;
        }
    }
    return NULL;
}

void add_message_to_topic(topic* t, user* u, char* text) {
    messages_list* ml = t->messages;
    message* nm = (message*) malloc(sizeof(message));
    nm->text=text;
    nm->sender=u;
    nm->next=NULL;
    if(ml->head==NULL && ml->tail==NULL) {
        ml->head=nm;
        ml->tail=nm;
    }
    else {
        ml->tail->next=nm;
        ml->tail=nm;
    }
}

void list_messages_of_topic(topic* t) {
    messages_list* ml = t->messages;
    message* current = ml->head;
    while(current != NULL) {
        printf("%s\n", current->text);
        current = current->next;
    }
}

int add_user(whiteboard* w, char* username, char* password) {
    users_list* ul = w->users;
    user* nu = (user*) malloc(sizeof(user));
    nu->username = username;
    nu->password = password;
    nu->next = NULL;
    if (ul->head == NULL && ul->tail == NULL) {
        ul->head = nu;
        ul->tail = nu;
    }
    else {
        ul->tail->next = nu;
        ul->tail = nu;
    }
}
