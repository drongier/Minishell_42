/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:36:16 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/12 23:31:33 by chbachir         ###   ########.fr       */
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

static int handle_token(t_shell *shell, t_parser **parser, t_lexer **lexer)
{
    t_list      *node_input;
    char        *clean_str;
    char        *input_copy;

    if (!(*lexer))
        return (-1);
    
    // Faire une copie de l'input avant de le nettoyer
    input_copy = ft_strdup((*lexer)->input);
    if (!input_copy)
        return (-1);
        
    clean_str = remove_quotes(input_copy);
    free(input_copy); // Libérer la copie

    if ((*lexer)->type == TOKEN_ARG)
    {
        node_input = ft_lstnew(ft_strdup((*lexer)->input)); // Utiliser une copie
        if (!node_input)
        {
            free(clean_str);
            return (-1);
        }
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
