/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:13:52 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/13 20:53:09 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_error_token_redi(t_shell *shell)
{
	ft_error(shell, ""SYNT"\n", NULL, 2);
	cleanup(shell);
}

void	ft_error(t_shell *shell, char *err_msg, char *args, int exit_status)
{
	int	i;

	i = 0;
	while (err_msg && err_msg[i])
	{
		if (err_msg[i] == '%' && err_msg[i + 1] == 's')
		{
			if (args)
			{
				ft_putstr_fd(args, STDERR_FILENO);
				i += 2;
			}
		}
		else
		{
			ft_putchar_fd(err_msg[i], STDERR_FILENO);
			i++;
		}
	}
	if (exit_status != -1)
		shell->exit_status = exit_status;
}
