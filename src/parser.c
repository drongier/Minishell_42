/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:36:16 by emaydogd          #+#    #+#             */
/*   Updated: 2024/10/15 14:12:50 by chbachir         ###   ########.fr       */
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
	cmd->infile = STDIN_FILENO;   // Default to standard input
	cmd->outfile = STDOUT_FILENO; // Default to standard output
	cmd->next = NULL;
	return (cmd);
}

void handle_heredoc(t_parser *parser, const char *delimiter) 
{
	char *line = NULL;
    int pipefd[2];

    if (pipe(pipefd) == -1) 
    {
        perror("pipe");
        return;
    }
    while (1) 
    {
        line = readline("heredoc> ");
        if (!line)
            break;
        if (strcmp(line, delimiter) == 0) 
        {
            free(line);
            break;
        }
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]);
    parser->infile = pipefd[0];
}

void parser(t_shell *shell)
{
	t_parser 	*parser;
	t_lexer 	*lexer;
	t_list		*node_input;
	int			pipefd[2];

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
			if (!check_error_token_redi(shell))
				return ;
			parser->outfile = pipefd[1];
			parser->next = new_cmd_node();
			parser = parser->next;
			parser->infile = pipefd[0];
		}
		lexer = lexer->next;
	}
	
}
