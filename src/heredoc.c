/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:31:07 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/27 15:23:45 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Nouvelle version de handle_heredoc
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

    g_signal = SIGUSR1; // Signaler le début du heredoc AVANT le fork

    pid = fork();
    if (pid == 0) // Processus fils
    {
        close(pipefd[0]);

        // Restaurer le comportement par défaut de SIGINT pour le processus fils
        /* struct sigaction sa_child;
        sa_child.sa_handler = SIG_DFL;
        sigemptyset(&sa_child.sa_mask);
        sa_child.sa_flags = 0;
        sigaction(SIGINT, &sa_child, NULL); */

        while (1)
        {
			setup_signal_handlers();
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

        // Gestion du status pour détecter l'interruption par SIGINT dans le fils
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {
            shell->exit_status = 130;
            close(pipefd[0]);
            parser->infile = -1;
            g_signal = 0; // Remettre g_signal à 0 après la gestion du signal
            return;
        }

        parser->infile = pipefd[0];
        g_signal = 0; // Signaler la fin du heredoc APRÈS le waitpid
    }
}
