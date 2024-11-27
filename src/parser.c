/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:36:16 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/26 10:09:11 by chbachir         ###   ########.fr       */
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
			{
				check_error_token_redi(shell);
				shell->parser = NULL;
				shell->exit_status = 2;
				return ;
			}
				lexer = lexer->next;
				char *clean_next_input = remove_quotes((char *)lexer->input);
				parser->infile = open(clean_next_input, O_RDONLY, 777);
				if (parser->infile == -1)
				{
					perror(clean_next_input);
					return ;
				}
		}
		else if (lexer->type == TOKEN_REDIR_OUT)
		{
			if (!lexer || !lexer->next)
			{
				check_error_token_redi(shell);
				shell->parser = NULL;
				shell->exit_status = 2;
				return;
			}
			lexer = lexer->next;
			parser->outfile = open(lexer->input, O_CREAT | O_RDWR | O_TRUNC, 0664);
			if (parser->outfile == - 1)
			{
				perror("open failed");
				return ;
			}
		}
		else if (lexer->type == TOKEN_REDIR_APPEND)
		{
			if (!lexer || !lexer->next)
			{
				check_error_token_redi(shell);
				shell->parser = NULL;
				shell->exit_status = 2;
				return;
			}
			lexer = lexer->next;
			parser->outfile = open(lexer->input, O_CREAT | O_RDWR | O_APPEND, 0664);
			if (parser->outfile == - 1)
			{
				perror("open failed");
				return ;
			}
		}
		else if (lexer->type == TOKEN_REDIR_HEREDOC)
		{
			if (!lexer || !lexer->next)
			{
				check_error_token_redi(shell);
				shell->parser = NULL;
				shell->exit_status = 2;
				return;
			}
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
			
			if (!lexer || !lexer->next)
			{
				check_error_token_redi(shell);
				shell->parser = NULL;
				shell->exit_status = 2;
				return;
			}
            if (pipe((int *)&pipex) == -1) 
            {
                perror("pipe");
                return;
            }
            parser->outfile = pipex.write_fd;
            parser->pipex = pipex;
            parser->next = new_cmd_node();
            parser = parser->next;
            parser->infile = pipex.read_fd;
        }
		lexer = lexer->next;
	}
}
