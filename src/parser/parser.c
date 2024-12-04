/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:36:16 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/04 17:40:10 by drongier         ###   ########.fr       */
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
		else if (lexer->type == TOKEN_REDIR_IN)
		{
			if (!lexer || !lexer->next)
				return (exit_no_lexer(shell));
			lexer = lexer->next;
			char *cl_input = remove_quotes((char *)lexer->input);
			parser->infile = open(cl_input, O_RDONLY, 777);
			if (parser->infile == -1)
			{
				if (shell->flag_pipe == 1)
				{
					failed_open_pipe(shell, cl_input);
					lexer = lexer->next;
					continue ;
				}
				else
				{
					exit_redir_in_open_failed(shell, cl_input);
					return ;
				}
			}
			free(cl_input);
		}
		else if (lexer->type == TOKEN_REDIR_OUT)
		{
			if (!lexer || !lexer->next)
				return (exit_no_lexer(shell));
			lexer = lexer->next;
			char *cl_input = remove_quotes((char *)lexer->input);
			parser->outfile = open(cl_input, O_CREAT | O_RDWR | O_TRUNC, 0664);
			if (parser->outfile == -1)
				return (exit_failed_open(shell));
		}
		else if (lexer->type == TOKEN_REDIR_APPEND)
		{
			char	*cl_input;
			
			if (!lexer || !lexer->next)
				return (exit_no_lexer(shell));
			lexer = lexer->next;
			cl_input = remove_quotes((char *)lexer->input);
			parser->outfile = open(cl_input, O_CREAT | O_RDWR | O_APPEND, 0664);
			if (parser->outfile == -1)
				return (exit_failed_open(shell));
		}
		else if (lexer->type == TOKEN_REDIR_HEREDOC)
		{
			if (!handle_heredoc_redir(shell, parser, &lexer))
				return ;
		}
		if (lexer->type == TOKEN_PIPE)
			if (!handle_pipe(shell, &parser, &lexer))
				return ;
		lexer = lexer->next;
		free(clean_str);
	}
	free(lexer);
}
