/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:45:30 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/09 14:11:19 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	setup_args(t_list *args, char *cmd_name, char *argv[])
{
	t_list	*arg_node;
	int		i;

	arg_node = args;
	argv[0] = cmd_name;
	i = 1;
	while (arg_node)
	{
		argv[i] = (char *)arg_node->content;
		i++;
		arg_node = arg_node->next;
	}
	argv[i] = NULL;
}

static void	handle_exec_error(t_shell *shell)
{
	if (errno == EACCES)
		exit(126);
	else if (errno == ENOENT)
		exit(127);
	else
	{
		ft_error(shell, "%s : command not found\n", 
			(char *)shell->parser->args->content, -1);
		exit(127);
	}
}

void	child_process(char *path, t_list *args, t_shell *shell)
{
	char	*cmd_name;
	char	*argv[100];
	char	**envp;

	cmd_name = (char *)shell->parser->args->content;
	envp = convert_env_to_array(shell->env);
	if (!envp)
		exit(EXIT_FAILURE);
	setup_args(args, cmd_name, argv);
	if (execve(path, argv, envp) == -1)
		handle_exec_error(shell);
	exit(EXIT_SUCCESS);
}

void	parent_process(pid_t pid, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}
