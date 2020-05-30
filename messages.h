//GENERAL
char *BANNER = "\033[1;34m __      __.__    .__  __        ___.                  __    \n/  \\    /  \\  |__ |__|/  |_  ____\\_ |__   ____   ____ |  | __\n\\   \\/\\/   /  |  \\|  \\   __\\/ __ \\| __ \\ /  _ \\ /  _ \\|  |/ /\n \\        /|   Y  \\  ||  | \\  ___/| \\_\\ (  <_> |  <_> )    < \n  \\__/\\  / |___|  /__||__|  \\___  >___  /\\____/ \\____/|__|_ \\\n       \\/       \\/              \\/    \\/                   \\/\n\nPress enter to start\n\n\033[0m";
char *COMMANDS_LIST = "\033[1;32m\nCommands:\n- create topic\n- list topics availables\n- list topics subscriptions\n- subscribe to topic\n - delete topic (only if owner)\n- append message\n- reply message\n- list messages\n- status message\n- exit\n\nENTER COMMAND: \033[0m";
char *COMMANDS_LIST_ADM = "\033[1;32m\nCommands:\n- create topic\n- list topics availables\n- subscribe to topic\n- delete topic (only if owner)\n- append message\n- reply message\n- list messages\n- status message\n- add user\n- remove user\n- show users\n- delete user\n- exit\n\nENTER COMMAND: \033[0m";
char *UNKNOWN = "Command not recognized\n\0";
char *EXIT = "Closing connection\n\0";
char *ACK = "\0";
char *CMD_BAD = "Command badly formed\n\0";
char *UNAUTH = "You are not authorized to use this command\n\0";


//addtopic
char *ADDT_COMMANDS = "Insert the name of the topic: \0";
char *ADDT_SUCCESS = "Topic correctly created\n\0";
char *ADDT_ERROR = "Error in topic creation, max num of topics reached\n\0";
char *ADDT_ERROR_NULL = "Error in topic creation, topic name cannot be null\n\0";
char *ADDT_ERROR_TOOLONG = "Error in topic creation, Topic name cannot be larger than 32 characters\n\0";


//delete topic
char * DELTOP_CODE = "Insert the code of the topic you want to delete: \0";
char * DELTOP_SUCC = "Topic correctly deleted\n\0";
char * DELTOP_FAIL_NOPROP = "Deletion failed, you are not the topic's owner\n\0";
char * DELTOP_FAIL_NOEXT= "Deletion failed, topic doesn't exists\n\0";


//addmessage
char *ADDM_SELECT_TOPIC = "Insert the code of the topic where you want to append the message: \0";
char *ADDM_MESSAGE_TEXT = "Insert the message: \0";
char *ADDM_MESSAGE_POSTED = "Message correctly added\n\0";
char *ADDM_ERROR_NOSUBSC = "Error, you can't write in a topic if you are not subscribed\n\0";
char *ADDM_ERROR_NOEXT = "Error, selected topic doesn't exists\n\0";
char *ADDM_ERROR_TEXT = "Message text cannot be void or larger than 1023 characters\n\0";


//List messsages
char *LTOP_SELECT_TOPIC = "Insert the code of the topic from where you want to read messages:\0";

//Manage users
char* ADDU_INSUSR = "Insert username to add: \0";
char* ADDU_INSPWD = "Insert password for the new user: \0";
char* ADDU_SUCC = "User correctly added\n\0";
char* ADDU_ERR_USR = "Username cannot be null or larger than 30 characters\n\0";
char* ADDU_ERR_PAS = "Password cannot be null or larger than 30 characters\n\0";
char* ADDU_ERR_LIM = "Max num users limit reached\n\0";
char* DELUSR_INS_USR = "Insert the code of the user you want to delete\n\0";
char* DELUSR_OK = "User deleted successfully\n\0";
char* DELUSR_ERR_ADM = "User admin cannot be deleted\n\0";
char* DELUSR_ERR_NOEXT = "Selected user doesn't exists\n\0";


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

//Topic subscription
char* TOPSUB_NAME = "Insert the code of the topic you want to be subscribed: \0";
char* TOPSUB_SUCC = "Topic subscription successful\n\0";
char* TOPSUB_FAIL_NOEXT = "Topic subscription failed, selected topic doesn't exists\n\0";
char* TOPSUB_FAIL_ALREADY = "Topic subscription failed, you are already subscribed to this topic!\n\0";

//Status message
char* STAT_MSGCODE = "Insert the code of the message you want to see the status: \0";
char* STAT_ERR_NOSENDER = "Only the sender can see the status of a message!\n\0";

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