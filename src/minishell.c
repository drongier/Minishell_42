/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:02:35 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/16 17:11:58 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_mini_env(t_shell *shell)
{
	free_env(shell->env);
	shell->env = NULL;
}

static void	init_shell(t_shell *shell)
{
	shell->cmdline = NULL;
	shell->env = NULL;
	shell->lexer = NULL;
	shell->parser = NULL;
	shell->exit_status = 0;
	shell->in_heredoc = 0;
	shell->flag_pipe = 0;
}

static void	run_minishell(t_shell *shell)
{
	lexer(shell);
	expander(shell);
	parser(shell);
	exec_start(shell);
	cleanup(shell);
}

static void	minishell(char **env)
{
	t_shell		shell;

	init_shell(&shell);
	init_env(&shell, env);
	while (1)
	{
		setup_signal_handlers();
		shell.cmdline = readline("\033[36;1m ➜ minishell$ \033[0m");
		if (!shell.cmdline)
		{
			clean_no_cmdline(&shell);
			break ;
		}
		if (shell.cmdline && shell.cmdline[0] != '\0')
			add_history(shell.cmdline);
		if (g_signal != 0)
			update_status_g(&shell);
		run_minishell(&shell);
		free(shell.cmdline);
		shell.cmdline = NULL;
	}
	if (shell.env)
		free_mini_env(&shell);
}

int	main(int ac, char **av, char **env)
{
	(void) ac;
	(void) av;
	minishell(env);
	return (0);
}
