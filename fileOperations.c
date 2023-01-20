#include "mainHeaders.h"

void copyfile(char srcp[], char dstp[], int left, int right) {
    FILE* src = fopen(srcp, "r");
    FILE* dst = fopen(dstp, "a");
    char c;
    int i = 0;
    while((c = fgetc(src)) != EOF) {
        if(i >= left && (i < right || right == -1)) {
            fputc(c, dst);
        }
        ++i;
    }
    fclose(src);
    fclose(dst);
}

void clearfile(char path[]) {
    fclose(fopen(path, "w"));
}

void addtofile(char path[], char* str, int pos) {
    clearfile(TEMP_PATH);
    copyfile(path, TEMP_PATH, 0, pos);
    FILE* tmpf = fopen(TEMP_PATH, "a");
    fprintf(tmpf, "%s", str);
    fclose(tmpf);
    copyfile(path, TEMP_PATH, pos, -1);
    clearfile(path);
    copyfile(TEMP_PATH, path, 0, -1);
}

int getpos(char path[], int line, int pos) {
    FILE* file = fopen(path, "r");
    if(!file) {
        return 0;
    }
    int ln = 1, ls = 0;
    char c;
    for(int i = 0; ; i++) {
        if(ln == line && ls == pos) {
            return i;
        }
        c = fgetc(file);
        if(c == EOF) return i;
        ls++;
        if(c == '\n') ++ln, ls = 0;
    }
}

void removefromfile(char path[], int left, int right) {
    clearfile(TEMP_PATH);
    copyfile(path, TEMP_PATH, 0, left);
    copyfile(path, TEMP_PATH, right, -1);
    clearfile(path);
    copyfile(TEMP_PATH, path, 0, -1);
}