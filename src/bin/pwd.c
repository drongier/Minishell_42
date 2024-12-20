/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:24:37 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/26 14:12:03 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_pwd(t_shell *shell)
{
	char	*buffer;

	buffer = malloc(sizeof(char) * 4097);
	printf("%s\n", getcwd(buffer, 4097));
	free(buffer);
}
