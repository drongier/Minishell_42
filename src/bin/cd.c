/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:27:23 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/10 11:13:38 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_cd(t_shell *shell, t_list *args)
{
	char	*home;

	home = ft_getenv(shell, "HOME");
	if (home == NULL)
		return (ft_error(shell, "bash: cd: HOME not set\n", NULL, 1));
	if (ft_lstsize(args) >= 2)
		return (ft_error(shell, "bash: cd: too many arguments\n", NULL, 1));
	if (args == NULL || args->content == NULL)
	{
		if (chdir(home) != 0)
			ft_error(shell, "bash: cd: %s: "NSFOD"\n", home, 1);
		return ;
	}
	if (ft_strncmp((char *)args->content, "~", 1) == 0)
	{
		if (chdir(home) != 0)
			ft_error(shell, "bash: cd: %s: "NSFOD"\n", home, 1);
		return ;
	}
	else if (chdir((char *)args->content) != 0)
		ft_error(shell, "bash: cd: %s: "NSFOD"\n", (char *)args->content, 1);
}
