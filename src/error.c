/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:13:52 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/26 14:55:04 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     check_error_token_redi(t_shell *shell)
{
        t_lexer *lexer;

        lexer = shell->lexer;
        if (!lexer->next)
        {
			ft_error(shell, "bash: syntax error near unexpected token `newline'\n", NULL, -1);
            return (0);
        }
        else
            return (1);
}

void	ft_error(t_shell *shell, char *err_msg, char *args, int exit_status)
{
	int i = 0;
	while (err_msg && err_msg[i])
	{
		if (err_msg[i] == '%' && err_msg[i + 1] == 's')
		{
			if (args)
			{
				ft_putstr_fd(args, STDERR_FILENO);
				i += 2; // je saute `%s`
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
