/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:36:16 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/05 18:44:54 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parser(t_shell *shell)
{
	t_parser	*parser;
	t_lexer		*lexer;
	t_list		*node_input;
	char		*clean_str;

	shell->parser = new_cmd_node();
	parser = shell->parser;
	lexer = shell->lexer;
	while (lexer)
	{
		clean_str = remove_quotes((char *)lexer->input);
		node_input = ft_lstnew(lexer->input);
		if (lexer->type == TOKEN_ARG)
			ft_lstadd_back(&parser->args, node_input);
		if (lexer->type == TOKEN_REDIR_IN)
			if (!handle_in_redir(shell, &parser, &lexer))
				return ;
		if (lexer->type == TOKEN_REDIR_OUT)
			if (!handle_out_redir(shell, &parser, &lexer))
				return ;
		if (lexer->type == TOKEN_REDIR_APPEND)
			if (!handle_append_redir(shell, &parser, &lexer))
				return ;
		if (lexer->type == TOKEN_REDIR_HEREDOC)
			if (!handle_heredoc_redir(shell, parser, &lexer))
				return ;
		if (lexer->type == TOKEN_PIPE)
			if (!handle_pipe(shell, &parser, &lexer))
				return ;
		lexer = lexer->next;
		free(clean_str);
	}
	free(lexer);
}
