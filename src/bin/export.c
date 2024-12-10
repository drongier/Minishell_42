/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:32:10 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/10 12:50:32 by drongier         ###   ########.fr       */
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

void	exec_export(t_shell *shell)
{
	int		i;
	char	*key;
	char	*val;

	while (shell->parser->args)
	{
		i = 0;
		while (((char *)shell->parser->args->content)[i])
		{
			if (((char *)shell->parser->args->content)[i] == '=')
				break ;
			i++;
		}
		key = ft_substr(shell->parser->args->content, 0, i);
		if (!is_valid_key(shell, key))
			break ;
		val = ft_substr(shell->parser->args->content, i + 1, \
		ft_strlen(shell->parser->args->content) - i - 1);
		val = remove_quotes_export(val);
		if (ft_getenv(shell, key))
			env_pop(&shell->env, key);
		if (val[0])
			env_push(&shell->env, key, val);
		shell->parser->args = shell->parser->args->next;
	}
}
