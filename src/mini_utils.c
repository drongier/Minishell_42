/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:00:18 by drongier          #+#    #+#             */
/*   Updated: 2024/12/09 14:01:00 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_no_cmdline(t_shell *shell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	clear_history();
	cleanup(shell);
}

void	update_status_g(t_shell *shell)
{
	shell->exit_status = 130;
	g_signal = 0;
}