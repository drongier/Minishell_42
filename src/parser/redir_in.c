/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:50:44 by drongier          #+#    #+#             */
/*   Updated: 2024/12/04 15:15:26 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	failed_open_pipe(t_shell *shell, char *clean_next_input)
{
	perror(clean_next_input);
	shell->exit_status = 1;
	shell->parser->args = NULL;
	shell->parser->infile = STDIN_FILENO;
	if (clean_next_input)
		free(clean_next_input);
}

void	exit_redir_in_open_failed(t_shell *shell, char *clean_next_input)
{
	perror(clean_next_input);
	cleanup(shell);
	shell->exit_status = 1;
}
