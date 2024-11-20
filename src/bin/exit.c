/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:51:01 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/20 13:05:28 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_exit(t_shell *shell)
{
	int nb_args;
	t_list *args;
	int exit_value;
	
	args = shell->parser->args;
	nb_args = ft_lstsize(args);
	printf("exit\n");
	if (nb_args > 2)
	{
		error(shell, "minishell: exit: too many arguments\n", NULL, 1);
		cleanup(shell);
		exit(1);
		
	}
	if (nb_args == 2)
	{
		exit_value = ft_atoi((const char *)args->next->content);
		if (!ft_isdigit(exit_value))
		{
			error(shell, "minishell: exit: %s: numeric argument required\n", args->next->content, 2);
		}
		else
		{
			
		}
	}
	//if (nb_args == 2 && !ft_isdigit(ft_atoi()))
	printf(":::: `%s`\n", (char *)shell->parser->args->next->content);
	
	error(shell, NULL, NULL, 0);
	cleanup(shell);
	//printf("\033[2J\033[1;1H");
	//todo = close open FDs
	exit(0);
}
