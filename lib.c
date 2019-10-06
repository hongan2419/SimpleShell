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
	if (strcmp(temp[size-1],"&")==0)
	{
		size--;
		*await = false;	
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
	res[size] = NULL;
	return res;
}

int checkCase(char** args, char* inFile, char* outFile, char** args2){
	int t=0;
	int size;
	for (size = 0; args[size] != NULL; size++)
	{
		if (strcmp(args[size], "|")==0){
			t = size;
		}
	}
	return 0;
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