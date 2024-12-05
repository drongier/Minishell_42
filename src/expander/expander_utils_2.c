/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 12:22:54 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/05 12:51:35 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_empty_var(void)
{
	char	*var_value;

	var_value = ft_strdup("$");
	return (var_value);
}

char	*extract_variable_name(const char *input, int start, int end)
{
	return (ft_substr(input, start, end - start));
}

char	*handle_reg_var(t_shell *shell, const char *input, int i, int *end)
{
	int		start;
	char	*var_value;
	char	*var_name;

	start = i + 1;
	*end = get_variable_end(input, start);
	var_name = extract_variable_name(input, start, *end);
	if (!var_name)
		return (NULL);
	if (ft_strlen(var_name) == 0)
	{
		free(var_name);
		var_value = handle_empty_var();
		if (!var_value)
			return (NULL);
	}
	else
	{
		var_value = get_regular_var_value(shell, var_name);
		free(var_name);
		if (!var_value)
			return (NULL);
	}
	return (var_value);
}

char	*get_var_value(t_shell *shell, const char *input, int i, int *end)
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

char	*build_new_input(char *input, int start, int end, char *var_value)
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
		free(start_str);
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
