/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:12:58 by drongier          #+#    #+#             */
/*   Updated: 2024/12/04 17:21:14 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_pipe(t_shell *shell, t_parser **parser, t_lexer **lexer)
{
	t_pipex	pipex;

	if (!*lexer || !(*lexer)->next)
		return (exit_no_lexer_int(shell));
	if (pipe((int *)&pipex) == -1)
	{
		perror("pipe");
		return (0);
	}
	(*parser)->outfile = pipex.write_fd;
	(*parser)->pipex = pipex;
	(*parser)->next = new_cmd_node();
	*parser = (*parser)->next;
	(*parser)->infile = pipex.read_fd;
	return (1);
}