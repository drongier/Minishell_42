/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:51:01 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/20 22:44:30 by chbachir         ###   ########.fr       */
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
	int exit_value;
	t_list *args;
	t_list *args_to_count;
	
	args = shell->parser->args;
	args_to_count = shell->parser->args;
	printf("%s\n", (const char *)args->next->content);

	nb_args = ft_lstsize(args_to_count);
	printf("nb args %d\n", nb_args);
	
	printf("exit\n");
	if (nb_args > 2)
	{
		error(shell, "minishell: exit: too many arguments\n", NULL, 1);
		cleanup(shell);
		exit(1);
	}
	else if (nb_args == 2)
	{
		if (exit_val_isdigit((char *)args->next->content))
		{
			exit_value = ft_atoi((const char *)args->next->content);
			printf("exit value %d\n", exit_value);
			if (exit_value > 256)
				exit_value = exit_value % 256;
			printf("exit value %d\n", exit_value);
			cleanup(shell);
			exit(exit_value);
		}
		else
		{
			error(shell, "minishell: exit: numeric argument required\n", (char *)args->next->content, 2);
			cleanup(shell);
			exit(1);
		}
	}
	//error(shell, NULL, NULL, 0);
	//cleanup(shell);
	//printf("\033[2J\033[1;1H");
	//todo = close open FDs
	//exit(0);*/
}
