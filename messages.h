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
char *COMMANDS_LIST = "\033[1;32m\nCommands:\n- create topic\n- list topics \n- delete topic (only if owner)\n- exit\n\nENTER COMMAND: \033[0m";
char *COMMANDS_LIST_ADM = "\033[1;32m\nCommands:\n- create topic\n- list topics \n- delete topic (only if owner)\n- add user\n- remove user\n- show users\n- exit\n\nENTER COMMAND: \033[0m";
char *UNKNOWN = "Comando non riconosciuto\n\0";
char *EXIT = "Closing connection\n\0";
char *ACK = "\0";
char *CMD_BAD = "Command badly formed\n\0";
char *UNAUTH = "You are not authorized to use this command\n\0";


//addtopic
char *ADDT_COMMANDS = "Inserisci il nome da assegnare al topic: \0";
char *ADDT_SUCCESS = "Topic creato correttamente\n\0";
char *ADDT_ERROR = "Error in topic creation\n\0";

//delete topic
char * DELTOP_CODE = "Inserisci il codice del topic che vuoi eliminare: \0";
char * DELTOP_SUCC = "Topic eliminato correttamente\n\0";
char * DELTOP_FAIL_NOPROP = "Eliminazione fallita, non sei il proprietario del topic\n\0";
char * DELTOP_FAIL_NOEXT= "Eliminazione fallita, il topic inserito non esiste\n\0";

//TODO
char *NOT_IMPLEMENTED = "Il comando corrente non è ancora stato implementato, sorry <3\n\0";

//addmessage
char *ADDM_SELECT_TOPIC = "Inserire l'indice relativo al topic su cui aggiungere il messaggio: \0";
char *ADDM_MESSAGE_TEXT = "Inserisci il testo del messaggio da aggiungere: \0";
char *ADDM_MESSAGE_POSTED = "Messaggio aggiunto correttamente\n\0";

//List messsages
char *LTOP_SELECT_TOPIC = "Inserire l'indice del topic di cui visualizzare i messaggi:\0";

//Manage users
char* ADDU_INSUSR = "Inserisci l'username da aggiungere: \0";
char* ADDU_INSPWD = "Inserisci la password da aggiungere: \0";
char* ADDU_SUCC = "Utente creato correttamente\n\0";

//Login

char* LOGIN_REQUSR = "Inserire USERNAME: \0";
char* LOGIN_REQPASS = "Inserire PASSWORD: \0";
char* LOGIN_SUCC = "Login effettuato correttamente\n\0";
char* LOGIN_FAILED = "Login fallito, username o password non corrette\n\0";


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