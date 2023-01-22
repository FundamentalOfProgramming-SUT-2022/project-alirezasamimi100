#include "mainHeaders.h"


int executeCommands() {
    char command[MAX_COMMAND_SIZE], func[MAX_COMMAND_SIZE], output[MAX_STRING_SIZE];
    clearfile(OUTPUT);
    fgets(command, MAX_COMMAND_SIZE, stdin);
    char *ptr = command;
    sscanf(ptr, "%s", func);
    ptr += strlen(func) + 1;
    if(!strcmp(func, "exit")) {
        return 0;
    } else if(!strcmp(func, "createfile")) {
        createfile(&ptr);
    } else if(!strcmp(func, "insertstr")) {
        insertstr(&ptr, NULL);
    } else if(!strcmp(func, "cat")) {
        cat(&ptr);
    } else if(!strcmp(func, "removestr")) {
        removestr(&ptr);
    } else if(!strcmp(func, "undo")) {
        undo(&ptr);
    } else if(!strcmp(func, "copy")) {
        copy(&ptr);
    } else if(!strcmp(func, "cut")) {
        cut(&ptr);
    } else if(!strcmp(func, "paste")) {
        paste(&ptr);
    } else if(!strcmp(func, "find")) {
        find(&ptr, NULL);
    } else {
        puts("Invalid Command!");
    }
    printfile(OUTPUT);
    return 1;
}

struct wildcard* getstr(char** sptr, char* str, int find) {
    struct wildcard *ans = NULL, *now = NULL;
    if(find) {
        ans = malloc(sizeof(struct wildcard));
        ans->pos = -1;
        now = ans;
    }
    const char end[] = {' ', '\"'};
    char* ptr = *sptr;
    int mode = 0, bs = 0;
    if(*ptr == '\"') mode = 1, ++ptr;
    int i = 0;
    while(*ptr != end[mode] && *ptr != '\n' && *ptr != '\0') {
        if(bs) {
            bs = 0;
            if(*ptr == 'n') {
                *str = '\n';
            }else {
                *str = *ptr;
            }
            ++str;
            ++i;
        } else {
            if(*ptr == '\\') {
                bs = 1;
            } else {
                if(find && *ptr == '*') {
                    now->pos = i;
                    now->next = malloc(sizeof(struct wildcard));
                    now = now->next;
                    now->pos = -1;
                }
                *str = *ptr;
                ++str;
                ++i;
            }
        }
        ++ptr;
    }
    *str = '\0';
    if(*ptr == end[mode]) ++ptr;
    *sptr = ptr;
    return ans;
}
