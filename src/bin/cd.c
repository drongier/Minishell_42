/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:27:23 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/26 14:45:56 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_cd(t_shell *shell, t_list *args)
{
	char	*home;
	int status;
	pid_t pid;

	home = ft_getenv(shell,"HOME");
	if (home == NULL)
	{
		ft_error(shell, "bash: cd: HOME not set\n", NULL, 1);
		return ;
	}
	if (ft_lstsize(args) >= 2)
	{
		ft_error(shell, "bash: cd: too many arguments\n", NULL, 1);
		return ;
	}
	if (args == NULL || args->content == NULL || ft_strncmp((char *)args->content, "~", 1) == 0)
	{
    	if (chdir(home) != 0)
        	ft_error(shell, "bash: cd: %s: No such file or directory\n", home, 1);
    	return;
	}

	else if (chdir((char  *)args->content) != 0)
		ft_error(shell, "bash: cd: %s: No such file or directory\n", (char *)args->content, 1);
}



