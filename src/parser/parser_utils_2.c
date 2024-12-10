/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 13:28:24 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/10 11:16:20 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_input_redir(t_shell *shell, t_parser *parser, t_lexer **lexer)
{
	char	*clean_input;
	int		fd;

	if (!(*lexer) || !(*lexer)->next)
		return (check_redir_error(shell));
	*lexer = (*lexer)->next;
	clean_input = remove_quotes((char *)(*lexer)->input);
	fd = open(clean_input, O_RDONLY, 777);
	if (fd == -1)
	{
		if (shell->flag_pipe == 1)
		{
			reset_p_pipe_error(shell, parser, clean_input);
			*lexer = (*lexer)->next;
			return (0);
		}
		perror(clean_input);
		cleanup(shell);
		shell->exit_status = 1;
		free(clean_input);
		return (-1);
	}
	parser->infile = fd;
	free(clean_input);
	return (0);
}

int	handle_output_redirection(t_shell *shell, t_parser *parser, \
									t_lexer **lexer, int flags)
{
	char	*clean_input;
	int		fd;

	if (!(*lexer) || !(*lexer)->next)
		return (check_redir_error(shell));
	*lexer = (*lexer)->next;
	clean_input = remove_quotes((char *)(*lexer)->input);
	fd = open_file(shell, clean_input, flags, 0664);
	if (fd == -1)
		return (-1);
	parser->outfile = fd;
	return (0);
}

int	handle_heredoc_redirection(t_shell *shell, \
				t_parser *parser, t_lexer **lexer)
{
	if (!(*lexer) || !(*lexer)->next)
		return (check_redir_error(shell));
	*lexer = (*lexer)->next;
	handle_heredoc(parser, (*lexer)->input);
	if (parser->infile == -1)
	{
		shell->exit_status = 130;
		return (-1);
	}
	return (0);
}
