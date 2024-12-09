/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:31:29 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/09 14:27:01 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_redirections_pipes(t_parser *parser)
{
	if (parser->infile != STDIN_FILENO)
	{
		dup2(parser->infile, STDIN_FILENO);
		close(parser->infile);
	}
	if (parser->outfile != STDOUT_FILENO)
	{
		dup2(parser->outfile, STDOUT_FILENO);
		close(parser->outfile);
	}
}

static void	execute_command(t_shell *shell, t_parser *parser)
{
	char	**args;
	char	*path;

	args = list_to_array(parser->args);
	path = get_external_cmd_path(shell, args[0]);
	if (ft_strncmp(args[0], "env", 3) == 0)
		exec_env(*shell);
	else if (execve(path, args, NULL) == -1)
	{
		ft_error(shell, "%s : command not found\n", args[0], -1);
		exit(127);
	}
}

static void	handle_child_process(t_shell *shell, t_parser *parser)
{
	handle_redirections_pipes(parser);
	execute_command(shell, parser);
	exit(EXIT_SUCCESS);
}

static void	wait_for_children(t_shell *shell)
{
	int	status;

	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
		else
			shell->exit_status = 1;
		shell->flag_pipe = 0;
	}
}

void	exec_with_pipe(t_shell *shell)
{
	t_parser	*parser;
	pid_t		pid;

	parser = shell->parser;
	while (parser)
	{
		pid = fork();
		if (pid == 0)
			handle_child_process(shell, parser);
		else
		{
			close_fds(parser);
			parser = parser->next;
		}
	}
	wait_for_children(shell);
}
