/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:27:23 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/13 20:02:20 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_cd(t_shell *shell, t_list *args)
{
	char	*home;
	char	*clean_arg;

	home = ft_getenv(shell, "HOME");
	if (home == NULL)
		return (ft_error(shell, "bash: cd: HOME not set\n", NULL, 1));
	if (ft_lstsize(args) >= 2)
		return (ft_error(shell, "bash: cd: too many arguments\n", NULL, 1));
	clean_arg = remove_quotes((char *)args->content);
	if (args == NULL || clean_arg == NULL)
	{
		if (chdir(home) != 0)
			ft_error(shell, "bash: cd: %s: "NSFOD"\n", home, 1);
		free(clean_arg);
		return ;
	}
	if (ft_strncmp(clean_arg, "~", 1) == 0)
	{
		if (chdir(home) != 0)
			ft_error(shell, "bash: cd: %s: "NSFOD"\n", home, 1);
		return (free(clean_arg));
	}
	else if (chdir(clean_arg) != 0)
		ft_error(shell, "bash: cd: %s: "NSFOD"\n", clean_arg, 1);
	free(clean_arg);
}
