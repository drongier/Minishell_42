/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:32:10 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/26 14:12:12 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_unset(t_shell *shell)
{
	while (shell->parser->args)
	{	
		env_pop(&shell->env, shell->parser->args->content);
		shell->parser->args = shell->parser->args->next;
	}
}
