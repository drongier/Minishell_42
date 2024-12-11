/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 13:54:47 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/11 12:41:56 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static int	handle_lexer_error(t_shell *shell, char **str, int error_flag)
{
	if (!str)
	{
		if (error_flag)
			ft_error(shell, "Lexer: Mismatched quotes detected\n", NULL, -1);
		else
			ft_error(shell, "Lexer: Memory allocation failed\n", NULL, -1);
		return (0);
	}
	return (1);
}

static void	identify_token(t_shell *shell, char *token, size_t *pos)
{
	if (ft_strncmp(token, "|", 1) == 0)
	{
		shell->flag_pipe = 1;
		push(&shell->lexer, "|", TOKEN_PIPE, *pos);
	}
	else if (ft_strncmp(token, "<<", 2) == 0)
		push(&shell->lexer, "<<", TOKEN_REDIR_HEREDOC, *pos);
	else if (ft_strncmp(token, ">>", 2) == 0)
		push(&shell->lexer, ">>", TOKEN_REDIR_APPEND, *pos);
	else if (ft_strncmp(token, "<", 1) == 0)
		push(&shell->lexer, "<", TOKEN_REDIR_IN, *pos);
	else if (ft_strncmp(token, ">", 1) == 0)
		push(&shell->lexer, ">", TOKEN_REDIR_OUT, *pos);
	else
		push(&shell->lexer, token, TOKEN_ARG, *pos);
	(*pos)++;
}

void	lexer(t_shell *shell)
{
	size_t	i;
	size_t	pos;
	char	**str;
	int		error_flag;
	char	*new_str;

	error_flag = 0;
	i = 0;
	pos = 0;
	new_str = add_spaces_around_redirection(shell->cmdline);
	str = custom_split(new_str, &error_flag);
	if (!handle_lexer_error(shell, str, error_flag))
		return ;
	shell->lexer = NULL;
	while (str[i])
		identify_token(shell, str[i++], &pos);
	free(new_str);
	free_split_res(str);
}
