/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:51:01 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/26 14:55:49 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//! TODO = close open FDs !!!!!!!!

static int	exit_val_isdigit(char *s)
{
	while (*s)
	{
		if(*s == '+' || *s == '-')
			s++;
		if (!ft_isdigit(*s))
		{
			return (0);
		}
		s++;
	}
	return (1);
}

void	exec_exit(t_shell *shell)
{
	int exit_value;
	char * cleaned_exit_code;

	printf("exit\n");
	if (ft_lstsize(shell->parser->args) > 2)
		ft_error(shell, "minishell: exit: too many arguments\n", NULL, 1);
	else if ((ft_lstsize(shell->parser->args) == 2) && (exit_val_isdigit(remove_quotes((char *)shell->parser->args->next->content))))
	{
			exit_value = ft_atoi(remove_quotes((char *)shell->parser->args->next->content));
			if (exit_value > 256)
				exit_value = exit_value % 256;
			cleanup(shell);
			exit(exit_value);
	}
	else
	{
		ft_error(shell, "minishell: exit: numeric argument required\n", (char *)shell->parser->args->next->content, -1);
		cleanup(shell);
		exit(2);
	}
}
