/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 13:25:12 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/12 11:09:21 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redirection(int type)
{
	return (type == TOKEN_REDIR_IN || \
			type == TOKEN_REDIR_OUT || \
			type == TOKEN_REDIR_APPEND || \
			type == TOKEN_REDIR_HEREDOC);
}

t_parser	*new_cmd_node(void)
{
	t_parser	*cmd;

	cmd = malloc(sizeof(t_parser));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = STDIN_FILENO;
	cmd->outfile = STDOUT_FILENO;
	cmd->next = NULL;
	return (cmd);
}

int	check_redir_error(t_shell *shell)
{
	check_error_token_redi(shell);
	shell->parser = NULL;
	shell->exit_status = 2;
	return (-1);
}

int	open_file(t_shell *shell, char *filename, int flags, int mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
	{
		perror(filename);
		cleanup(shell);
		shell->exit_status = 1;
		free(filename);
		return (-1);
	}
	free(filename);
	return (fd);
}

void	reset_p_pipe_error(t_shell *shell, \
				t_parser *parser, char *clean_input)
{
	perror(clean_input);
	shell->exit_status = 1;
	parser->args = NULL;
	parser->infile = STDIN_FILENO;
	free(clean_input);
}
