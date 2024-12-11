/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:36:16 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/11 12:43:16 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	create_new_pipe(t_shell *shell, t_parser **parser, t_lexer *lexer)
{
	t_pipex	pipex;

	if (!lexer || !lexer->next)
		return (check_redir_error(shell));
	if (pipe((int *)&pipex) == -1)
	{
		perror("pipe");
		return (-1);
	}
	(*parser)->outfile = pipex.fd[1];
	(*parser)->pipex = pipex;
	(*parser)->next = new_cmd_node();
	*parser = (*parser)->next;
	(*parser)->infile = pipex.fd[0];
	return (0);
}

int	process_redirection(t_shell *shell, t_parser *parser, t_lexer **lexer)
{
	int	status;

	if ((*lexer)->type == TOKEN_REDIR_IN)
		status = handle_input_redir(shell, parser, lexer);
	else if ((*lexer)->type == TOKEN_REDIR_OUT)
		status = handle_output_redirection(shell, parser, lexer, \
										O_CREAT | O_RDWR | O_TRUNC);
	else if ((*lexer)->type == TOKEN_REDIR_APPEND)
		status = handle_output_redirection(shell, parser, lexer, \
										O_CREAT | O_RDWR | O_APPEND);
	else if ((*lexer)->type == TOKEN_REDIR_HEREDOC)
		status = handle_heredoc_redirection(shell, parser, lexer);
	else
		status = 0;
	return (status);
}

static int	process_pipe(t_shell *shell, t_parser **parser, t_lexer *lexer)
{
	return (create_new_pipe(shell, parser, lexer));
}

static int	handle_token(t_shell *shell, t_parser **parser, t_lexer **lexer)
{
	t_list		*node_input;
	char		*clean_str;

	if (!(*lexer))
		return (-1);
	clean_str = remove_quotes((char *)(*lexer)->input);
	if ((*lexer)->type == TOKEN_ARG)
	{
		node_input = ft_lstnew((*lexer)->input);
		if (node_input)
			ft_lstadd_back(&(*parser)->args, node_input);
	}
	else if (is_redirection((*lexer)->type))
	{
		free(clean_str);
		return (process_redirection(shell, *parser, lexer));
	}
	else if ((*lexer)->type == TOKEN_PIPE)
	{
		free(clean_str);
		return (process_pipe(shell, parser, *lexer));
	}
	free(clean_str);
	return (0);
}

void	parser(t_shell *shell)
{
	t_parser	*parser;
	t_lexer		*lexer;
	int			status;

	shell->parser = new_cmd_node();
	if (!shell->parser)
		return ;
	parser = shell->parser;
	lexer = shell->lexer;
	while (lexer)
	{
		status = handle_token(shell, &parser, &lexer);
		if (status == -1)
			break ;
		lexer = lexer->next;
	}
}

/* 
// old version, celle qui marche mais pas clean
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
					if (shell->flag_pipe == 1)
					{
						perror(clean_next_input);
						shell->exit_status = 1;
						shell->parser->args = NULL;
						shell->parser->infile = STDIN_FILENO;
						if (clean_next_input)
            				free(clean_next_input);
						lexer = lexer->next;
						continue;
					}
					else 
					{
						perror(clean_next_input);
						cleanup(shell);
						shell->exit_status = 1;
						return ;
					}
				}
				free(clean_next_input);
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
			char *clean_next_input = remove_quotes((char *)lexer->input);
			parser->outfile = open(clean_next_input, O_CREAT | O_RDWR | O_TRUNC, 0664);
			if (parser->outfile == - 1)
			{
				perror("open failed");
				cleanup(shell);
				shell->exit_status = 1;
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
			char *clean_next_input = remove_quotes((char *)lexer->input);
			parser->outfile = open(clean_next_input, O_CREAT | O_RDWR | O_APPEND, 0664);
			if (parser->outfile == - 1)
			{
				perror("open failed");
				cleanup(shell);
				shell->exit_status = 1;
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
		free(clean_str);
	}
	free(lexer);
} */
