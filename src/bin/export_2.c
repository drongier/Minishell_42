/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:26:44 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/16 16:54:50 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_in_pipe(t_shell shell)
{
	while (shell.env)
	{
		write(1, "declare -x ", 11);
		write(1, shell.env->key, ft_strlen(shell.env->key));
		write(1, "=", 1);
		write(1, shell.env->value, \
				ft_strlen(shell.env->value));
		write(1, "\n", 1);
		shell.env = shell.env->next;
	}
}

void	write_in_outfile(t_shell shell)
{
	while (shell.env != NULL)
	{
		write(shell.parser->outfile, "declare -x ", 11);
		write(shell.parser->outfile, shell.env->key, ft_strlen(shell.env->key));
		write(shell.parser->outfile, "=", 1);
		write(shell.parser->outfile, shell.env->value, \
			ft_strlen(shell.env->value));
		write(shell.parser->outfile, "\n", 1);
		shell.env = shell.env->next;
	}
}

void	exec_export_no_args(t_shell shell)
{
	if (shell.env == NULL)
		ft_error(&shell, "env: empty environment", NULL, 1);
	if (shell.flag_pipe)
	{
		write_in_pipe(shell);
		exit(0);
	}
	else
		write_in_outfile(shell);
}
