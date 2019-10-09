#ifndef _LIB_H_
#define _LIB_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

#define MAX_LINE 80

#define DEFAULT 0
#define INP_REDIC 1
#define OUT_REDIC 2
#define PIPE 3

int checkCase(char**, char**, char**, char**);
char** argsSplit(char*, bool*);
char** loadHistory();
bool addHistory(char**);

#endif
