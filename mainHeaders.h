#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

#define MAX_COMMAND_SIZE 1024
#define FILE_PATH_SIZE 1024
#define MAX_STRING_SIZE 1024
#define TEMP_PATH "root/.temp"
#define CLIPBOARD "root/.clipboard"
#define OUTPUT "root/.output"

int executeCommands();
void getstr(char** sptr, char* str);
void createfile(char** sptr);
void insertstr(char** sptr, char* pipeString);
void addtofile(char path[], char* str, int pos);
void copyfile(char src[], char dst[], int left, int right);
void cat(char** sptr);
int getpos(char* path, int line, int pos);
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
