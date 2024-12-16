/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 13:28:24 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/16 16:28:46 by drongier         ###   ########.fr       */
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
			return (-1);
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

	if ((*lexer)->type == TOKEN_ARG)
	{
		node_input = ft_lstnew(ft_strdup((*lexer)->input));
		if (!node_input)
			return (free(clean_str), -1);
		ft_lstadd_back(&(*parser)->args, node_input);
	}
	return (0);
}

// #include "../minishell.h"

// int	handle_input_redir(t_shell *shell, t_parser *parser, t_lexer **lexer)
// {
// 	char	*clean_input;
// 	int		fd;

// 	if (!(*lexer) || !(*lexer)->next)
// 		return (check_redir_error(shell));
// 	*lexer = (*lexer)->next;
// 	clean_input = remove_quotes((char *)(*lexer)->input);
// 	fd = open(clean_input, O_RDONLY, 777);
// 	if (fd == -1)
// 	{
// 		if (shell->flag_pipe == 1)
// 		{
// 			reset_p_pipe_error(shell, parser, clean_input);
// 			*lexer = (*lexer)->next;
// 			return (0);
// 		}
// 		perror(clean_input);
// 		cleanup(shell);
// 		shell->exit_status = 1;
// 		free(clean_input);
// 		return (-1);
// 	}
// 	parser->infile = fd;
// 	free(clean_input);
// 	return (0);
// }

// int	handle_output_redirection(t_shell *shell, t_parser *parser, \
// 									t_lexer **lexer, int flags)
// {
// 	char	*clean_input;
// 	int		fd;

// 	if (!(*lexer) || !(*lexer)->next)
// 		return (check_redir_error(shell));
// 	*lexer = (*lexer)->next;
// 	clean_input = remove_quotes((char *)(*lexer)->input);
// 	fd = open_file(shell, clean_input, flags, 0664);
// 	if (fd == -1)
// 		return (-1);
// 	parser->outfile = fd;
// 	return (0);
// }

// int	handle_heredoc_redirection(t_shell *shell, \
// 				t_parser *parser, t_lexer **lexer)
// {
// 	if (!(*lexer) || !(*lexer)->next)
// 		return (check_redir_error(shell));
// 	*lexer = (*lexer)->next;
// 	handle_heredoc(parser, (*lexer)->input);
// 	if (parser->infile == -1)
// 	{
// 		shell->exit_status = 130;
// 		return (-1);
// 	}
// 	return (0);
// }

// int	handle_token_arg(t_lexer **lexer, t_parser **parser, char *clean_str)
// {
// 	t_list	*node_input;

// 	if ((*lexer)->type == TOKEN_ARG)
// 	{
// 		node_input = ft_lstnew(ft_strdup((*lexer)->input));
// 		if (!node_input)
// 			return (free(clean_str), -1);
// 		ft_lstadd_back(&(*parser)->args, node_input);
// 	}
// 	return (0);
// }
