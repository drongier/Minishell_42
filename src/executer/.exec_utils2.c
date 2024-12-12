/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:26:58 by drongier          #+#    #+#             */
/*   Updated: 2024/12/11 14:46:14 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_command_path(t_shell *shell, char *cmd_clean, \
							int is_direct_path)
{
	char	*cmd_path;

	if (is_direct_path)
		cmd_path = ft_strdup(cmd_clean);
	else
		cmd_path = get_external_cmd_path(shell, cmd_clean);
	return (cmd_path);
}

char	*create_env_str(t_env *env)
{
	char	*env_str;
	size_t	key_len;
	size_t	val_len;

	key_len = ft_strlen(env->key);
	val_len = ft_strlen(env->value);
	env_str = malloc(sizeof(char) * (key_len + val_len + 2));
	if (!env_str)
		return (NULL);
	ft_strcpy(env_str, env->key);
	env_str[key_len] = '=';
	ft_strcpy(env_str + key_len + 1, env->value);
	return (env_str);
}

int	ft_env_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		env = env->next;
		size++;
	}
	return (size);
}

char	**convert_env_to_array(t_env *env)
{
	char	**envp;
	int		i;

	envp = malloc(sizeof(char *) * (ft_env_size(env) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		envp[i] = create_env_str(env);
		if (!envp[i])
		{
			free_envp(envp, i);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}
