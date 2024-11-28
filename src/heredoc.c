/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:31:07 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/28 14:47:19 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_heredoc(t_shell *shell, t_parser *parser, const char *delimiter)
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
    if (pid == 0)
    {
		signal(SIGINT, SIG_DFL);
		save_terminal_settings();
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
    else if (pid > 0)
    {
        close(pipefd[1]);
        waitpid(pid, &status, 0);

        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {
			write(STDOUT_FILENO, "^C\n", 3);
            close(pipefd[0]);
			g_signal = WTERMSIG(status);
			restore_terminal_settings();
            rl_on_new_line();
        	rl_replace_line("", 0);
        	rl_redisplay();
            parser->infile = -1;
			return;
        }
		else if (WIFEXITED(status))
        	shell->exit_status = WEXITSTATUS(status);

        parser->infile = pipefd[0];
    }
}
