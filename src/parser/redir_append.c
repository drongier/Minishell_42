/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_append.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:19:48 by drongier          #+#    #+#             */
/*   Updated: 2024/12/05 17:53:58 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_append_redir(t_shell *shell, t_parser **parser, t_lexer **lexer)
{
	char	*cl_input;

	cl_input = remove_quotes((char *)(*lexer)->next->input);
	if (!*lexer || !(*lexer)->next)
		return (exit_no_lexer_int(shell));
	*lexer = (*lexer)->next;
	(*parser)->outfile = open(cl_input, O_CREAT | O_RDWR | O_APPEND, 0664);
	if ((*parser)->outfile == -1)
	{
		ft_error(shell, " Permission denied", cl_input, 1);
		cleanup(shell);
		free(cl_input);
		return (0);
	}
	free(cl_input);
	return (1);
}
