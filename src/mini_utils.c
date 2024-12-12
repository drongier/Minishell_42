/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:00:18 by drongier          #+#    #+#             */
/*   Updated: 2024/12/12 10:59:14 by chbachir         ###   ########.fr       */
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
