/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:32:10 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/12 23:31:55 by chbachir         ###   ########.fr       */
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

void    exec_export(t_shell *shell)
{
    int     i;
    char    *key;
    char    *val;
    char    *tmp_val;
    t_list  *current_arg;

    current_arg = shell->parser->args;
    while (current_arg)
    {
        i = 0;
        while (((char *)current_arg->content)[i])
        {
            if (((char *)current_arg->content)[i] == '=')
                break;
            i++;
        }
        
        key = ft_substr(current_arg->content, 0, i);
        if (!is_valid_key(shell, key))
        {
            free(key);
            break;
        }
        
        val = ft_substr(current_arg->content, i + 1,
            ft_strlen(current_arg->content) - i - 1);
        tmp_val = val;
        val = remove_quotes_export(val);
        
        if (tmp_val != val) // Si une nouvelle chaîne a été créée
            free(tmp_val);
            
        if (ft_getenv(shell, key))
            env_pop(&shell->env, key);
            
        if (val && val[0])
            env_push(&shell->env, key, val);
        else
        {
            free(key);
            free(val);
        }
        
        current_arg = current_arg->next;
    }
}
