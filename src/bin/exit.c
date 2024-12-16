/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:51:01 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/16 13:13:00 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

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
	long	exit_val;

	arg = remove_quotes((char *)shell->parser->args->next->content);
	if (!exit_val_isdigit(arg))
	{
		ft_error(shell, ""M"\n", arg, -1);
		free(arg);
		return (2);
	}
	exit_val = ft_atol(arg);
	if (exit_val > INT_MAX || exit_val < INT_MIN)
	{
		ft_error(shell, ""M"\n", arg, -1);
		free(arg);
		return (2);
	}
	if (exit_val > 256)
		exit_val = exit_val % 256;
	free(arg);
	return ((int)exit_val);
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
	if (ft_lstsize(shell->parser->args) > 2)
	{
		ft_error(shell, "minishell: exit: too many arguments\n", NULL, 1);
		return ;
	}
	else
		exit_val = handle_exit_value(shell);
	cleanup(shell);
	exit(exit_val);
}
