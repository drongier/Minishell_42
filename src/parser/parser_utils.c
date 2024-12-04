/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:15:33 by drongier          #+#    #+#             */
/*   Updated: 2024/12/04 17:20:23 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_parser *new_cmd_node()
{
	t_parser *cmd = malloc(sizeof(t_parser));
	if (!cmd)
		return NULL;
	cmd->args = NULL;
	cmd->infile = STDIN_FILENO;
	cmd->outfile = STDOUT_FILENO;
	cmd->next = NULL;
	return (cmd);
}

int	exit_no_lexer_int(t_shell *shell)
{
	check_error_token_redi(shell);
	shell->parser = NULL;
	shell->exit_status = 2;
	return (0);
}

void	exit_no_lexer(t_shell *shell)
{
	check_error_token_redi(shell);
	shell->parser = NULL;
	shell->exit_status = 2;
}

void	exit_failed_open(t_shell *shell)
{
	perror("open failed");
	cleanup(shell);
	shell->exit_status = 1;
}