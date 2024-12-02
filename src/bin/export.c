/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:32:10 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/02 18:39:51 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int is_valid_key(char *key)
{
	int i;

	i = 0;
	if (ft_isalpha(key[i]) == 0 && key[i] != '_')
		return (0);
	i++;
	while (key[i])
	{
		if (ft_isalnum(key[i]) == 0 && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	exec_export(t_shell *shell)
{
	int		i;
	int		j;
	char	*key;
	char	*val;

	i = 0;
	while(shell->parser->args)
	{
		j = 0;
		while (((char *)shell->parser->args->content)[j])
		{
			if (((char *)shell->parser->args->content)[j] == '=')
				break ;
			j++;
		}
		key = ft_substr(shell->parser->args->content, 0, j);
		if (!is_valid_key(key))
		{
			ft_error(shell, "bash: export: `%s': not a valid identifier\n", (char *)shell->parser->args->content, 1);
			break ;
			//exit(EXIT_FAILURE);
		}
		val = ft_substr(shell->parser->args->content, j + 1, ft_strlen(shell->parser->args->content) - j - 1); // Correction ici
		// Supprimer les guillemets si présents
		if (val[0] == '"' || val[0] == '\'')
		{
			size_t len = ft_strlen(val);
			if (val[len - 1] == val[0])
			{
				val[len - 1] = '\0';
				val = ft_substr(val, 1, len - 2);
			}
		}
		if (ft_getenv(shell, key))
			env_pop(&shell->env, key);
		if (val[0])
			env_push(&shell->env, key, val);
		shell->parser->args = shell->parser->args->next;
	}
}
