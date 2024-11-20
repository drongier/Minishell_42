/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:51:01 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/20 15:00:40 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	exit_val_isdigit(char *s)
{
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (0);
		s++;
	}
	return 1;
}

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
		printf("1 ex value %d\n", exit_value);
		if (!exit_val_isdigit((const char *)args->next->content))
		{
			error(shell, "minishell: exit: numeric argument required\n", (char *)args->next->content, 2);
			cleanup(shell);
			exit(1);
		}
		else
		{
			printf("2 ex value %d\n", exit_value);
			if (exit_value > 256)
				exit_value = exit_value % 256;
			printf("3 ex value %d\n", exit_value);
			cleanup(shell);
			exit(exit_value);
		}
	}
	/*//if (nb_args == 2 && !ft_isdigit(ft_atoi()))
	printf(":::: `%s`\n", (char *)shell->parser->args->next->content);
	
	error(shell, NULL, NULL, 0);
	cleanup(shell);
	//printf("\033[2J\033[1;1H");
	//todo = close open FDs
	exit(0);*/
}
