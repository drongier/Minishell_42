/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 16:32:52 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/13 17:10:09 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Function to skip spaces in the string
void	skip_spaces(const char *str, size_t *i)
{
	while (str[*i] == ' ')
		(*i)++;
}

// Function to process a token during counting
void	process_token(const char *str, size_t *i, t_state *state)
{
	while (str[*i] && (state->in_quotes || str[*i] != ' '))
	{
		if (!state->in_quotes && (str[*i] == '"' || str[*i] == '\''))
		{
			state->in_quotes = 1;
			state->quote_char = str[*i];
		}
		else if (state->in_quotes && str[*i] == state->quote_char)
		{
			state->in_quotes = 0;
			state->quote_char = '\0';
		}
		(*i)++;
	}
}

// Function to count the number of tokens in the string
size_t	count_tokens(const char *str, int *error_flag)
{
	size_t		count;
	size_t		i;
	t_state		state;

	count = 0;
	i = 0;
	state.in_quotes = 0;
	state.quote_char = '\0';
	*error_flag = 0;
	while (str[i])
	{
		skip_spaces(str, &i);
		if (str[i] == '\0')
			break ;
		count++;
		process_token(str, &i, &state);
	}
	if (state.in_quotes)
		*error_flag = 1;
	return (count);
}

// Function to allocate the result array
char	**allocate_result_array(size_t tokens)
{
	char	**result;

	result = malloc(sizeof(char *) * (tokens + 1));
	return (result);
}

// Function to initialize parsing indices and state
void	initialize_parse_state(size_t *i, size_t *j, t_state *state)
{
	*i = 0;
	*j = 0;
	state->in_quotes = 0;
	state->quote_char = '\0';
}
