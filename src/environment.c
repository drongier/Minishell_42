/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:39:05 by emaydogd          #+#    #+#             */
/*   Updated: 2024/10/14 12:02:21 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_push(t_env **env, char *key, char *value)
{
	t_env	*last;
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (0);
	node->key = key;
	node->value = value;
	node->next = NULL;
	if (*env == NULL)
		return (*env = node, 1);
	last = *env;
	while (last->next != NULL)
		last = last->next;
	return (last->next = node, 1);
}
int	env_pop(t_env **env, char *key)
{
	t_env 	*prev;
	t_env	*tmp;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (ft_strncmp(key, tmp->key, ft_strlen(tmp->key)) == 0)
		{
			if (!prev)
				*env = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp);
			return (1);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (0);
}

void	init_env(t_shell *shell, char **env)
{
	int		i;
	int		j;
	char	*key;
	char	*val;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j])
		{
			if (env[i][j] == '=')
				break ;
			j++;
		}
		key = ft_substr(env[i], 0, j);
		val = ft_substr(env[i], j + 1, ft_strlen(env[i]) - j + 1);
		env_push(&shell->env, key, val);
		i++;
	}
}

char *ft_getenv(t_shell *shell, char *key)
{
	t_env *tmp;

	tmp = shell->env;
	while (tmp)
	{
		if (ft_strcmp(key, tmp->key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
