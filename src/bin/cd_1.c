/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:27:23 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/16 15:35:18 by chbachir         ###   ########.fr       */
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
	if (args == NULL)
		return (handle_home_cd(shell, home));
	clean_arg = remove_quotes((char *)args->content);
	if (ft_strcmp(clean_arg, "-") == 0)
		return (handle_previous_dir(shell), free(clean_arg));
	if (ft_strncmp(clean_arg, "~", 1) == 0)
		return (handle_tilde_path(shell, home, clean_arg));
	handle_regular_path(shell, clean_arg);
}
