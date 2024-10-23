/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:27:23 by emaydogd          #+#    #+#             */
/*   Updated: 2024/10/12 21:01:26 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_cd(t_shell *shell)
{
	t_parser *parser;
	char	*home;
	
	parser = shell->parser;
	home = ft_getenv(shell,"HOME");
	if (home == NULL)
	{
		error(shell, "bash: cd: HOME not set\n", NULL);
		return ;
	}
	if (ft_lstsize(parser->args) >= 2)
	{
		error(shell, "bash: cd: too many arguments\n", NULL);
		return ;
	}
	if (parser->args == NULL || parser->args->content == NULL || ft_strncmp((char *)parser->args->content, "~", 1) == 0)
	{
    	if (chdir(home) == 0)
        	error(shell, NULL, NULL);
    	else
        	error(shell, "bash: cd: %s: No such file or directory\n", home);
    	return;
	}
	else if (chdir((char  *)parser->args->content) == 0)
		error(shell, NULL, NULL);
	else
		error(shell, "bash: cd: %s: No such file or directory\n", (char *)parser->args->content);
}



