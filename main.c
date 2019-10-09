#include "Header/lib.h"

int main()
{
	char line[MAX_LINE];
	bool await;
	while (1)
	{
		printf("osh>");
		fflush(stdout);
		if (!fgets(line, MAX_LINE, stdin))
		{
			break;
		}
		line[strcspn(line, "\n")] = 0;
		await = true;
		char **args = argsSplit(line, &await);
		if (strcmp(args[0],"!!")==0)
		{
			args=loadHistory();
			if (args[0] == NULL){
				printf("No commands in history.");
				}
			else{
				for (int i = 0; args[i]!=NULL; i++)
					printf("%s ",args[i]);
				}
		}
		int pid = fork();
		if (pid == 0)
		{
			addHistory(args);
			execvp(args[0], args);
			return 0;
		}
		else
		{
			if (await)
				wait(NULL);
		}
		free(args);
	}

	return 0;
}