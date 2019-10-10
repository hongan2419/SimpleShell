#include "Header/lib.h"

int main(int argc, char **argv)
{
	char line[MAX_LINE];
	bool await = true;
	int fileOpen;
	char *inFile;
	char *outFile;
	char **args;
	char **args2;
	while (1)
	{
		// INPUT DATA
		printf("osh>");
		fflush(stdout);
		// IGNORE EMPTY LINE
		if (!fgets(line, MAX_LINE, stdin))
		{
			continue;
		}
		// CHECK IF ANY PREVIOUS PROCESS HAVE DONE
		if (waitpid(-1, NULL, WNOHANG) > 0)
		{
			printf("previous process done\n");
		}
		// SPLIT ARGUMENT AND CHECK IF WE SHOULD WAIT
		args = argsSplit(line);
		if (args == NULL)
		{
			continue;
		}
		// CHECK EXIT CMD
		if (strcmp(args[0], "exit") == 0 && args[1] == NULL)
		{
			free(args);
			break;
		}
		// CHECK HISTORY CMD
		if (strcmp(args[0], "!!") == 0 && args[1] == NULL)
		{
			// LOAD HISTORY CMD
			free(args);
			args = loadHistory();
			// ERROR HANDLE
			if (args == NULL)
			{
				fprintf(stderr, "ERROR: No commands in history\n");
				continue;
			}
			else
			{
				// PRINT HISTORY CMD
				for (int i = 0; args[i] != NULL; i++)
					printf("%s ", args[i]);
				printf("\n");
				fflush(stdout);
			}
		}
		// ADD HISTORY CMD
		addHistory(args);
		// CHECK CHILD PROCESS RUN SEPERATE
		await = checkAwait(args);
		// CHECK CASE OF ARGS
		args2 = NULL;
		int t = checkCase(args, &inFile, &outFile, &args2);
		// CREATE CHILD PROCESS
		int pid = fork();
		// ERROR HANDLE
		if (pid < 0)
		{
			fprintf(stderr, "ERROR: forking child failed\n");
			continue;
		}
		if (pid == 0)
		{
			// CHILD PROCESS
			// PIPE CASE
			if (t == PIPE)
			{
				int pipes[2];
				// PIPE CREATE AND HANDLE ERROR
				if (pipe(pipes) < 0)
				{
					fprintf(stderr, "ERROR: making pipe failed\n");
					continue;
				}
				// CREATE CHILD PROCESS
				int pidLayer2 = fork();
				// HANDLE ERROR
				if (pidLayer2 < 0)
				{
					fprintf(stderr, "ERROR: forking child failed\n");
					continue;
				}
				if (pidLayer2 == 0)
				{
					// ON CHILD PROCESS
					close(pipes[0]);
					int temp = dup(STDOUT_FILENO);
					dup2(pipes[1], STDOUT_FILENO);
					close(pipes[1]);
					// HANDLE EXEC ERROR
					if (execvp(args[0], args) < 0)
					{
						dup2(temp, STDOUT_FILENO);
						fprintf(stderr, "ERROR: %s\n", strerror(errno));
						exit(errno);
					}
				}
				else
				{
					// ON PARENT PROCESS
					wait(NULL);
					close(pipes[1]);
					dup2(pipes[0], STDIN_FILENO);
					close(pipes[0]);
					// HANDLE EXEC ERROR
					if (execvp(args2[0], args2) < 0)
					{
						fprintf(stderr, "ERROR: %s\n", strerror(errno));
						exit(errno);
					}
				}
			}
			// REDIRECT INPUT CASE
			if (t == INP_REDIC)
			{
				fileOpen = open(inFile, O_RDONLY);
				// HANDLE FILE ERROR
				if (fileOpen < 0)
				{
					fprintf(stderr, "ERROR: file failed\n");
					exit(1);
				}
				dup2(fileOpen, STDIN_FILENO);
				close(fileOpen);
				free(inFile);
			}
			// REDIRECT OUTPUT CASE
			if (t == OUT_REDIC)
			{
				fileOpen = open(outFile, O_WRONLY | O_CREAT);
				// HANDLE FILE ERROR
				if (fileOpen < 0)
				{
					fprintf(stderr, "ERROR: file failed\n");
					exit(1);
				}
				dup2(fileOpen, STDOUT_FILENO);
				close(fileOpen);
				free(outFile);
			}
			// HANDLE EXEC ERROR
			if (execvp(args[0], args) < 0)
			{
				fprintf(stderr, "ERROR: %s\n", strerror(errno));
				exit(errno);
			}
		}
		else
		{
			// ON PARENT PROCESS
			if (await)
				wait(NULL);
		}
		// FREE MEM
		if (args)
			freeMem(args);
		if (args2)
			freeMem(args2);
	}
	return 0;
}