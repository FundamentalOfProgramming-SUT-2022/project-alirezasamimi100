#include "mainHeaders.h"


void executeCommands() {
    char command[MAX_COMMAND_SIZE], func[MAX_COMMAND_SIZE];
    fgets(command, MAX_COMMAND_SIZE, stdin);
    char *ptr = command;
    sscanf(ptr, "%s", func);
    ptr += strlen(func) + 1;
    if(!strcmp(func, "createfile")) {
        createfile(&ptr);
    }
}