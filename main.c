#include "Header/lib.h"

int main(int argc, char **argv)
{
	char line[MAX_LINE];
	bool await;
	int fileOpen;
	char *inFile;
	char *outFile;
	char **args;
	char **args2;
	while (1)
	{
		printf("osh>");
		fflush(stdout);
		dup2(0, 0);
		if (!fgets(line, MAX_LINE, stdin))
		{
			break;
		}
		line[strcspn(line, "\n")] = '\0';
		await = true;
		args = argsSplit(line, &await);
		if (args == NULL)
		{
			continue;
		}
		int t = checkCase(args, &inFile, &outFile, args2);
		int temp;
		if (t == INP_REDIC)
		{
			fileOpen = open(inFile, O_RDONLY);
			temp=dup(STDIN_FILENO);
			dup2(fileOpen, STDIN_FILENO);
			close(fileOpen);
			free(inFile);
		}
		else if (t == OUT_REDIC)
		{
			fileOpen = open(outFile, O_WRONLY);
			temp=dup(STDOUT_FILENO);
			dup2(fileOpen, STDOUT_FILENO);
			close(fileOpen);
			free(outFile);
		}
		if (strcmp(args[0], "!!") == 0)
		{
			free(args);
			args = loadHistory();
			for (int i = 0; args[i] != NULL; i++)
				printf("%s ", args[i]);
		}
		int pid = fork();
		if (pid == 0)
		{
			addHistory(args);
			execvp(args[0], args);
			exit(0);
		}
		else
		{
			if (await)
				wait(NULL);
		}
		if (t == INP_REDIC)
			dup2(temp, STDIN_FILENO);
		else if (t==OUT_REDIC)
			dup2(temp, STDOUT_FILENO);
		free(args);
	}
	return 0;
}