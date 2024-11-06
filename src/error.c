/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:13:52 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/06 12:15:09 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error(t_shell *shell, char *err_msg, char *args, int err_num)
{
	if (!err_msg)
		shell->exit_status = err_num;
	else
	{
		printf(err_msg, args);
		shell->exit_status = err_num;
	}
}
