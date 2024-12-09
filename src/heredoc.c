/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:31:07 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/09 15:50:04 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_content(char *content, char *line)
{
	char	*tmp;
	char	*new_content;

	if (content)
	{
		tmp = ft_strjoin(content, "\n");
		free(content);
		new_content = ft_strjoin(tmp, line);
		free(tmp);
	}
	else
		new_content = ft_strdup(line);
	return (new_content);
}

static void	write_heredoc_content(int fd, char *content)
{
	if (content)
	{
		write(fd, content, ft_strlen(content));
		write(fd, "\n", 1);
	}
}

static void	handle_child_process(int *pipefd, const char *delimiter)
{
	struct sigaction	sa;
	char				*line;
	char				*content;

	content = NULL;
	setup_heredoc_signals(&sa);
	close(pipefd[0]);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		content = append_content(content, line);
		free(line);
	}
	write_heredoc_content(pipefd[1], content);
	free(content);
	close(pipefd[1]);
	exit(EXIT_SUCCESS);
}

static void	handle_parent_process(t_parser *parser, int *pipefd, pid_t pid)
{
	int	status;

	close(pipefd[1]);
	waitpid(pid, &status, 0);
	parser->infile = pipefd[0];
}

void	handle_heredoc(t_shell *shell, t_parser *parser, const char *delimiter)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return ;
	}
	pid = fork();
	if (pid == 0)
		handle_child_process(pipefd, delimiter);
	else
		handle_parent_process(parser, pipefd, pid);
}
