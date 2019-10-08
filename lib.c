#include "Header/lib.h"

char **argsSplit(char args[MAX_LINE], bool *await)
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
	if (strcmp(temp[size - 1], "&") == 0)
	{
		size--;
		*await = false;
	}
	res = (char **)malloc(sizeof(char *) * (size + 1));
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

int checkCase(char **args, char** inFile, char** outFile, char **args2)
{
	int t = 0;
	int size;
	int lengtgargs2=0;
	for (size = 0; args[size] != NULL; size++)
	{
		if (strcmp(args[size], "|") == 0)
		{
			t = size;
		}
		lengtgargs2=size-t;
	}
	args2= (char**) malloc(lengtgargs2* sizeof(char *));
	if(t)
	{
		//TO DO: 
		//Split args into args and args2
		//Owner: Vinh
		//Ex: args=["ls","-a","|","less", NULL]
		//After the function args should be ["ls", "-a", NULL] and args2 should be ["less", NULL]
		bool check = false;
		for (size = 0; args[size] != NULL; size++)
		{
			if (strcmp(args[size], "|") == 0)
			{
				check=true;
			}
			if(check)
			{
				args2[size-t-1 ]= (char*) malloc(sizeof(args[size]));
				strcpy(args2[size-t-1],args[size]);
			}
		}
		check=false;
		for (size = 0; args[size] != NULL; size++)
		{
			if (strcmp(args[size], "|") == 0)
			{
				check=true;
			}
			if(check)
			{
				free(args[size]);
			}
		}
		return PIPE;
	}
	if (strcmp(args[size - 2], "<") == 0)
	{
		*inFile = (char *)malloc(sizeof(char) * (strlen(args[size - 1])) + 1);
		strcpy(*inFile, args[size - 1]);
		args[size - 2] = NULL;
		return INP_REDIC;
	}
	return 0;
}

char **loadHistory()
{
	//TO DO:
	//Read from file and assign into result
	//Owner: Thao
	FILE *file;
	file = fopen("history.txt", "r");
	char *line_buf = NULL;
	size_t line_buf_size = 0;

	getline(&line_buf, &line_buf_size, file);
	fclose(file);

	bool await = true;
	char **p = argsSplit(line_buf, &await);

	free(line_buf);
	line_buf = NULL;

	return p;
}

bool addHistory(char **args)
{
	//TO DO:
	//Write down args into file to read it later
	//Owner: Thao
	FILE *file;
	file = fopen("history.txt", "w");
	for (int i = 0; args[i] != NULL; i++)
		fprintf(file, "%s ", args[i]);
	fclose(file);
}