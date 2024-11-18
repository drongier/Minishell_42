/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:36:16 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/18 21:18:20 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_parser *new_cmd_node()
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

void parser(t_shell *shell)
{
	t_parser 	*parser;
	t_lexer 	*lexer;
	t_list		*node_input;

	shell->parser = new_cmd_node();
	parser = shell->parser;
	lexer = shell->lexer;
	while (lexer)
	{
		node_input = ft_lstnew(lexer->input);
		if (lexer->type == TOKEN_ARG)
			ft_lstadd_back(&parser->args, node_input);
		else if (lexer->type == TOKEN_REDIR_IN)
		{
			if (!check_error_token_redi(shell))
				return ;
			lexer = lexer->next;
			parser->infile = open(lexer->input, O_RDONLY, 777);
			if (parser->infile == -1)
			{
				printf("open failed");
				return ;
			}
		}
		else if (lexer->type == TOKEN_REDIR_OUT)
		{
			if (!check_error_token_redi(shell))
				return ;
			lexer = lexer->next;
			parser->outfile = open(lexer->input, O_CREAT | O_RDWR | O_TRUNC, 0664);
			if (parser->outfile == - 1)
			{
				printf("1 open failed");
				return ;
			}
		}
		else if (lexer->type == TOKEN_REDIR_APPEND)
		{
			if (!check_error_token_redi(shell))
				return ;
			lexer = lexer->next;
			parser->outfile = open(lexer->input, O_CREAT | O_RDWR | O_APPEND, 0664);
			if (parser->outfile == - 1)
			{
				printf("2 open failed");
				return ;
			}
		}
		else if (lexer->type == TOKEN_REDIR_HEREDOC)
		{
			if (!check_error_token_redi(shell))
				return ;
			lexer = lexer->next;
			handle_heredoc(shell, parser, lexer->input);
			if (parser->infile == -1)
			{
				shell->exit_status = 130;
				return ;
			}
		}
		if (lexer->type == TOKEN_PIPE)
        {
            t_pipex pipex;

            if (pipe((int *)&pipex) == -1) 
            {
                perror("pipe");
                return;
            }
            if (!check_error_token_redi(shell))
                return;
            parser->outfile = pipex.write_fd;
            parser->pipex = pipex;
            parser->next = new_cmd_node();
            parser = parser->next;
            parser->infile = pipex.read_fd;
        }
		lexer = lexer->next;
	}
}
