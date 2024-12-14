/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 16:34:23 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/14 14:12:35 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Function to free allocated tokens in case of error
void	free_tokens(char **tokens, size_t count)
{
	size_t	k;

	k = 0;
	while (k < count)
	{
		free(tokens[k]);
		k++;
	}
	free(tokens);
}

// Function to extract a token from the string
int	extract_token(const char *str, size_t *i, t_state *state, char **token)
{
	size_t	start;
	size_t	len;

	start = *i;
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
	len = *i - start;
	*token = malloc(len + 1);
	if (!(*token))
		return (0);
	ft_strncpy(*token, &str[start], len);
	(*token)[len] = '\0';
	return (1);
}

// Main function to split the string into tokens
char	**custom_split(const char *str, int *error_flag)
{
	size_t		tokens;
	char		**result;
	size_t		i;
	size_t		j;
	t_state		state;

	tokens = count_tokens(str, error_flag);
	if (*error_flag)
		return (NULL);
	result = allocate_result_array(tokens);
	if (!result)
		return (NULL);
	initialize_parse_state(&i, &j, &state);
	while (str[i])
	{
		skip_spaces(str, &i);
		if (str[i] == '\0')
			break ;
		if (!extract_token(str, &i, &state, &result[j]))
			return (free_tokens(result, j), NULL);
		j++;
	}
	result[j] = NULL;
	return (result);
}
