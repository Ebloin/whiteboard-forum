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

//GENERAL
char *BANNER = BOLD_BLUE" __      __.__    .__  __        ___.                  __    \n/  \\    /  \\  |__ |__|/  |_  ____\\_ |__   ____   ____ |  | __\n\\   \\/\\/   /  |  \\|  \\   __\\/ __ \\| __ \\ /  _ \\ /  _ \\|  |/ /\n \\        /|   Y  \\  ||  | \\  ___/| \\_\\ (  <_> |  <_> )    < \n  \\__/\\  / |___|  /__||__|  \\___  >___  /\\____/ \\____/|__|_ \\\n       \\/       \\/              \\/    \\/                   \\/\n\nPress enter to start\n\n"RESET;

char* BANNER2 = BACK_BLUE
"          / / /\\ \\ \\ |__ (_) |_ ___  / __\\ ___   ___ | | __          \n"
"          \\ \\/  \\/ / '_ \\| | __/ _ \\/__\\/// _ \\ / _ \\| |/ /          \n"
"           \\  /\\  /| | | | | ||  __/ \\/  \\ (_) | (_) |   <           \n"
"            \\/  \\/ |_| |_|_|\\__\\___\\_____/\\___/ \\___/|_|\\_\\          "RESET BOLD_BLUE
"\n\nPress ENTER to start"RESET;

char* BANNER3 = BACK_BLUE
"\n                                                                                  \n"
"     ██     ██ ██   ██ ██ ████████ ███████ ██████   ██████   ██████  ██   ██      \n"
"     ██     ██ ██   ██ ██    ██    ██      ██   ██ ██    ██ ██    ██ ██  ██       \n"
"     ██  █  ██ ███████ ██    ██    █████   ██████  ██    ██ ██    ██ █████        \n"
"     ██ ███ ██ ██   ██ ██    ██    ██      ██   ██ ██    ██ ██    ██ ██  ██       \n"
"      ███ ███  ██   ██ ██    ██    ███████ ██████   ██████   ██████  ██   ██      \n"
"                                                                                  \n\n"
RESET BOLD_BLUE "Press ENTER to start! " RESET;

char *COMMANDS_LIST = BACK_GREEN 
"\nCOMMANDS:                                  " RESET BOLD_GREEN "\n\
- create topic\n\
- delete topic (only if owner)\n\
- list available topics\n\
- list subscripted topics\n\
- subscribe to topic\n\
- create thread\n- reply message\n\
- reply message\n\
- list messages\n- status message\n\
- message status\n\
- exit\n\n"
RESET"CMD > ";

char *COMMANDS_LIST_ADM = BACK_GREEN 
"\nCOMMANDS:                                  " RESET BOLD_GREEN "\n\
- create topic\n\
- delete topic (only if owner)\n\
- list available topics\n\
- subscribe to topic\n\
- create thread\n- reply message\n\
- reply message\n\
- list messages\n- status message\n\
- message status\n\
- exit\n" RESET BACK_RED
"ADMIN COMMANDS:                            \n" RESET BOLD_RED
"- add user\n\
- show users\n\
- delete user\n\n"
RESET "CMD > ";

char *UNKNOWN = BOLD_RED"ERROR: Command not recognized"RESET"\n\0";
char *EXIT = "Closing connection\n\0";
char *ACK = "\0";
char *UNAUTH = BOLD_RED"You are not authorized to use this command"RESET"\n\0";


//addtopic
char *ADDT_COMMANDS = BOLD_GREEN"Insert the name of the topic: "RESET"\0";
char *ADDT_SUCCESS = BOLD_BLUE"Topic correctly created"RESET"\n\0";
char *ADDT_ERROR = BOLD_RED"ERROR: Max num of topics reached"RESET"\n\0";
char *ADDT_ERROR_NULL = BOLD_RED"ERROR: topic name cannot be null"RESET"\n\0";
char *ADDT_ERROR_TOOLONG = BOLD_RED"ERROR: Topic name cannot be larger than 32 characters"RESET"\n\0";


//delete topic
char * DELTOP_CODE = BOLD_GREEN"Insert the code of the topic you want to delete: "RESET"\0";
char * DELTOP_SUCC = BOLD_BLUE"Topic correctly deleted"RESET"\n\0";
char * DELTOP_FAIL_NOPROP = BOLD_RED"ERROR: you are not the topic's owner"RESET"\n\0";
char * DELTOP_FAIL_NOEXT= BOLD_RED"ERROR: selected topic doesn't exists"RESET"\n\0";


//addmessage
char *ADDM_SELECT_TOPIC = BOLD_GREEN"Insert the code of the topic where you want to append the message: "RESET"\0";
char *ADDM_MESSAGE_TEXT = BOLD_GREEN"Insert the message: "RESET"\0";
char *ADDM_MESSAGE_POSTED = BOLD_BLUE"Message correctly added"RESET"\n\0";
char *ADDM_ERROR_NOSUBSC = BOLD_RED"ERROR: you can't write in a topic if you are not subscribed"RESET"\n\0";
char *ADDM_ERROR_NOEXT = BOLD_RED"ERROR: selected topic doesn't exists"RESET"\n\0";
char *ADDM_ERROR_TEXT = BOLD_RED"ERROR: Message text cannot be void or larger than 60 characters"RESET"\n\0";
char* ADDM_ERROR_LIMIT = BOLD_RED"ERROR: Messages limit reached in the topic"RESET"\n\0";

//List messsages
char *LTOP_SELECT_TOPIC = BOLD_GREEN"Insert the code of the topic from where you want to read messages: "RESET"\0";

//Manage users
char* ADDU_INSUSR = BOLD_GREEN"Insert username to add: "RESET"\0";
char* ADDU_INSPWD = BOLD_GREEN"Insert password for the new user: "RESET"\0";
char* ADDU_SUCC = BOLD_BLUE"User correctly added"RESET"\n\0";
char* ADDU_ERR_USR = BOLD_RED"ERROR: Username cannot be null or larger than 30 characters"RESET"\n\0";
char* ADDU_ERR_PAS = BOLD_RED"ERROR: Password cannot be null or larger than 30 characters"RESET"\n\0";
char* ADDU_ERR_LIM = BOLD_RED"ERROR: Max num users limit reached"RESET"\n\0";
char* ADDU_ERR_EXT = BOLD_RED"ERROR: This username is already taken"RESET"\n\0";
char* DELUSR_INS_USR = BOLD_GREEN"Insert the code of the user you want to delete: "RESET"\0";
char* DELUSR_OK = BOLD_BLUE"User deleted successfully"RESET"\n\0";
char* DELUSR_ERR_ADM = BOLD_RED"ERROR: User admin cannot be deleted"RESET"\n\0";
char* DELUSR_ERR_NOEXT = BOLD_RED"ERROR: Selected user doesn't exists"RESET"\n\0";


//Login
char* LOGIN_REQUSR = BOLD_GREEN"Insert USERNAME: "RESET"\0";
char* LOGIN_REQPASS = BOLD_GREEN"Insert PASSWORD: "RESET"\0";
char* LOGIN_SUCC = BOLD_BLUE"Logged in"RESET"\n\0";
char* LOGIN_FAILED = BOLD_RED"Login failed, username o password not correct!"RESET"\n\0";

//Reply
char* REPLY_SELMES = BOLD_GREEN"Insert the ID of the message you want to reply: "RESET"\0";
char* REPLY_TEXT = BOLD_GREEN"Insert the text of the reply: "RESET"\0";
char* REPLY_ERRNOEXT = BOLD_RED"ERROR: the inserted message ID doesn't exists"RESET"\n\0";
char* REPLY_SUCC = BOLD_BLUE"Reply successfully posted"RESET"\n\0";

//Topic subscription
char* TOPSUB_NAME = BOLD_GREEN"Insert the code of the topic you want to be subscribed: "RESET"\0";
char* TOPSUB_SUCC = BOLD_BLUE"Topic subscription successful"RESET"\n\0";
char* TOPSUB_FAIL_NOEXT = BOLD_RED"ERROR: selected topic doesn't exists"RESET"\n\0";
char* TOPSUB_FAIL_ALREADY = BOLD_RED"ERROR: you are already subscribed to this topic!"RESET"\n\0";

//Status message
char* STAT_MSGCODE = BOLD_GREEN"Insert the code of the message you want to see the status: "RESET"\0";
char* STAT_ERR_NOSENDER = BOLD_RED"ERROR: Only the sender can see the status of a message!"RESET"\n\0";
