/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 12:10:13 by chbachir          #+#    #+#             */
/*   Updated: 2024/10/15 12:21:33 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_path(t_shell *shell, char *cmd)
{
	char * cmd_path;

	t_parser *parser;
	
	parser = shell->parser;
	cmd_path = get_external_cmd_path(cmd);
	if (parser->infile != STDIN_FILENO)
		dup2(parser->infile, STDIN_FILENO);
	exec_cmd(cmd_path, parser->args);
}

void	exec_start(t_shell *shell)
{
	t_parser *current_node; 
	
	current_node = shell->parser;
	while (current_node)
	{
		t_parser *cmd = (t_parser *)current_node;
		while (cmd->args)
		{
			char *content = (char *)cmd->args->content;
			cmd->args = cmd->args->next;
			exec_bin(shell, content);
			break ;
		}
		current_node = current_node->next;
	}
}

void	exec_bin(t_shell *shell, char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		exec_echo(shell);
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		exec_pwd(shell);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		exec_cd(shell);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		exec_export(shell);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		exec_unset(shell);
	else if (ft_strncmp(cmd, "env", 3) == 0)
		exec_env(*shell);
	else if (ft_strncmp(cmd, "exit", 4) == 0 && (shell->cmdline[4] == '\0' || shell->cmdline[4] == ' '))
		exec_exit(shell);
	else
		exec_path(shell, cmd);
}
