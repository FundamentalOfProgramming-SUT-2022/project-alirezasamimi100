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
    getstr(&ptr, path, 0);
    while(*ptr != '-') ++ptr;
    if(!pipeString) {
        ptr += 6;
        getstr(&ptr, str, 0);
        pipeString = str;
    }
    int line, pos, offset;
    while(*ptr != '-') ++ptr;
    sscanf(ptr, "--pos %d:%d%n", &line, &pos, &offset);
    ptr += offset;
    *sptr = ptr;
    if(checkfile(path + 1)) {
        return;
    }
    createbackup(path + 1);
    addtofile(path + 1, pipeString, getpos(path + 1, line, pos));

}

void cat(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    while(*ptr != '-') ++ptr;
    ptr += 7;
    getstr(&ptr, path, 0);
    *sptr = ptr;
    if(checkfile(path + 1)) {
        return;
    }
    copyfile(path + 1, OUTPUT, 0, -1);
}

void removestr(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    int t, size, line, pos, left, right;
    while(*ptr != '-') ++ptr;
    ptr += 7;
    getstr(&ptr, path, 0);
    while(*ptr != '-') ++ptr;
    sscanf(ptr, "--pos %d:%d%n", &line, &pos, &t);
    ptr += t;
    while(*ptr != '-') ++ptr;
    sscanf(ptr, "-size %d%n", &size, &t);
    ptr += t;
    while(*ptr != '-') ++ptr;
    *sptr = ptr + 2;
    if(checkfile(path + 1)) {
        return;
    }
    if(*(ptr + 1) == 'f') {
        left = getpos(path + 1, line, pos);
        right = left + size;
    } else {
        right = getpos(path + 1, line, pos);
        left = right - size;
    }
    createbackup(path + 1);
    removefromfile(path + 1, left, right);
}

void undo(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    while(*ptr != '-') ++ptr;
    ptr += 7;
    getstr(&ptr, path, 0);
    *sptr = ptr;
    if(checkfile(path + 1)) {
        return;
    }
    restorebackup(path + 1);
}

void copy(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    int t, size, line, pos, left, right;
    while(*ptr != '-') ++ptr;
    ptr += 7;
    getstr(&ptr, path, 0);
    while(*ptr != '-') ++ptr;
    sscanf(ptr, "--pos %d:%d%n", &line, &pos, &t);
    ptr += t;
    while(*ptr != '-') ++ptr;
    sscanf(ptr, "-size %d%n", &size, &t);
    ptr += t;
    while(*ptr != '-') ++ptr;
    *sptr = ptr + 2;
    if(checkfile(path + 1)) {
        return;
    }
    if(*(ptr + 1) == 'f') {
        left = getpos(path + 1, line, pos);
        right = left + size;
    } else {
        right = getpos(path + 1, line, pos);
        left = right - size;
    }
    clearfile(CLIPBOARD);
    copyfile(path + 1, CLIPBOARD, left, right);
}

void cut(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    int t, size, line, pos, left, right;
    while(*ptr != '-') ++ptr;
    ptr += 7;
    getstr(&ptr, path, 0);
    while(*ptr != '-') ++ptr;
    sscanf(ptr, "--pos %d:%d%n", &line, &pos, &t);
    ptr += t;
    while(*ptr != '-') ++ptr;
    sscanf(ptr, "-size %d%n", &size, &t);
    ptr += t;
    while(*ptr != '-') ++ptr;
    *sptr = ptr + 2;
    if(checkfile(path + 1)) {
        return;
    }
    if(*(ptr + 1) == 'f') {
        left = getpos(path + 1, line, pos);
        right = left + size;
    } else {
        right = getpos(path + 1, line, pos);
        left = right - size;
    }
    clearfile(CLIPBOARD);
    copyfile(path + 1, CLIPBOARD, left, right);
    createbackup(path + 1);
    removefromfile(path + 1, left, right);
}

void paste(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    while(*ptr != '-') ++ptr;
    ptr += 7;
    getstr(&ptr, path, 0);
    int line, pos, offset;
    while(*ptr != '-') ++ptr;
    sscanf(ptr, "--pos %d:%d%n", &line, &pos, &offset);
    ptr += offset;
    *sptr = ptr;
    if(checkfile(path + 1)) {
        return;
    }
    createbackup(path + 1);
    faddtofile(path + 1, CLIPBOARD, getpos(path + 1, line, pos));
}

void find(char** sptr, char* pipeString) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE], str[MAX_STRING_SIZE];
    struct wildcard* wcards = NULL;
    int at = 1, bw = 0, cnt = 0, er = 0;
    while(*ptr != '-') ++ptr;
    if(!pipeString) {
        ptr += 6;
        wcards = getstr(&ptr, str, 1);
        pipeString = str;
    }
    while(*ptr != '-') ++ptr;
    ptr += 7;
    getstr(&ptr, path, 0);
    while(*ptr == ' ') ++ptr;
    int k;
    if(*(ptr + 1) == 'c') {
        cnt = 1;
        k = 6;
    } else if(*(ptr + 2) == 't') {
        sscanf(ptr, "-at %d%n", &at, &k);
        ++er;
    } else if(*(ptr + 1) == 'a') {
        ++er;
        at = 0;
        k = 4;
    } else if(*(ptr + 1) == 'b') {
        k = 7;
        bw = 1;
    }
    ptr += k;
    while(*ptr == ' ') ++ptr;
    if(*(ptr + 1) == 'c') {
        cnt = 1;
        k = 6;
    } else if(*(ptr + 2) == 't') {
        sscanf(ptr, "-at %d%n", &at, &k);
        ++er;
    } else if(*(ptr + 1) == 'a') {
        ++er;
        at = 0;
        k = 4;
    } else if(*(ptr + 1) == 'b') {
        k = 7;
        bw = 1;
    }
    ptr += k;
    *sptr = ptr;
    if(checkfile(path + 1)) {
        return;
    }
    if(er > 1) {
        puts("Invalid Arguments!");
        return;
    }
    k = 0;
    struct match* ans = findinfile(path + 1, pipeString, wcards);
    FILE* file = fopen(OUTPUT, "w");
    while(ans->left != -1) {
        k++;
        if(!cnt) {
            if(at == k) {
                if(bw) fprintf(file, "%d", ans->word);
                else fprintf(file, "%d", ans->left);
            } else if(!at) {
                if(k > 1) fprintf(file, ", ");
                if(bw) fprintf(file, "%d", ans->word);
                else fprintf(file, "%d %d", ans->left);
            }
        }
        struct match* tmp = ans->next;
        free(ans);
        ans = tmp;
    }
    if(cnt) fprintf(file, "%d", k);
    else if(!k || k < at) {
        fprintf(file, "-1");
    }
    fprintf(file, "\n");
    fclose(file);
}