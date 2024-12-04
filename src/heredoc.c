/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:31:07 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/04 11:58:25 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
		exit(EXIT_FAILURE);
}
// Nouvelle version de handle_heredoc
void	handle_heredoc(t_shell *shell, t_parser *parser, const char *delimiter)
{
	pid_t pid;
	int pipefd[2];
	int status;
	char *line;
	char *content = NULL;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return;
	}
	pid = fork();
	if (pid == 0) // Processus fils
	{
		struct sigaction sa;
		sa.sa_handler = heredoc_signal_handler;
		sa.sa_flags = 0;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGINT, &sa, NULL);
		close(pipefd[0]);
		while (1)
		{
			line = readline("heredoc> ");
			if (!line || strcmp(line, delimiter) == 0)
			{
				free(line);
				break;
			}
			if (content)
			{
				char *tmp = ft_strjoin(content, "\n");
				free(content);
				content = ft_strjoin(tmp, line);
				free(tmp);
			}
			else
				content = ft_strdup(line);
			free(line);
		}
		if (content) // Écrire le contenu accumulé dans le pipe
		{
			write(pipefd[1], content, ft_strlen(content));
			write(pipefd[1], "\n", 1); // Ajouter un retour à la ligne à la fin
		}
		free(content);
		close(pipefd[1]);
		exit(EXIT_SUCCESS); // Le fils se termine toujours avec EXIT_SUCCESS
	}
	else // Processus parent
	{
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		parser->infile = pipefd[0];
	}
}
