#define LIST_TOPICS 0
#define LIST_TOPIC_MESSAGES 1
#define ADD_MESSAGE_TO_TOPIC 2
#define ADD_TOPIC 3
#define MESSAGE_STATUS 4
#define QUIT 5

char *BANNER = "\033[1;34m __      __.__    .__  __        ___.                  __    \n/  \\    /  \\  |__ |__|/  |_  ____\\_ |__   ____   ____ |  | __\n\\   \\/\\/   /  |  \\|  \\   __\\/ __ \\| __ \\ /  _ \\ /  _ \\|  |/ /\n \\        /|   Y  \\  ||  | \\  ___/| \\_\\ (  <_> |  <_> )    < \n  \\__/\\  / |___|  /__||__|  \\___  >___  /\\____/ \\____/|__|_ \\\n       \\/       \\/              \\/    \\/                   \\/\n\n\033[0m";
char *COMMANDS_LIST = "\033[1;32m\nCommands:\n0)List topics\n1)List messages in a topic\n2)Add Message to topic\n3)Create topic\n4)Show the status of a message you sent\n5)Quit\n\nENTER COMMAND: \033[0m";
char *UNKNOWN = "Comando non riconosciuto\n";

char *ACK = "";

//addtopic
char *ADDT_COMMANDS = "Inserisci il nome da assegnare al topic: ";
char *ADDT_SUCCESS = "Topic creato correttamente\n";

//TODO
char *NOT_IMPLEMENTED = "Il comando corrente non è ancora stato implementato, sorry <3\n";

//addmessage
char *ADDM_SELECT_TOPIC = "Inserire l'indice relativo al topic su cui aggiungere il messaggio: ";
char *ADDM_MESSAGE_TEXT = "Inserisci il testo del messaggio da aggiungere: ";
char *ADDM_MESSAGE_POSTED = "Messaggio aggiunto correttamente\n";

//List messsages
char *LTOP_SELECT_TOPIC = "Inserire l'ndice del topic di cui visualizzare i messaggi:";
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