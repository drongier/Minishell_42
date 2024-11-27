/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:02:35 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/27 15:05:09 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void minishell(char **env)
{
    t_shell shell;

    shell.cmdline = NULL;
    shell.env = NULL;
    shell.lexer = NULL;
    shell.parser = NULL;
    shell.exit_status = 0;
    shell.in_heredoc = 0;
    
    init_env(&shell, env);
    
    while (1)
    {
		g_signal = 0;
        shell.cmdline = readline("\033[36;1m ➜ minishell$ \033[0m");
        
        if (!shell.cmdline)
        {
            write(STDOUT_FILENO, "exit\n", 5);
            break;
        }
        if (shell.cmdline && shell.cmdline[0] != '\0')
		{
        	add_history(shell.cmdline);
        	lexer(&shell);
        	expander(&shell);
        	parser(&shell);
        	exec_start(&shell);
			if (g_signal)
            	shell.exit_status = 130;
			/* if (g_signal == 130)
			{
				//shell.exit_status = 130;
				//free(shell.cmdline);
				continue ;
			} */
		}
        cleanup(&shell);
		//rl_on_new_line();  // Assurer que readline est prêt pour la prochaine entrée
    }
    
    if (shell.env)
    {
        free_env(shell.env);
        shell.env = NULL;
    }
    if (shell.cmdline)
    {
        free(shell.cmdline);
        shell.cmdline = NULL;
    }
}

int	main(int ac, char **av, char **env)
{
	(void) ac;
	(void) av;
	rl_catch_signals = 0;
	setup_signal_handlers();
	minishell(env);
	return (0);
}
