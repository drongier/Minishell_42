/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:02:35 by emaydogd          #+#    #+#             */
/*   Updated: 2024/10/14 12:33:53 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	minishell(char **env)
{
	t_shell	shell;

	shell.cmdline = NULL;
	shell.env = NULL;
	shell.lexer = NULL;
	shell.parser = NULL;
	shell.exit_status = 0;
	init_env(&shell, env);
	while (1)
	{
 		shell.cmdline = readline("\033[36;1m ➜ minishell$ \033[0m");
		if (!shell.cmdline)
			return ;
		shell.cmdline[ft_strlen(shell.cmdline)] = '\0';
 		add_history(shell.cmdline);
		lexer(&shell);
		expander(&shell);
		//print_lexer(shell); // optional only printing: delete after finish
		parser(&shell);
		//print_cmdtable(shell);
		exec_start(&shell);
		//free(shell.cmdline);
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
