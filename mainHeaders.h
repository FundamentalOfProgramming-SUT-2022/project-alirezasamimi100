#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <ncurses.h>

#define MAX_COMMAND_SIZE 1024
#define FILE_PATH_SIZE 1024
#define MAX_STRING_SIZE 1024 * 256
#define TEMP_PATH "root/.temp"
#define CLIPBOARD "root/.clipboard"
#define OUTPUT "root/.output"
#define BASE "root"
#define OPEN_FILE "root/.open"
#define SLOPEN_FILE "/root/.open"
#define CERO "root/.error"
#define max(X,Y) (X > Y ? X : Y)
#define min(X,Y) (X < Y ? X : Y)
#define TABSIZE 4

struct wildcard {
    int pos;
    struct wildcard* next;
};
struct match {
    int left, right, word;
    struct match* next;
};

int executeCommands(char* command, char* path);
struct wildcard* getinstr(char** sptr, char* str, int find);
void createfile(char** sptr);
void insertstr(char** sptr, char* pipeString);
void addtofile(char path[], char* str, int pos);
void faddtofile(char path[], char fstr[], int pos);
void copyfile(char src[], char dst[], int left, int right);
void cat(char** sptr);
int getpos(char path[], int line, int pos);
void removestr(char** sptr);
void removefromfile(char path[], int left, int right);
int filesize(char path[]);
void filetostr(char path[], char* str);
void strtofile(char* str, char path[]);
void hidefile(char path[], char hidden[]);
void createbackup(char path[]);
int checkfile(char path[]);
void clearfile(char path[]);
void restorebackup(char path[]);
void undo(char** sptr);
void printfile(char path[]);
void copy(char** sptr);
void cut(char** sptr);
void paste(char** sptr);
struct match* findinfile(char path[], char ptrn[], struct wildcard* wcards, int del);
void find(char** sptr, char* pipeString);
int replace(char** sptr);
void grep(char** sptr, char* pipeString);
void tree(char** sptr);
void prtree(char path[], int depth, int pre);
void autoindent(char** sptr, int wtf);
void compare(char** sptr);
int getlinecount(char path[]);
int getcharcount(char path[], int line);
char getfilechar(char path[], int pos);
void reterr(char *err);