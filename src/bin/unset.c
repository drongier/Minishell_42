/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:32:10 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/09 14:14:06 by drongier         ###   ########.fr       */
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
