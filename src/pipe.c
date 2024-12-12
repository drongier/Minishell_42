/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:31:29 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/12 11:03:15 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child_redirections(int prev_fd, t_parser *parser)
{
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
			exit(1);
		close(prev_fd);
	}
	if (parser->next)
	{
		if (dup2(parser->outfile, STDOUT_FILENO) == -1)
			exit(1);
		close(parser->outfile);
		close(parser->next->infile);
	}
}

static void	handle_parent_pipes(int *prev_fd, t_parser *parser)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (parser->next)
	{
		close(parser->outfile);
		*prev_fd = parser->next->infile;
	}
}

static void	handle_child(t_shell *shell, t_parser *parser, int prev_fd)
{
	handle_child_redirections(prev_fd, parser);
	handle_redirections_pipes(parser);
	execute_command(shell, parser);
}

static void	process_pipe(t_shell *shell, t_parser *parser, int *prev_fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (parser->next)
		setup_pipe(parser, pipe_fd);
	pid = fork();
	if (pid < 0)
	{
		handle_pipe_error(*prev_fd, parser);
		return ;
	}
	if (pid == 0)
		handle_child(shell, parser, *prev_fd);
	handle_parent_pipes(prev_fd, parser);
}

void	exec_with_pipe(t_shell *shell)
{
	t_parser	*parser;
	int			prev_fd;

	parser = shell->parser;
	prev_fd = -1;
	while (parser)
	{
		process_pipe(shell, parser, &prev_fd);
		parser = parser->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	wait_for_children(shell);
}
