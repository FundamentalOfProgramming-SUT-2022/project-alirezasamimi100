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
    strtofile(str, TEMP_PATH);
    copyfile(path, TEMP_PATH, pos, -1);
    clearfile(path);
    copyfile(TEMP_PATH, path, 0, -1);
}

void faddtofile(char path[], char fstr[], int pos) {
    clearfile(TEMP_PATH);
    copyfile(path, TEMP_PATH, 0, pos);
    copyfile(fstr, TEMP_PATH, 0, -1);;
    copyfile(path, TEMP_PATH, pos, -1);
    clearfile(path);
    copyfile(TEMP_PATH, path, 0, -1);
}

int getpos(char path[], int line, int pos) {
    FILE* file = fopen(path, "r");
    if(!file) {
        return 0;
    }
    int ln = 0, ls = 0;
    char c;
    for(int i = 0; ; i++) {
        if(ln == line && ls == pos) {
            fclose(file);
            return i;
        }
        c = fgetc(file);
        if(c == EOF) {
            fclose(file);
            return i;
        }
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

void strtofile(char* str, char path[]) {
    FILE* file = fopen(path, "a");
    fputs(str, file);
    fclose(file);
}

int filesize(char path[]) {
    int size = 0;
    FILE* file = fopen(path, "r");
    while(fgetc(file) != EOF) {
        ++size;
    }
    return size;
}

void filetostr(char path[], char* str) {
    char c;
    FILE* file = fopen(path, "r");
    while((c = fgetc(file)) != EOF) {
        *str = c;
        ++str;
    }
    *str = '\0';
}

void hidefile(char path[], char hidden[]) {
    int k = strrchr(path, '/') - path + 1;
    strncpy(hidden, path, k);
    hidden[k] = '.';
    strcpy(hidden + k + 1, path + k);
}

int checkfile(char path[]) {
    int k = strrchr(path, '/') - path;
    path[k] = '\0';
    DIR* dir = opendir(path);
    path[k] = '/';
    if(!dir) {
        reterr("Directory Doesn't Exist!");
        return 1;
    } else {
        closedir(dir);
        FILE* file = fopen(path, "r");
        if(!file) {
            reterr("File Doesn't Exist!");
            return 1;
        } else {
            fclose(file);
        }
    }
    return 0;
}

void createbackup(char path[]) {
    char old[FILE_PATH_SIZE], new[FILE_PATH_SIZE];
    hidefile(path, old);
    strcpy(new, old);
    int size = strlen(old);
    old[size + 1] = '\0';
    new[size + 1] = '\0';
    for(int i = 8; i >= 0; i--) {
        old[size] = '0' + i;
        new[size] = '1' + i;
        FILE* file = fopen(old, "r");
        if(file) {
            fclose(file);
            rename(old, new);
        }
    }
    copyfile(path, old, 0, -1);
}

void restorebackup(char path[]) {
    char old[FILE_PATH_SIZE], new[FILE_PATH_SIZE];
    hidefile(path, old);
    strcpy(new, old);
    int size = strlen(old);
    old[size + 1] = '\0';
    new[size + 1] = '\0';
    new[size] = '0';
    FILE* file = fopen(new, "r");
    if(file) {
        fclose(file);
        rename(new, path);
    } else {
        reterr("No Actions To Undo!");
        return;
    }
    for(int i = 0; i <= 8; i++) {
        old[size] = '0' + i;
        new[size] = '1' + i;
        FILE* file = fopen(new, "r");
        if(file) {
            fclose(file);
            rename(new, old);
        } else {
            break;
        }
    }
}

void printfile(char path[]) {
    char c;
    FILE* file = fopen(path, "r");
    while((c = fgetc(file)) != EOF) {
        fputc(c, stdout);
    }
}

struct match* findinfile(char path[], char ptrn[], struct wildcard* wcards, int del) {
    struct match *ans = malloc(sizeof(struct match)), *now = ans;
    ans->left = -1;
    int nfa[MAX_STRING_SIZE], wild[MAX_STRING_SIZE], wfa[MAX_STRING_SIZE], sz = strlen(ptrn);
    memset(wild, 0, sizeof(wild));
    memset(nfa, -1, sizeof(nfa));
    while(wcards && wcards->pos != -1) {
        wild[wcards->pos] = 1;
        struct wildcard* tmp = wcards->next;
        if(del) free(wcards);
        wcards = tmp;
    }
    if(del) free(wcards);
    FILE* file = fopen(path, "r");
    char c = fgetc(file);
    int wn = 0, ls = 1, i = 0;
    for(; c != EOF; i++) {
        if(c != ' ' && c != '\n') {
            wn += ls;
            ls = 0;
        }else {
            ls = 1;
        }
        if(nfa[sz] != -1 && (!wild[sz - 1] || c == ' ' || c == '\n')) {
            now->left = nfa[sz];
            now->right = i;
            now->word = wfa[sz];
            now->next = malloc(sizeof(struct match));
            now = now->next;
            now->left = -1;
            memset(nfa, -1, sizeof(nfa));
        }
        nfa[0] = i;
        wfa[0] = wn;
        for(int j = 1; nfa[j] == -1 && wild[j - 1]; j++) {
            nfa[j] = i;
            wfa[j] = wn;
        }
        for(int j = sz; j; j--) {
            if(nfa[j] != -1 && wild[j - 1] && c != ' ' && c != '\n') continue;
            nfa[j] = -1;
            if(nfa[j - 1] != -1 && (c == ptrn[j - 1] || (wild[j - 1] && c != ' ' && c != '\n'))) nfa[j] = nfa[j - 1], wfa[j] = wfa[j - 1];
            if(nfa[j] != -1) {
                for(int k = j + 1; nfa[k] == -1 && wild[k - 1]; k++) {
                    nfa[k] = nfa[j];
                    wfa[k] = wfa[j];
                }
            }
        }
        c = fgetc(file);
    }
    if(nfa[sz] != -1) {
        now->left = nfa[sz];
        now->right = i;
        now->word = wfa[sz];
        now->next = malloc(sizeof(struct match));
        now = now->next;
        now->left = -1;
    }
    return ans;
}

void prtree(char path[], int depth, int pre) {
    DIR* dir = opendir(path);
    struct dirent *file;
    FILE* out = fopen(OUTPUT, "a");
    char *name = strrchr(path, '/');
    for(int i = 0; i < pre; i++) fputc('-', out);
    if(!name) {
        fprintf(out, "%s/\n", path);
    } else {
        fprintf(out, "%s/\n", name + 1);
    }
    if(!depth) {
        fclose(out);
        closedir(dir);
        return;
    }
    while(file = readdir(dir)) {
        if(file->d_name[0] == '.') continue;
        if(file->d_type == DT_REG) {
            for(int i = 0; i <= pre; i++) fputc('-', out);
            fprintf(out, "%s\n", file->d_name);
        } else if(file->d_type == DT_DIR) {
            char newpath[FILE_PATH_SIZE];
            strcpy(newpath, path);
            strcpy(newpath + strlen(newpath), "/");
            strcpy(newpath + strlen(newpath), file->d_name);
            fclose(out);
            prtree(newpath, depth - 1, pre + 1);
            out = fopen(OUTPUT, "a");
        }
    }
    fclose(out);
    closedir(dir);
}

int getlinecount(char path[]) {
    FILE* file = fopen(path, "r");
    int ans = 1;
    char c;
    while((c = fgetc(file)) != EOF) {
        if(c == '\n') ++ans;
    }
    fclose(file);
    return ans;
}

int getcharcount(char path[], int line) {
    FILE* file = fopen(path, "r");
    int ans = 0;
    char str[MAX_STRING_SIZE], c;
    for(int i = 0; i < line; i++) fgets(str, MAX_STRING_SIZE, file);
    while((c = fgetc(file)) != EOF) {
        if(c == '\n') break;
        ++ans;
    }
    fclose(file);
    return ans;
}

char getfilechar(char path[], int pos) {
    char ans = 0;
    FILE* file = fopen(path, "r");
    while(pos--) {
        ans = fgetc(file);
    }
    fclose(file);
    return ans;
}

void reterr(char* err) {
    clearfile(CERO);
    addtofile(CERO, err, 0);
}