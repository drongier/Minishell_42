#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include "minishell.h"

int	* piping()
{
	int *pipefd;

	pipefd = malloc(sizeof(int) * 2);
	if (pipe(pipefd) == -1)
	{
		printf("Pipe failure\n");
		exit(1);
	}
	return pipefd;
}

/* int main(void)
{
	pid_t pid;
	int pipefd[2];
	char		*buffer;

	buffer = malloc(sizeof(char) * 100 + 1);
	if (pipe(pipefd))
	{
		printf("Pipe failure\n");
		exit(1);
	}
	printf("%d\n%d\n", pipefd[0], pipefd[1]);
	pid = fork();
	if (pid == 0)
	{
		printf("enfant\n");
		close(pipefd[1]);
		read(pipefd[0], buffer, 100);
		return EXIT_SUCCESS;
	}
	else if (pid < 0)
	{
		printf("Fork Failed\n");
		return EXIT_FAILURE;
	}
	else
	{
		printf("Chakib is here\n");
		close(pipefd[0]);
		write(pipefd[1], "Hello", 5);

		return EXIT_SUCCESS;
	}
} */
