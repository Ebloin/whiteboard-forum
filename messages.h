#define LIST_TOPICS 8
#define LIST_TOPIC_MESSAGES 1
#define ADD_MESSAGE_TO_TOPIC 2
#define ADD_TOPIC 3
#define MESSAGE_STATUS 4
#define QUIT 5
#define ADD_USER 6
#define LIST_USERS 7

#define MESSAGE_RECEIVED 1
#define MESSAGE_PUBLISHED 2

char *BANNER = "\033[1;34m __      __.__    .__  __        ___.                  __    \n/  \\    /  \\  |__ |__|/  |_  ____\\_ |__   ____   ____ |  | __\n\\   \\/\\/   /  |  \\|  \\   __\\/ __ \\| __ \\ /  _ \\ /  _ \\|  |/ /\n \\        /|   Y  \\  ||  | \\  ___/| \\_\\ (  <_> |  <_> )    < \n  \\__/\\  / |___|  /__||__|  \\___  >___  /\\____/ \\____/|__|_ \\\n       \\/       \\/              \\/    \\/                   \\/\n\nPress enter to start\n\n\033[0m";
char *COMMANDS_LIST = "\033[1;32m\nCommands:\n- create topic\n- list topics \n- delete topic (only if owner)\n- add message\n- reply message\n- exit\n\nENTER COMMAND: \033[0m";
char *COMMANDS_LIST_ADM = "\033[1;32m\nCommands:\n- create topic\n- list topics \n- delete topic (only if owner)\n- add message\n- reply message\n- add user\n- remove user\n- show users\n- exit\n\nENTER COMMAND: \033[0m";
char *UNKNOWN = "Command not recognized\n\0";
char *EXIT = "Closing connection\n\0";
char *ACK = "\0";
char *CMD_BAD = "Command badly formed\n\0";
char *UNAUTH = "You are not authorized to use this command\n\0";


//addtopic
char *ADDT_COMMANDS = "Insert the name of the topic: \0";
char *ADDT_SUCCESS = "Topic correctly created\n\0";
char *ADDT_ERROR = "Error in topic creation\n\0";

//delete topic
char * DELTOP_CODE = "Insert the code of the topic you want to delete: \0";
char * DELTOP_SUCC = "Topic correctly deleted\n\0";
char * DELTOP_FAIL_NOPROP = "Deletion failed, you are not the topic's owner\n\0";
char * DELTOP_FAIL_NOEXT= "Deletion failed, topic doesn't exists\n\0";


//addmessage
char *ADDM_SELECT_TOPIC = "Insert the code of the topic where you want to append the message: \0";
char *ADDM_MESSAGE_TEXT = "Insert the message: \0";
char *ADDM_MESSAGE_POSTED = "Message correctly added\n\0";

//List messsages
char *LTOP_SELECT_TOPIC = "Insert the code of the topic from where you want to read messages:\0";

//Manage users
char* ADDU_INSUSR = "Insert username to add: \0";
char* ADDU_INSPWD = "Insert password for the new user: \0";
char* ADDU_SUCC = "User correctly added\n\0";

//Login
char* LOGIN_REQUSR = "Insert USERNAME: \0";
char* LOGIN_REQPASS = "Insert PASSWORD: \0";
char* LOGIN_SUCC = "Logged in\n\0";
char* LOGIN_FAILED = "Login failed, username o password not correct!\n\0";

//Reply
char* REPLY_SELMES = "Insert the ID of the message you want to reply: \0";
char* REPLY_TEXT = "Insert the text of the reply: \0";
char* REPLY_ERRNOEXT = "Error, the inserted message ID doesn't exists\n\0";
char* REPLY_SUCC = "Reply successfully posted\n\0";

/*
[0;31m	Red
[1;31m	Bold Red
[0;32m	Green
[1;32m	Bold Green
[0;33m	Yellow
[01;33m	Bold Yellow
[0;34m	Blue
[1;34m	Bold Blue
[0;35m	Magenta
[1;35m	Bold Magenta
[0;36m	Cyan
[1;36m	Bold Cyan
[0m	Reset
*/