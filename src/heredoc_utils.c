/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:42:32 by drongier          #+#    #+#             */
/*   Updated: 2024/12/09 15:43:38 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
		exit(EXIT_FAILURE);
}

void	setup_heredoc_signals(struct sigaction *sa)
{
	sa->sa_handler = heredoc_signal_handler;
	sa->sa_flags = 0;
	sigemptyset(&sa->sa_mask);
	sigaction(SIGINT, sa, NULL);
}
