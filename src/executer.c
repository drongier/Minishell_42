/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 12:10:13 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/18 22:53:38 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_path(t_shell *shell, char *cmd, t_list *args)
{
	char 			*cmd_path;
	int				saved_out;
	int				saved_in;
	char			*cmd_clean;

	cmd_clean = remove_quotes(cmd);
	cmd_path = get_external_cmd_path(shell, cmd_clean);
	if (ft_strchr(cmd_clean, '/'))
		cmd_path = ft_strdup(cmd_clean); // Copier le chemin de la commande
	saved_out = dup(STDOUT_FILENO);
	saved_in = dup(STDIN_FILENO);
	if (shell->parser->outfile != STDOUT_FILENO)
		dup2(shell->parser->outfile, STDOUT_FILENO);
	if (shell->parser->infile != STDIN_FILENO)
		dup2(shell->parser->infile, STDIN_FILENO);
	exec_cmd(cmd_path, args, shell);
	dup2(saved_out, STDOUT_FILENO);
	dup2(saved_in, STDIN_FILENO);
}

void	exec_bin(t_shell *shell, char *cmd, t_list *args)
{

	if (ft_strncmp(cmd, "echo", 4) == 0)
		exec_echo(shell, args);
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		exec_pwd(shell);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		exec_cd(shell, args);
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
	if (shell->exit_status == 130)
    {
        // Interruption du heredoc, ne pas exécuter les commandes
        return;
    }


	parser = shell->parser;
	if (shell->flag_pipe == 1)
		exec_with_pipe(shell);
	else
		while (parser)
		{
			t_list *args = parser->args;
			while (args)
			{
				char *cmd_name = (char *)args->content;
				args = args->next;
				exec_bin(shell, cmd_name, args);
				break ;
			}
			parser = parser->next;
		}
}
