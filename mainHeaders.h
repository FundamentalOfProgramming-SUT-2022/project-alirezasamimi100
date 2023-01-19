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

void executeCommands();
void createfile(char**);