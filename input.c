#include "mainHeaders.h"


int executeCommands(char* command, char* path) {
    char commandstr[MAX_COMMAND_SIZE], func[MAX_COMMAND_SIZE], output[MAX_STRING_SIZE];
    if(!command) {
        fgets(commandstr, MAX_COMMAND_SIZE, stdin);
        command = commandstr;
    }
    char *ptr = command, *pipe = NULL;
    while(1) {
        clearfile(OUTPUT);
        sscanf(ptr, "%s", func);
        ptr += strlen(func) + 1;
        if(!strcmp(func, "exit")) {
            return 1;
        } else if(!strcmp(func, "createfile")) {
            createfile(&ptr);
        } else if(!strcmp(func, "insertstr")) {
            insertstr(&ptr, pipe);
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
            find(&ptr, pipe);
        } else if(!strcmp(func, "replace")) {
            return 10 + replace(&ptr);
        } else if(!strcmp(func, "grep")) {
            grep(&ptr, pipe);
        } else if(!strcmp(func, "tree")) {
            tree(&ptr);
        } else if(!strcmp(func, "auto-indent")) {
            autoindent(&ptr, 0);
        } else if(!strcmp(func, "compare")) {
            compare(&ptr);
        } else if(!strcmp(func, "open")) {
            char tmp[FILE_PATH_SIZE];
            strcpy(tmp, path);
            getinstr(&ptr, path, 0);
            if(checkfile(path + 1)) {
                strcpy(path, tmp);
                return 0;
            }
            return 2;
        } else if(!strcmp(func, "save")) {
            if(*path != '/') {
                reterr("No Filename Specified!");
            } else return 3;
        } else if(!strcmp(func, "saveas")) {
            char tmp[FILE_PATH_SIZE];
            strcpy(tmp, path);
            getinstr(&ptr, path, 0);
            if(checkfile(path + 1)) {
                strcpy(path, tmp);
                return 0;
            }
            return 3;
        } else {
            reterr("Invalid Command!");
        }
        while(*ptr == ' ') ++ptr;
        if(*ptr == '=' && *(ptr + 1) == 'D') {
            ptr += 3;
            filetostr(OUTPUT, output);
            pipe = output;
        }else break;
    }
    return 0;
}

struct wildcard* getinstr(char** sptr, char* str, int find) {
    struct wildcard *ans = NULL, *now = NULL;
    if(find) {
        ans = malloc(sizeof(struct wildcard));
        ans->pos = -1;
        now = ans;
    }
    const char end[] = {' ', '\"'};
    char* ptr = *sptr;
    while(*ptr == ' ') ++ptr;
    int mode = 0, bs = 0;
    if(*ptr == '\"') mode = 1, ++ptr;
    int i = 0;
    while((bs || *ptr != end[mode]) && *ptr != '\n' && *ptr != '\0') {
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
    while(*ptr == ' ') ++ptr;
    *sptr = ptr;
    return ans;
}
