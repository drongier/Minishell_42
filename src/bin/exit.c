/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:51:01 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/03 11:40:58 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	exit_val_isdigit(char *s)
{
	while (*s)
	{
		if (*s == '+' || *s == '-')
			s++;
		if (!ft_isdigit(*s))
			return (0);
		s++;
	}
	return (1);
}

static int	handle_exit_value(t_shell *shell)
{
	char	*arg;
	int		exit_val;

	arg = remove_quotes((char *)shell->parser->args->next->content);
	if (!exit_val_isdigit(arg))
	{
		ft_error(shell, ""M"\n", arg, -1);
		return (2);
	}
	exit_val = ft_atoi(arg);
	if (exit_val > 256)
		exit_val = exit_val % 256;
	free(arg);
	return (exit_val);
}

void	exec_exit(t_shell *shell)
{
	int	exit_val;

	printf("exit\n");
	if (!shell->parser->args->next)
	{
		cleanup(shell);
		exit(shell->exit_status);
	}
	exit_val = handle_exit_value(shell);
	if (ft_lstsize(shell->parser->args) > 2)
	{
		ft_error(shell, "minishell: exit: too many arguments\n", NULL, 1);
		return ;
	}
	cleanup(shell);
	exit(exit_val);
}
