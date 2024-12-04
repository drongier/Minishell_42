/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:26:44 by drongier          #+#    #+#             */
/*   Updated: 2024/12/04 17:29:16 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_heredoc_redir(t_shell *shell, t_parser *parser, t_lexer **lexer)
{
	if (!*lexer || !(*lexer)->next)
		return (exit_no_lexer_int(shell));
	*lexer = (*lexer)->next;
	handle_heredoc(shell, parser, (*lexer)->input);
	if (parser->infile == -1)
	{
		shell->exit_status = 130;
		return (0);
	}
	return (1);
}