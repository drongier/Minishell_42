/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:50:44 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/18 22:33:20 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

void handle_sigint(int sig)
{
    g_signal = sig;
    
    // Si nous sommes dans readline principal
    if (rl_readline_state & RL_STATE_READCMD)
    {
        write(STDOUT_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
    else // Si nous sommes dans le heredoc
    {
        write(STDOUT_FILENO, "^C\n", 3);
    }
}

void setup_signal_handlers(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // Pour redémarrer les appels système interrompus

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Optionnel: Ignorer SIGQUIT (Ctrl+\)
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}
