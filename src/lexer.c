/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 13:54:47 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/25 11:56:50 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	push(t_lexer **lexer, char *input, t_token_type type, size_t pos)
{
	t_lexer	*last;
	t_lexer	*token;

	token = malloc(sizeof(t_lexer));
	if (!token)
		return (0);
	token->input = ft_strdup(input);
	if (!token->input)
	{
		free(token);
		return (0); 
	}
	token->type = type;
	token->pos = pos;
	token->next = NULL;
	if (*lexer == NULL)
	{
		*lexer = token;
		return (1);
	}
	last = *lexer;
	while (last->next != NULL)
		last = last->next;
	last->next = token;
	return (1);
}

void	free_split_res(char **split_res)
{
	int i;

	i = 0;
	while (split_res[i])
	{
		free(split_res[i]);
		i++;
	}
	free(split_res);
}

void lexer(t_shell *shell)
{
    size_t i;
    size_t pos;
    char **str;
    int error_flag = 0;

    str = custom_split(shell->cmdline, &error_flag); // Utilisation de custom_split avec gestion des erreurs
    if (!str)
    {
        if (error_flag)
            error(shell, "Lexer: Mismatched quotes detected\n", NULL, 1);
        else
            error(shell, "Lexer: Memory allocation failed\n", NULL, 1);
        return;
    }
    i = 0;
    pos = 0;
    shell->lexer = NULL;
    while (str[i] != NULL)
    {
		if (ft_strncmp(str[i], "|", 1) == 0)
		{
			shell->flag_pipe = 1;
			push(&shell->lexer, "|", TOKEN_PIPE, pos);
		}
		else if (ft_strncmp(str[i], "<<", 2) == 0)
			push(&shell->lexer, "<<", TOKEN_REDIR_HEREDOC, pos);
		else if (ft_strncmp(str[i], ">>", 2) == 0)
			push(&shell->lexer, ">>", TOKEN_REDIR_APPEND, pos);
		else if (ft_strncmp(str[i], "<", 1) == 0)
			push(&shell->lexer, "<", TOKEN_REDIR_IN, pos);
        else if (ft_strncmp(str[i], ">", 1) == 0)
            push(&shell->lexer, ">", TOKEN_REDIR_OUT, pos);
        else
            push(&shell->lexer, str[i], TOKEN_ARG, pos);
        pos++;
        i++;
    }
    free_split_res(str);
}

