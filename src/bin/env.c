/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:32:10 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/25 14:03:33 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_env(t_shell shell)
{	
	if (shell.env == NULL)
		error(&shell, "env: empty environment", NULL);
	while (shell.env != NULL)
	{
		printf("%s=%s\n", shell.env->key, shell.env->value);
		shell.env = shell.env->next;
		error(&shell, NULL, NULL);
	}
}
