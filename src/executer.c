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
	char 		*cmd_path;
	t_parser 	*parser;
	
	parser = shell->parser;
	cmd_path = get_external_cmd_path(cmd);
	exec_cmd(cmd_path, parser->args);

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
void	exec_start(t_shell *shell)
{
	t_parser *parser; 

	parser = shell->parser;
	while (parser)
	{
		while (parser->args)
		{
			printf("| Commande : %s\n", (char *)parser->args->content);
			printf("| Infile : %d\n", parser->infile);
			printf("| Outfile : %d\n", parser->outfile);
			char *content = (char *)parser->args->content;
			parser->args = parser->args->next;
			printf("| pos :%zu\n", shell->lexer->pos);
			exec_bin(shell, content);
			break ;
		}
		parser = parser->next;
	}
}