/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 13:28:24 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/17 20:10:23 by chbachir         ###   ########.fr       */
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
	if (!clean_input)
		return (-1);
	fd = open(clean_input, O_RDONLY, 777);
	if (fd == -1)
	{
		ft_error(shell, "%s: No such file or directory\n", clean_input, 1);
		free(clean_input);
		if (shell->flag_pipe == 1)
		{
			parser->infile = -1;
			return (0);
		}
		cleanup(shell);
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

int	handle_token_arg(t_lexer **lexer, t_parser **parser, char *clean_str)
{
	t_list	*node_input;

	if ((*lexer)->type == TOKEN_ARG && (*lexer)->input[0] != '\0')
	{
		node_input = ft_lstnew(ft_strdup((*lexer)->input));
		if (!node_input)
			return (free(clean_str), -1);
		ft_lstadd_back(&(*parser)->args, node_input);
	}
	return (0);
}
