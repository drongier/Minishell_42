/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:32:10 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/13 12:16:19 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_key(t_shell *shell, char *key)
{
	int	i;

	i = 0;
	if (ft_isalpha(key[i]) == 0 && key[i] != '_')
	{
		ft_error(shell, "bash: export: `%s': not a valid identifier\n", \
		(char *)shell->parser->args->content, 1);
		return (0);
	}
	i++;
	while (key[i])
	{
		if (ft_isalnum(key[i]) == 0 && key[i] != '_')
		{
			ft_error(shell, "bash: export: `%s': not a valid identifier\n", \
			(char *)shell->parser->args->content, 1);
			return (0);
		}
		i++;
	}
	return (1);
}

static char	*remove_quotes_export(char *val)
{
	size_t	len;
	char	*new_val;

	len = ft_strlen(val);
	if ((val[0] == '"' || val[0] == '\'') && val[len - 1] == val[0])
	{
		new_val = ft_substr(val, 1, len - 2);
		return (new_val);
	}
	return (val);
}

void	update_environment(t_shell *shell, char *key, char *val)
{
	if (ft_getenv(shell, key))
		env_pop(&shell->env, key);
	if (val && val[0])
		env_push(&shell->env, key, val);
	else
	{
		free(key);
		free(val);
	}
}

void	process_key_value(t_shell *shell, char *content)
{
	int		i;
	char	*key;
	char	*val;
	char	*tmp_val;

	i = 0;
	while (content[i])
	{
		if (content[i] == '=')
			break ;
		i++;
	}
	key = ft_substr(content, 0, i);
	if (!is_valid_key(shell, key))
	{
		free(key);
		return ;
	}
	val = ft_substr(content, i + 1, ft_strlen(content) - i - 1);
	tmp_val = val;
	val = remove_quotes_export(val);
	if (tmp_val != val)
		free(tmp_val);
	update_environment(shell, key, val);
}

void	exec_export(t_shell *shell)
{
	t_list	*current_arg;

	current_arg = shell->parser->args;
	while (current_arg)
	{
		process_key_value(shell, (char *)current_arg->content);
		current_arg = current_arg->next;
	}
}
