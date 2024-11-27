/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:50:44 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/27 15:17:17 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

void handle_sigint(int sig)
{
    g_signal = sig;
	//printf("g_signal = %d\n", g_signal);
    
    if (rl_readline_state & RL_STATE_READCMD)
    {
        write(STDOUT_FILENO, "^C\n", 3);
		//printf("sig === %d\n", sig);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
	/* else //garder pour notre heredoc... ou pas ?
		write(STDOUT_FILENO, "^C\n", 3); */
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
