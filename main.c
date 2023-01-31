#include "mainHeaders.h"

void printmf(int lr, int lc, int rows, int columns) {
    FILE* file = fopen(OPEN_FILE, "r");
    char str[MAX_STRING_SIZE];
    for(int i = 0; i < lr; i++) fgets(str, MAX_STRING_SIZE, file);
    for(int i = 0; i < rows; i++) {
        move(i, 0);
        if(!fgets(str, MAX_STRING_SIZE, file)) {
            printw("~\n");
            continue;
        }
        int sz = strlen(str);
        printw("%d ", i + lr + 1);
        for(int j = lc; j < sz && j < lc + columns - 2 && str[j] != '\n'; j++) {
            printw("%c", str[j]);
        }
        if(sz - lc < columns - 2) printw("\n");
    }
    fclose(file);
}

void gol(int *cury, int *curx, int *lr, int *lc, int rows, int columns) {
    if(*curx == 2) return;
    if(*lc && *curx <= 6) {
        --*lc;
    } else {
        --*curx;
    }
}

void gor(int *cury, int *curx, int *lr, int *lc, int rows, int columns) {
    int sz = getcharcount(OPEN_FILE, *cury + *lr);
    if(*curx + *lc - 2 == sz) return;
    if(columns - *curx <= 5 && sz + 3 - *lc - columns > 0) ++*lc;
    else ++*curx;
}

void gou(int *cury, int *curx, int *lr, int *lc, int rows, int columns) {
    if(!*cury) return;
    int sz = getcharcount(OPEN_FILE, *cury + *lr - 1);
    while(*curx + *lc - 2 > sz) gol(cury, curx, lr, lc, rows, columns);
    if(*lr && *cury <= 4) {
        --*lr;
    } else {
        --*cury;
    }
}

void god(int *cury, int *curx, int *lr, int *lc, int rows, int columns) {
    int sz = getlinecount(OPEN_FILE);
    if(*cury + *lr + 1 == sz) return;
    int st = getcharcount(OPEN_FILE, *cury + *lr + 1);
    while(*curx + *lc - 2 > st) gol(cury, curx, lr, lc, rows, columns);
    if(rows - *cury <= 5 && sz - *lr - rows > 0) ++*lr;
    else ++*cury;
}

int main() {
    int mode = 0, rows, columns, upd = 0;
    char path[FILE_PATH_SIZE], *modes[] = {"NORMAL", "INSERT", "VISUAL", "COMMAND", "FIND"}, filename[FILE_PATH_SIZE];
    path[0] = '\0';
    initscr();
    cbreak();
    noecho();
    getmaxyx(stdscr, rows, columns);
    clearfile(OPEN_FILE);
    int cury = 0, curx = 2, lr = 0, lc = 0;
    while(1) {
        printmf(lr, lc, rows - 2, columns);
        if(path[0]) {
            strcpy(filename, "/");
            strcat(filename, path);
        } else {
            strcpy(filename, "NEW FILE");
        }
        if(!upd) {
            strcat(filename, " +");
        }
        mvprintw(rows - 2, 0, "%s | %s", modes[mode], filename);
        move(cury, curx);
        refresh();
        int key = getch();
        if(key == '\e') {
            mode = 0;
            continue;
        }
        if(mode == 0) {
            switch (key){
            case 'i':
                mode = 1;
                break;
            case 'v':
                mode = 2;
                break;
            case ':':
                mode = 3;
                break;
            case '/':
                mode = 4;
                break;
            case 'h':
                gol(&cury, &curx, &lr, &lc, rows - 2, columns);
                break;
            case 'l':
                gor(&cury, &curx, &lr, &lc, rows - 2, columns);
                break;
            case 'j':
                god(&cury, &curx, &lr, &lc, rows - 2, columns);
                break;
            case 'k':
                gou(&cury, &curx, &lr, &lc, rows - 2, columns);
                break;
            }
        }
        refresh();
    }
    endwin();
    return 0;
}