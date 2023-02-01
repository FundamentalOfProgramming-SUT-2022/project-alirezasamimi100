#include "mainHeaders.h"

int spos = -1, pos, cury, curx, lr, lc;

struct match *ffa = NULL, *fnow = NULL;

void printmf(int rows, int columns) {
    struct match *wtf = ffa;
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
        int tj = getpos(OPEN_FILE, lr + i, 0);
        tj += lc;
        for(int j = lc; j < sz && j < lc + columns - 2 && str[j] != '\n'; j++) {
            if(ffa) {
                while(wtf->left !=-1 && wtf->right <= tj) wtf = wtf->next;
            }
            if(wtf && wtf->left != -1 && tj >= wtf->left && tj < wtf->right) attron(COLOR_PAIR(2));
            if(spos != -1 && ((tj >= spos && tj < pos) || (tj >= pos && tj < spos))) attron(COLOR_PAIR(1));
            printw("%c", str[j]);
            if(spos != -1 && ((tj >= spos && tj < pos) || (tj >= pos && tj < spos))) attroff(COLOR_PAIR(1));
            if(wtf && wtf->left != -1 && tj >= wtf->left && tj < wtf->right) attroff(COLOR_PAIR(2));
            ++tj;
        }
        printw("\n");
    }
    fclose(file);
}

void gol(int rows, int columns) {
    if(curx + lc == 2) return;
    if(lc && curx <= 6) {
        --lc;
    } else {
        --curx;
    }
}

void gor(int rows, int columns) {
    int sz = getcharcount(OPEN_FILE, cury + lr);
    if(curx + lc - 2 == sz) return;
    if(columns - curx <= 5 && sz + 3 - lc - columns > 0) ++lc;
    else ++curx;
}

void gou(int rows, int columns) {
    if(cury + lr == 0) return;
    int sz = getcharcount(OPEN_FILE, cury + lr - 1);
    while(curx + lc - 2 > sz) gol(rows, columns);
    if(lr && cury <= 4) {
        --lr;
    } else {
        --cury;
    }
}

void god(int rows, int columns) {
    int sz = getlinecount(OPEN_FILE);
    if(cury + lr + 1 == sz) return;
    int st = getcharcount(OPEN_FILE, cury + lr + 1);
    while(curx + lc - 2 > st) gol(rows, columns);
    if(rows - cury <= 5 && sz - lr - rows > 0) ++lr;
    else ++cury;
}

void gofirst(int rows, int columns) {
    curx = 2;
    cury = lc = lr = 0;
}

void goend(int rows, int columns) {
    int t = cury + lr - 1;
    while(cury + lr != t) {
        t = cury + lr;
        god(rows, columns);
    }
    t = curx + lc - 1;
    while(curx + lc != t) {
        t = curx + lc;
        gor(rows, columns);
    }
}

void mvtopos(int rows, int columns, int pos) {
    gofirst(rows, columns);
    int sz = getlinecount(OPEN_FILE);
    for(int i = 0; i < sz; i++) {
        int t = getcharcount(OPEN_FILE, i);
        if(pos <= t) {
            while(pos--) {
                gor(rows, columns);
            }
            break;
        }
        pos -= t + 1;
        god(rows, columns);
    }
}

int main() {
    int mode = 0, rows, columns, upd = 0, coms = 0, fds = 0;
    char path[FILE_PATH_SIZE], *modes[] = {"NORMAL", "INSERT", "VISUAL", "COMMAND", "FIND", "FIND"},
        filename[FILE_PATH_SIZE], findstr[MAX_STRING_SIZE], command[MAX_COMMAND_SIZE], cers[MAX_COMMAND_SIZE], ptpy[FILE_PATH_SIZE], ffs[MAX_STRING_SIZE];
    path[0] = findstr[0] = command[0] = '\0';
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    cbreak();
    noecho();
    keypad(stdscr, true);
    ESCDELAY = 10;
    getmaxyx(stdscr, rows, columns);
    clearfile(OPEN_FILE);
    clearfile(CLIPBOARD);
    curx = 2;
    cury = lr = lc = 0;
    while(1) {
        pos = getpos(OPEN_FILE, cury + lr, curx + lc - 2);
        clearfile(CERO);
        printmf(rows - 2, columns);
        if(path[0]) {
            strcpy(filename, path);
        } else {
            strcpy(filename, "NEW FILE");
        }
        if(!upd) {
            strcat(filename, " +");
        }
        mvprintw(rows - 2, 0, "%s | %s\n", modes[mode], filename);
        if(mode == 3) {
            printw(":%s\n", command);
        } else if(mode == 4) {
            printw("/%s\n", findstr);
        }
        move(cury, curx);
        refresh();
        int key = getch();
        if(key == '\e') {
            mode = 0;
            spos = -1;
            ffa = fnow = NULL;
            fds = coms = 0;
            findstr[0] = command[0] = 0;
            continue;
        }
        if(mode == 0) {
            switch (key){
            case 'i':
                mode = 1;
                break;
            case 'v':
                mode = 2;
                spos = pos;
                break;
            case ':':
                mode = 3;
                break;
            case '/':
                mode = 4;
                break;
            case 'h':
                gol(rows - 2, columns);
                break;
            case 'l':
                gor(rows - 2, columns);
                break;
            case 'j':
                god(rows - 2, columns);
                break;
            case 'k':
                gou(rows - 2, columns);
                break;
            case 'u':
                gofirst(rows - 2, columns);
                restorebackup(OPEN_FILE);
                break;
            case 'p':
                createbackup(OPEN_FILE);
                faddtofile(OPEN_FILE, CLIPBOARD, pos);
                break;
            case '=':
                autoindent(NULL, 1);
                break;
            case 'r':
                gofirst(rows - 2, columns);
                break;
            case 't':
                goend(rows - 2, columns);
                break;
            }
        } else if(mode == 1) {
            upd = 0;
            switch (key){
            case KEY_BACKSPACE:
                if(pos) {
                    if(lc + curx > 2) {
                        gol(rows - 2, columns);
                    } else {
                        gou(rows - 2, columns);
                        int tp = lc + curx - 1;
                        while(lc + curx > tp) {
                            tp = lc + curx;
                            gor(rows - 2, columns);
                        }
                    }
                    createbackup(OPEN_FILE);
                    removefromfile(OPEN_FILE, pos - 1, pos);
                }
                break;
            default:
                char wtf[2] = " ";
                wtf[0] = key;
                createbackup(OPEN_FILE);
                addtofile(OPEN_FILE, wtf, pos);
                if(key == '\n') {
                    while(lc + curx > 2) gol(rows - 2, columns);
                    god(rows - 2, columns);
                } else {
                    gor(rows - 2, columns);
                }
            }
        } else if(mode == 2) {
            switch (key){
            case 'h':
                gol(rows - 2, columns);
                break;
            case 'l':
                gor(rows - 2, columns);
                break;
            case 'j':
                god(rows - 2, columns);
                break;
            case 'k':
                gou(rows - 2, columns);
                break;
            case 'd':
                upd = 0;
                createbackup(OPEN_FILE);
                clearfile(CLIPBOARD);
                copyfile(OPEN_FILE, CLIPBOARD, min(pos, spos), max(pos, spos));
                removefromfile(OPEN_FILE, min(pos, spos), max(pos, spos));
                if(spos < pos) {
                    mvtopos(rows - 2, columns, spos);
                }
                mode = 0;
                spos = -1;
                break;
            case 'y':
                clearfile(CLIPBOARD);
                copyfile(OPEN_FILE, CLIPBOARD, min(pos, spos), max(pos, spos));
                mode = 0;
                spos = -1;
                break;
            case 'r':
                gofirst(rows - 2, columns);
                break;
            case 't':
                goend(rows - 2, columns);
                break;
            }
        } else if(mode == 3) {
            switch (key){
            case KEY_BACKSPACE:
                if(coms) {
                    command[--coms] = 0;
                } else {
                    mode = 0;
                }
                break;
            case '\n':
                upd = 0;
                strcpy(ptpy, path);
                int k = executeCommands(command, path);
                if(k >= 8) {
                    if(k >= 10) {
                        mvtopos(rows - 2, columns, k - 10);
                    }
                }
                if(k == 1) {
                    endwin();
                    return 0;
                }
                if(ptpy[0] && (k >= 2 || getcharcount(OUTPUT, 0))) {
                    if(k == 3) strcpy(ptpy, path);
                    upd = 1;
                    clearfile(ptpy + 1);
                    copyfile(OPEN_FILE, ptpy + 1, 0, -1);
                }
                if(k == 3) reterr("Saved!");
                if(k == 2) {
                    gofirst(rows - 2, columns);
                    clearfile(OPEN_FILE);
                    copyfile(path + 1, OPEN_FILE, 0, -1);
                }
                if(getcharcount(OUTPUT, 0)) {
                    gofirst(rows - 2, columns);
                    clearfile(OPEN_FILE);
                    copyfile(OUTPUT, OPEN_FILE, 0, -1);
                    path[0] = 0;
                }
                mode = 0;
                coms = 0;
                command[0] = 0;
                break;
            default:
                command[coms] = key;
                command[++coms] = 0;
            }
            
        } else if(mode == 4) {
            switch (key){
            case KEY_BACKSPACE:
                if(fds) {
                    findstr[--fds] = 0;
                } else {
                    mode = 0;
                }
                break;
            case '\n':
                char* help = findstr;
                struct wildcard *pls = getinstr(&help, ffs, 1);
                ffa = findinfile(OPEN_FILE, ffs, pls, 1);
                mode = 5;
                fds = 0;
                findstr[0] = 0;
                if(ffa && ffa->left != -1) {
                    mvtopos(rows - 2, columns, ffa->left);
                    fnow = ffa;
                }
                break;
            default:
                findstr[fds] = key;
                findstr[++fds] = 0;
            }
        } else if(mode == 5) {
            switch (key){
            case 'n':
                if(fnow && fnow->next) {
                    fnow = fnow->next;
                    if(fnow->left == -1) fnow = ffa;
                    mvtopos(rows - 2, columns, fnow->left);
                }
                break;
            default:
                fnow = NULL;
                struct match *tmp = ffa;
                while(ffa) {
                    if(ffa->left != -1) tmp = ffa->next;
                    else tmp = NULL;
                    free(ffa);
                    ffa = tmp;
                }
                mode = 0;
            }
        }
        int ty = cury, tx = curx;
        filetostr(CERO, cers);
        if(mode < 3) mvprintw(rows - 1, 0, "%s\n", cers);
        cury = ty;
        curx = tx;
        refresh();
    }
    endwin();
    return 0;
}