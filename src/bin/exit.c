/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:51:01 by chbachir          #+#    #+#             */
/*   Updated: 2024/10/12 21:18:28 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_exit(t_shell *shell)
{
	printf("exit\n");
	error(shell, NULL, NULL);
	cleanup(shell);
	//printf("\033[2J\033[1;1H");
	//todo = close open FDs
	exit(0);
}
