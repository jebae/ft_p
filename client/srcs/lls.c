#include "client.h"

int		handle_lls(char **args)
{
	int		pid;

	pid = fork();
	if (pid == 0)
	{
		if (args[1] == NULL)
			execl("/bin/ls", "/bin/ls", NULL);
		else
			execl("/bin/ls", "/bin/ls", args[1], NULL);
	}
	else if (pid > 0)
	{
		if (wait4(pid, NULL, 0, NULL) == -1)
		{
			printf("failed to execute ls\n");
			return (-1);
		}
	}
	else
	{
		printf("failed to execute ls\n");
		return (-1);
	}
	return (0);
}