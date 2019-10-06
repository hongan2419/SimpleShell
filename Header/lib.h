#ifndef _LIB_H_
#define _LIB_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>


#define MAX_LINE 80
char** argsSplit(char*, bool*);
char** loadHistory();
bool addHistory();

#endif