/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:50:44 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/27 22:17:20 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sigint(int sig)
{
    g_signal = sig;
    if (rl_readline_state & RL_STATE_READCMD)
    {
        write(STDOUT_FILENO, "^C\n", 3); // ^C
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
	// else //garder pour notre heredoc... ou pas ?
	// 	write(STDOUT_FILENO, "^C\n", 3); // sleep 100
}

void setup_signal_handlers(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // mieux avec restart
	
    sigaction(SIGINT, &sa, NULL);
    
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}
