#include "../minishell.h"

void	exec_clear(t_shell *shell)
{
	cleanup(shell);
	printf("\033[2J\033[1;1H");
}