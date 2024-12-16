/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:51:03 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/16 13:16:18 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	replace_variable_in_input(t_lexer *lexer, \
							int start, int end, char *var_value)
{
	char	*new_input;

	new_input = build_new_input(lexer->input, start, end, var_value);
	if (!new_input)
		return (0);
	free(lexer->input);
	lexer->input = new_input;
	return (1);
}

void	free_var_value_if_needed(const char *input, int i, char *var_value)
{
	if (!is_special_variable(input, i))
		free(var_value);
}

int	expand_variable_at(t_shell *shell, t_lexer *lexer, int *i)
{
	int		end;
	char	*var_value;

	var_value = get_var_value(shell, lexer->input, *i, &end);
	if (!var_value)
		return (0);
	if (!replace_variable_in_input(lexer, *i, end, var_value))
	{
		free_var_value_if_needed(lexer->input, *i, var_value);
		return (0);
	}
	*i += ft_strlen(var_value);
	free_var_value_if_needed(lexer->input, *i, var_value);
	return (1);
}

int	process_lexer_input(t_shell *shell, t_lexer *lexer)
{
	int		i;
	int		single_quote;

	i = 0;
	single_quote = 0;
	while (lexer->input[i])
	{
		if (lexer->input[i] == '\'')
			single_quote = !single_quote;
		if (lexer->input[i] == '$' && !single_quote)
		{
			if (!expand_variable_at(shell, lexer, &i))
				return (0);
		}
		else
			i++;
	}
	return (1);
}

void	expander(t_shell *shell)
{
	t_lexer		*lexer;

	lexer = shell->lexer;
	while (lexer)
	{
		if (!process_lexer_input(shell, lexer))
			return ;
		lexer = lexer->next;
	}
}
