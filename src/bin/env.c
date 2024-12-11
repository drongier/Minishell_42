/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:32:10 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/11 12:38:46 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_env(t_shell shell)
{
	if (shell.env == NULL)
		ft_error(&shell, "env: empty environment", NULL, 1);
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
