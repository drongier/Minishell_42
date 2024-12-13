/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:32:10 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/13 19:18:00 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_env(t_shell shell)
{
	if (shell.env == NULL)
		ft_error(&shell, "env: empty environment", NULL, 1);
	if (shell.flag_pipe)
	{
		while (shell.env != NULL)
		{
			write(1, shell.env->key, ft_strlen(shell.env->key));
			write(1, "=", 1);
			write(1, shell.env->value, \
				ft_strlen(shell.env->value));
			write(1, "\n", 1);
			shell.env = shell.env->next;
		}
		exit(0);
	}
	while (shell.env != NULL)
	{
		write(shell.parser->outfile, shell.env->key, ft_strlen(shell.env->key));
		write(shell.parser->outfile, "=", 1);
		write(shell.parser->outfile, shell.env->value, \
			ft_strlen(shell.env->value));
		write(shell.parser->outfile, "\n", 1);
		shell.env = shell.env->next;
	}
}
