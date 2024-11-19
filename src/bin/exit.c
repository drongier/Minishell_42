/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:51:01 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/06 12:38:12 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_exit(t_shell *shell)
{

	char **args = list_to_array(shell->parser->args);

	if (args[1] && args[2])
	{
		shell->exit_status = 1;
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
	}
	if (shell->parser->args->next)
	{
		error(shell, "exit\n", NULL, 156);
		cleanup(shell);
		//todo = close open FDs
		exit(0);
	}
	else
	{
		error(shell, "exit\n", NULL, 0);
		cleanup(shell);
		exit(0);
	}

}
