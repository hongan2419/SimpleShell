#include "Header/lib.h"

char **argsSplit(char args[MAX_LINE], bool* await)
{
	char **res;
	char delim[] = " ";
	char *temp[MAX_LINE];
	char *ptr = strtok(args, delim);
	int size = 0;
	while (ptr != NULL)
	{
		temp[size++] = strdup(ptr);
		ptr = strtok(NULL, delim);
	}
	res = (char **)malloc(sizeof(char *) * size);
	if (res == NULL)
	{
		return NULL;
	}
	for (int i = 0; i < size; i++)
	{
		res[i] = (char *)malloc(sizeof(char) * (strlen(temp[i]) + 1));
		strcpy(res[i], temp[i]);
		free(temp[i]);
	}
	if (strcmp(res[size-1],"&")==0)
	{
		size--;
		*await = false;	
	}
	res[size] = NULL;
	return res;
}


char** loadHistory(){
	//TO DO:
	//Read from file and assign into result
	//Owner: Thao
}

bool addHistory(char** args){
	//TO DO:
	//Write down args into file to read it later
	//Owner: Thao
}