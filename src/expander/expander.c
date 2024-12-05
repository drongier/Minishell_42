/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:51:03 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/05 12:52:28 by chbachir         ###   ########.fr       */
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
	free_var_value_if_needed(lexer->input, *i, var_value);
	*i += ft_strlen(var_value);
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

/* 
// working version backup
#include "minishell.h"

static int	is_var_char(char c)
{
	return ((c >= 'A' && c <= 'Z') || \
			(c >= 'a' && c <= 'z') || \
			(c >= '0' && c <= '9') || \
			(c == '_'));
}

static int	is_special_variable(const char *input, int i)
{
	return (input[i + 1] == '?');
}

static int	get_variable_end(const char *input, int start)
{
	int	j;

	j = start;
	while (input[j] && input[j] != ' ' && \
			input[j] != '"' && input[j] != '\'' && \
			is_var_char(input[j]))
		j++;
	return (j);
}

static char	*extract_variable_name(const char *input, int start, int end)
{
	char	*var_name;

	var_name = ft_substr(input, start, end - start);
	if (!var_name)
		return (NULL);
	return (var_name);
}

static char	*handle_reg_var(t_shell *shell, const char *input, int i, int *end)
{
	int		start;
	char	*var_value;
	char	*var_name;
	char	*env_value;

	start = i + 1;
	*end = get_variable_end(input, start);
	var_name = extract_variable_name(input, start, *end);
	if (!var_name)
		return (NULL);
	if (ft_strlen(var_name) == 0)
	{
		free(var_name);
		var_value = ft_strdup("$");
		if (!var_value)
			return (NULL);
	}
	else
	{
		env_value = ft_getenv(shell, var_name);
		if (env_value)
			var_value = ft_strdup(env_value);
		else
			var_value = ft_strdup("");
		free(var_name);
		if (!var_value)
			return (NULL);
	}
	return (var_value);
}

static char	*get_var_value(t_shell *shell, const char *input, int i, int *end)
{
	char	*var_value;

	if (is_special_variable(input, i))
	{
		var_value = ft_itoa(shell->exit_status);
		if (!var_value)
			return (NULL);
		*end = i + 2;
	}
	else
	{
		var_value = handle_reg_var(shell, input, i, end);
		if (!var_value)
			return (NULL);
	}
	return (var_value);
}

static char	*build_new_input(const char *input, int start, int end, const char *var_value)
{
	char	*start_str;
	char	*end_str;
	char	*temp;
	char	*new_input;

	start_str = ft_substr(input, 0, start);
	if (!start_str)
		return (NULL);
	end_str = ft_strdup(&input[end]);
	if (!end_str)
	{
		free (start_str);
		return (NULL);
	}
	temp = ft_strjoin(start_str, var_value);
	free(start_str);
	if (!temp)
	{
		free(end_str);
		return (NULL);
	}
	new_input = ft_strjoin(temp, end_str);
	free(temp);
	free(end_str);
	return (new_input);
}

static int	replace_variable_in_input(t_shell *shell, t_lexer *lexer, \
									int start, int end, const char *var_value)
{
	char	*new_input;

	new_input = build_new_input(lexer->input, start, end, var_value);
	if (!new_input)
		return (0);
	free(lexer->input);
	lexer->input = new_input;
	return (1);
}

static int expand_variable_at(t_shell *shell, t_lexer *lexer, int *i)
{
	int		end;
	char	*var_value;

	var_value = get_var_value(shell, lexer->input, *i, &end);
	if (!var_value)
		return (0);
	if (!replace_variable_in_input(shell, lexer, *i, end, var_value))
	{
		if (!is_special_variable(lexer->input, *i))
			free(var_value);
		return (0);
	}
	if (!is_special_variable(lexer->input, *i))
		free(var_value);
	*i += ft_strlen(var_value);
	return (1);
}

static int	process_lexer_input(t_shell *shell, t_lexer *lexer)
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
} */

