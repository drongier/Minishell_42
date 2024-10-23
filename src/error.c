/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:13:52 by chbachir          #+#    #+#             */
/*   Updated: 2024/09/25 13:08:53 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error(t_shell *shell, char *err_msg, char *args)
{
	if (!err_msg)
		shell->exit_status = 0;
	else
	{
		printf(err_msg, args);
		shell->exit_status = 1;
	}
}

int	check_error_token_redi(t_shell *shell)
{
	t_lexer *lexer;

	lexer = shell->lexer;

	if (!lexer->next)
	{
		error(shell, "bash: syntax error near unexpected token `newline'\n", NULL);
		return(0);
	}
	else
		return (1);
}
