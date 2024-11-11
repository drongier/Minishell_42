/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:27:23 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/11 13:25:05 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_cd(t_shell *shell, t_list *args)
{
	char	*home;
	
	home = ft_getenv(shell,"HOME");
	if (home == NULL)
	{
		error(shell, "bash: cd: HOME not set\n", NULL, 0);
		return ;
	}
	if (ft_lstsize(args) >= 2)
	{
		error(shell, "bash: cd: too many arguments\n", NULL, 1);
		return ;
	}
	if (args == NULL || args->content == NULL || ft_strncmp((char *)args->content, "~", 1) == 0)
	{
    	if (chdir(home) == 0)
        	error(shell, NULL, NULL, 0);
    	else
        	error(shell, "bash: cd: %s: No such file or directory\n", home, 1);
    	return;
	}

	else if (chdir((char  *)args->content) == 0)
		error(shell, NULL, NULL, 0);
	else
		error(shell, "bash: cd: %s: No such file or directory\n", (char *)args->content, 1);
}



