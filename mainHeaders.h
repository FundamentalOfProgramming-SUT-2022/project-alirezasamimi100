#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#define MAX_COMMAND_SIZE 1024
#define FILE_PATH_SIZE 1024
#define MAX_STRING_SIZE 1024
#define COMMAND_COUNT 14
#define TEMP_PATH "root/.temp"

void executeCommands();
void getstr(char**, char*);
void createfile(char**);
void insertstr(char**);
void addtofile(char*, char*, int, int);
void copyfile(FILE*, FILE*);
