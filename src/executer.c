/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 12:10:13 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/06 12:14:41 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_path(t_shell *shell, char *cmd, t_list *args)
{
	char 		*cmd_path;
	
	cmd_path = get_external_cmd_path(cmd);
	exec_cmd(cmd_path, args);
}

void	exec_bin(t_shell *shell, char *cmd, t_list *args)
{

	if (ft_strncmp(cmd, "echo", 4) == 0)
	{
		exec_echo(shell, args);
		printf("\n");
	}
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
		exec_path(shell, cmd, args);
}
void	exec_start(t_shell *shell)
{
	t_parser *parser; 

	parser = shell->parser;
	while (parser)
	{
		t_list *args = parser->args;
		printf("| Parser check : %i\n", parser->outfile);
		while (args)
		{
			printf("| Commande : %s\n", (char *)parser->args->content);
			printf("| Infile : %d\n", parser->infile);
			printf("| Outfile : %d\n", parser->outfile);
			char *content = (char *)args->content;
			args = args->next;
			exec_bin(shell, content, args);
			break ;
		}
		parser = parser->next;
	}
}