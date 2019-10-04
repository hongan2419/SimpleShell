#include "Header/lib.h"

int main()
{
	char line[MAX_LINE];
	while (1)
	{
		printf("osh>");
		fflush(stdout);
		if (!fgets(line, MAX_LINE, stdin))
			break;
		char *args[] = {line, (char *)0};
		int pid = fork();
		if (pid == 0)
		{
			execvp(line, args);
			perror("exec");
			exit(1);
		}
		else
		{
			wait(NULL);
		}
	}

	return 0;
}