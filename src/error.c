/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:13:52 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/17 21:42:05 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     check_error_token_redi(t_shell *shell)
{
        t_lexer *lexer;

        lexer = shell->lexer;
        if (!lexer->next)
        {
                ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", STDERR_FILENO);
                return(0);
        }
        else
                return (1);
}

void	error(t_shell *shell, char *err_msg, char *args, int err_num)
{
	if (!err_msg)
		shell->exit_status = err_num;
	else
	{       
                ft_putstr_fd(args, STDERR_FILENO);
                ft_putstr_fd(err_msg, STDERR_FILENO);
		shell->exit_status = err_num;
	}
}
