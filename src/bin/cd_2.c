/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:33:44 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/16 15:35:53 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_pwd(t_shell *shell, char *oldpwd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return ;
	env_pop(&shell->env, "OLDPWD");
	env_push(&shell->env, ft_strdup("OLDPWD"), ft_strdup(oldpwd));
	env_pop(&shell->env, "PWD");
	env_push(&shell->env, ft_strdup("PWD"), ft_strdup(pwd));
	free(pwd);
}

void	handle_home_cd(t_shell *shell, char *home)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (chdir(home) != 0)
		ft_error(shell, "bash: cd: %s: "NSFOD"\n", home, 1);
	else
		update_pwd(shell, oldpwd);
	free(oldpwd);
}

void	handle_previous_dir(t_shell *shell)
{
	char	*oldpwd;
	char	*current_pwd;

	oldpwd = ft_getenv(shell, "OLDPWD");
	if (!oldpwd)
		return (ft_error(shell, "bash: cd: OLDPWD not set\n", NULL, 1));
	current_pwd = getcwd(NULL, 0);
	if (chdir(oldpwd) != 0)
	{
		free(current_pwd);
		return (ft_error(shell, "bash: cd: %s: "NSFOD"\n", oldpwd, 1));
	}
	printf("%s\n", oldpwd);
	update_pwd(shell, current_pwd);
	free(current_pwd);
}

void	handle_tilde_path(t_shell *shell, char *home, char *clean_arg)
{
	char	*full_path;
	char	*oldpwd;

	full_path = malloc(strlen(home) + strlen(clean_arg));
	if (!full_path)
		return (ft_error(shell, "malloc\n", NULL, 1), free(clean_arg));
	ft_strcpy(full_path, home);
	if (ft_strlen(clean_arg) > 1)
		ft_strcat(full_path, clean_arg + 1);
	oldpwd = getcwd(NULL, 0);
	if (chdir(full_path) != 0)
		ft_error(shell, "bash: cd: %s: "NSFOD"\n", full_path, 1);
	else
		update_pwd(shell, oldpwd);
	free(oldpwd);
	free(full_path);
	free(clean_arg);
}

void	handle_regular_path(t_shell *shell, char *clean_arg)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (chdir(clean_arg) != 0)
		ft_error(shell, "bash: cd: %s: "NSFOD"\n", clean_arg, 1);
	else
		update_pwd(shell, oldpwd);
	free(oldpwd);
	free(clean_arg);
}
