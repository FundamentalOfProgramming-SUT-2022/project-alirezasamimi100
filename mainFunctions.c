#include "mainHeaders.h"

void createfile(char** sptr) {
    char* ptr = *sptr;
    const char end[] = {' ', '\"'};
    int mode = 0;
    if(*(ptr + 7) == '\"') ptr++, mode = 1;
    ptr += 8;
    char path[FILE_PATH_SIZE], *cur = path;
    while(*ptr != end[mode] && *ptr != '\0') {
        *cur = *ptr;
        if(*cur == '/') {
            *cur = '\0';
            mkdir(path, 0755);
            *cur = '/';
        }
        ++cur;
        ++ptr;
    }
    FILE* file = fopen(path, "r");
    if(!file) {
        file = fopen(path, "w");
    } else {
        puts("File Already Exists!");
    }
    fclose(file);
    *sptr = ptr;
}