/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:32:10 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/13 15:39:49 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_unset(t_shell *shell)
{
	t_list	*current_arg;
	t_list	*next_arg;

	current_arg = shell->parser->args;
	while (current_arg)
	{
		env_pop(&shell->env, (char *)current_arg->content);
		next_arg = current_arg->next;
		free(current_arg->content);
		free(current_arg);
		current_arg = next_arg;
	}
	shell->parser->args = NULL;
}
