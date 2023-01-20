#include "mainHeaders.h"


void executeCommands() {
    char command[MAX_COMMAND_SIZE], func[MAX_COMMAND_SIZE], output[MAX_STRING_SIZE];
    output[0] = '\0';
    fgets(command, MAX_COMMAND_SIZE, stdin);
    char *ptr = command;
    sscanf(ptr, "%s", func);
    ptr += strlen(func) + 1;
    if(!strcmp(func, "createfile")) {
        createfile(&ptr);
    } else if(!strcmp(func, "insertstr")) {
        insertstr(&ptr, NULL);
    } else if(!strcmp(func, "cat")) {
        cat(&ptr, output);
        printf("%s\n", output);
    } else if(!strcmp(func, "removestr")) {
        removestr(&ptr);
    } else {
        puts("Invalid Command!");
    }
}

void getstr(char** sptr, char* str) {
    const char end[] = {' ', '\"'};
    char* ptr = *sptr;
    int mode = 0, bs = 0;
    if(*ptr == '\"') mode = 1, ++ptr;
    while(*ptr != end[mode] && *ptr != '\n' && *ptr != '\0') {
        if(bs) {
            bs = 0;
            if(*ptr == 'n') {
                *str = '\n';
            }else {
                *str = *ptr;
            }
            ++str;
        } else {
            if(*ptr == '\\') {
                bs = 1;
            } else {
                *str = *ptr;
                ++str;
            }
        }
        ++ptr;
    }
    *str = '\0';
    *sptr = ptr;
}
