/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:24:37 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/10 14:20:48 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_pwd(t_shell *shell)
{
	char	*buffer;
	char	*pwd;

	buffer = malloc(sizeof(char) * 4097);
	pwd = getcwd(buffer, 4097);
	write(shell->parser->outfile, pwd, ft_strlen(pwd));
	write(shell->parser->outfile, "\n", 1);
	free(buffer);
}
