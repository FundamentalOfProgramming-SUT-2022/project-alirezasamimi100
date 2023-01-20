#include "mainHeaders.h"

void createfile(char** sptr) {
    char* ptr = *sptr;
    const char end[] = {' ', '\"'};
    int mode = 0;
    while(*ptr != '-') ++ptr;
    if(*(ptr + 7) == '\"') ptr++, mode = 1;
    ptr += 8;
    char path[FILE_PATH_SIZE], *cur = path;
    while(*ptr != end[mode] && *ptr != '\n' && *ptr != '\0') {
        *cur = *ptr;
        if(*cur == '/') {
            *cur = '\0';
            mkdir(path, 0755);
            *cur = '/';
        }
        ++cur;
        ++ptr;
    }
    *cur = '\0';
    FILE* file = fopen(path, "r");
    if(!file) {
        file = fopen(path, "w");
    } else {
        puts("File Already Exists!");
    }
    fclose(file);
    *sptr = ptr;
}

void insertstr(char** sptr, char* pipeString) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE], str[MAX_STRING_SIZE];
    while(*ptr != '-') ++ptr;
    ptr += 7;
    getstr(&ptr, path);
    while(*ptr != '-') ++ptr;
    if(!pipeString) {
        ptr += 6;
        getstr(&ptr, str);
        pipeString = str;
    }
    int line, pos, offset;
    while(*ptr != '-') ++ptr;
    sscanf(ptr, "--pos %d:%d%n", &line, &pos, &offset);
    ptr += offset;
    *sptr = ptr;
    addtofile(path + 1, pipeString, getpos(path + 1, line, pos));

}

void cat(char** sptr, char* output) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    while(*ptr != '-') ++ptr;
    ptr += 7;
    getstr(&ptr, path);
    *sptr = ptr;
    FILE* file = fopen(path + 1, "r");
    if(!file) {
        puts("File Not Found!");
    } else {
        char c;
        while((c = fgetc(file)) != EOF) {
            *output = c;
            ++output;
        }
        *output = '\0';
    }
}

void removestr(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    int t, size, line, pos, left, right;
    while(*ptr != '-') ++ptr;
    ptr += 7;
    getstr(&ptr, path);
    while(*ptr != '-') ++ptr;
    sscanf(ptr, "--pos %d:%d%n", &line, &pos, &t);
    ptr += t;
    while(*ptr != '-') ++ptr;
    sscanf(ptr, "-size %d%n", &size, &t);
    ptr += t;
    while(*ptr != '-') ++ptr;
    if(*(ptr + 1) == 'f') {
        left = getpos(path + 1, line, pos);
        right = left + size;
    } else {
        right = getpos(path + 1, line, pos);
        left = right - size;
    }
    *sptr = ptr + 2;
    removefromfile(path + 1, left, right);
}