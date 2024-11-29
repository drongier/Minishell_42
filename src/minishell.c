/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:02:35 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/29 12:41:11 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	minishell(char **env)
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
        setup_signal_handlers();
        shell.cmdline = readline("\033[36;1m ➜ minishell$ \033[0m");
        if (!shell.cmdline)
        {
            write(STDOUT_FILENO, "exit\n", 5);
            break ;
        }
        if (shell.cmdline && shell.cmdline[0] != '\0')
            add_history(shell.cmdline);
        if (g_signal != 0)
        {
            shell.exit_status = 130;
            g_signal = 0;
        }
        lexer(&shell);
        expander(&shell);
        parser(&shell);
        exec_start(&shell);
        cleanup(&shell);
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
	minishell(env);
	return (0);
}
