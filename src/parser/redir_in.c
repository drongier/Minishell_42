/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:50:44 by drongier          #+#    #+#             */
/*   Updated: 2024/12/05 17:49:57 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	failed_open_pipe(t_shell *shell, char *clean_next_input)
{
	perror(clean_next_input);
	shell->exit_status = 1;
	shell->parser->args = NULL;
	shell->parser->infile = STDIN_FILENO;
	if (clean_next_input)
		free(clean_next_input);
}

void	exit_redir_in_open_failed(t_shell *shell, char *clean_next_input)
{
	perror(clean_next_input);
	cleanup(shell);
	shell->exit_status = 1;
}

int	handle_in_redir(t_shell *shell, t_parser **parser, t_lexer **lexer)
{
	char	*cl_input;

	if (!*lexer || !(*lexer)->next)
		return (exit_no_lexer_int(shell));
	cl_input = remove_quotes((char *)(*lexer)->next->input);
	*lexer = (*lexer)->next;
	(*parser)->infile = open(cl_input, O_RDONLY, 777);
	if ((*parser)->infile == -1)
	{
		if (shell->flag_pipe == 1)
		{
			failed_open_pipe(shell, cl_input);
			*lexer = (*lexer)->next;
			return (1);
		}
		else
		{
			exit_redir_in_open_failed(shell, cl_input);
			return (0);
		}
	}
	free(cl_input);
	return (1);
}
