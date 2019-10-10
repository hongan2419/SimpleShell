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
	int x = 0;
	while (1)
	{
		printf("osh>");
		fflush(stdout);
		if (!fgets(line, MAX_LINE, stdin))
		{
			continue;
		}
		await = true;
		args = argsSplit(line, &await);
		if (args == NULL)
		{
			continue;
		}
		if (strcmp(args[0], "exit") == 0 && args[1] == NULL)
		{
			free(args);
			break;
		}
		if (strcmp(args[0], "!!") == 0 && args[1] == NULL)
		{
			free(args);
			args=loadHistory();
			if (args[0] == NULL){
				printf("No commands in history.");
				}
			else{
				for (int i = 0; args[i]!=NULL; i++)
					printf("%s ",args[i]);
				}
		}
		args2 = NULL;
		int t = checkCase(args, &inFile, &outFile, &args2);
		int pid = fork();
		if (pid < 0)
		{
			fprintf(stderr, "ERROR: forking child failed\n");
			continue;
		}
		if (pid == 0)
		{
			addHistory(args);
			if (t == PIPE)
			{
				int pipes[2];
				if (pipe(pipes) < 0)
				{
					fprintf(stderr, "ERROR: making pipe failed\n");
					continue;
				}
				int pidLayer2 = fork();
				if (pidLayer2 < 0)
				{
					fprintf(stderr, "ERROR: forking child failed\n");
					continue;
				}
				if (pidLayer2 == 0)
				{
					close(pipes[0]);
					int temp = dup(STDOUT_FILENO);
					dup2(pipes[1], STDOUT_FILENO);
					close(pipes[1]);
					if (execvp(args[0], args) < 0)
					{
						dup2(temp, STDOUT_FILENO);
						fprintf(stderr, "ERROR: %s\n", strerror(errno));
						exit(errno);
					}
					exit(0);
				}
				else
				{
					wait(NULL);
					close(pipes[1]);
					dup2(pipes[0], STDIN_FILENO);
					close(pipes[0]);
					if (execvp(args2[0], args2) < 0)
					{

						fprintf(stderr, "ERROR: %s\n", strerror(errno));
						exit(errno);
					}
				}
			}
			else
			{
				if (t == INP_REDIC)
				{
					fileOpen = open(inFile, O_RDONLY);
					dup2(fileOpen, STDIN_FILENO);
					close(fileOpen);
					free(inFile);
				}
				if (t == OUT_REDIC)
				{
					fileOpen = open(outFile, O_WRONLY | O_CREAT);
					dup2(fileOpen, STDOUT_FILENO);
					close(fileOpen);
					free(outFile);
				}
				if (execvp(args[0], args) < 0)
				{
					fprintf(stderr, "ERROR: %s\n", strerror(errno));
					exit(errno);
				}
			}
			exit(0);
		}
		else
		{
			if (await)
				wait(NULL);
		}
		if (args)
			freeMem(args);
		if (args2)
			freeMem(args2);
	}
	return 0;
}