/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 12:14:31 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/14 16:08:30 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_var_char(char c)
{
	return ((c >= 'A' && c <= 'Z') || \
			(c >= 'a' && c <= 'z') || \
			(c >= '0' && c <= '9') || \
			(c == '_'));
}

int	is_special_variable(const char *input, int i)
{
	return (input[i + 1] == '?');
}

int	get_variable_end(const char *input, int start)
{
	int	j;

	j = start;
	while (input[j] && input[j] != ' ' && \
			input[j] != '"' && input[j] != '\'' && \
			is_var_char(input[j]))
		j++;
	return (j);
}

char	*get_regular_var_value(t_shell *shell, char *var_name)
{
	char	*env_value;
	char	*var_value;

	env_value = ft_getenv(shell, var_name);
	if (env_value)
		var_value = ft_strdup(env_value);
	else
		var_value = ft_strdup("");
	return (var_value);
}
