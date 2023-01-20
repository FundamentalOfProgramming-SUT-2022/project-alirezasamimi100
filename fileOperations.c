#include "mainHeaders.h"

void copyfile(FILE* src, FILE* dst) {
    char c;
    fseek(src, 0, SEEK_SET);
    fseek(dst, 0, SEEK_SET);
    while((c = fgetc(src)) != EOF) {
        fputc(c, dst);
    }
}

void addtofile(char path[], char* str, int line, int pos) {
    FILE* file = fopen(path, "r+");
    if(!file) {
        puts("File Not Found!");
        return;
    }
    char buff[MAX_STRING_SIZE];
    FILE* tmpf = fopen(TEMP_PATH, "w+");
    for(int i = 0; i < line; i++) {
        fgets(buff, MAX_STRING_SIZE, file);
        fputs(buff, tmpf);
    }
    fgets(buff, MAX_STRING_SIZE, file);
    int size = strlen(buff);
    for(int i = 0; i < pos; i++) {
        if(!buff[i]) break;
        fputc(buff[i], tmpf);
    }
    fprintf(tmpf, "%s", str);
    for(int i = pos; i < size; i++) {
        fputc(buff[i], tmpf);
    }
    char c;
    while((c = fgetc(file)) != EOF) {
        fputc(c, tmpf);
    }
    freopen(path, "w", file);
    copyfile(tmpf, file);
    fclose(tmpf);
    fclose(file);
}