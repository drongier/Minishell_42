/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:36:16 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/18 16:31:27 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_cmdtable(t_shell shell)
{
	t_parser *current_node = shell.parser;
	int cmd_num = 1;

	printf("Command Table:\n");
	while (current_node)
	{
		printf("\n	Command %d:\n", cmd_num++);
		t_parser *cmd = (t_parser *)current_node;
		printf("		infile: %d\n", cmd->infile);
		printf("		outfile: %d\n", cmd->outfile);
		int i = 0;
		if (cmd->args)
		{
			while (cmd->args)
			{
				printf("		Arg[%d]: %s\n", i, (char *)cmd->args->content);
				i++;
				cmd->args = cmd->args->next;
			}
		}
		else
			printf("  No command arguments.\n");
		current_node = current_node->next;
	}
	printf("------------------------------------------------\n");
}

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
			handle_heredoc(parser, lexer->input);
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
