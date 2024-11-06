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

static void	exec_path(t_shell *shell, char *cmd)
{
	char 			*cmd_path;
	t_parser 		*parser;
	struct stat		cmd_stat;
	
	parser = shell->parser;
	if (ft_strchr(cmd, '/'))
		cmd_path = ft_strdup(cmd); // Copier le chemin de la commande
	else
		cmd_path = get_external_cmd_path(cmd);
	//printf("cmd_path = '%s'\n", cmd_path);
	if (!cmd_path)
    {
        error(shell, "minishell: %s: Command not found\n", cmd, 127);
        return ;
    }
 	if (stat(cmd_path, &cmd_stat) == 0)
    {
        // Vérifier si c'est un répertoire
        if (S_ISDIR(cmd_stat.st_mode))
        {
            error(shell, "minishell: %s: Is a directory\n", cmd_path, 126);
            free(cmd_path); // Ne pas oublier de libérer la mémoire
            return;
        }

        // Vérifier les permissions d'exécution
        if (access(cmd_path, X_OK) != 0)
        {
            error(shell, "minishell: %s: Permission denied\n", cmd_path, 126);
            free(cmd_path); // Ne pas oublier de libérer la mémoire
            return;
        }
    }
    else
    {
        // Le fichier n'existe pas
        error(shell, "minishell: %s: No such file or directory\n", cmd_path, 127);
        free(cmd_path); // Ne pas oublier de libérer la mémoire
        return;
    }



	if (parser->infile != STDIN_FILENO)
		dup2(parser->infile, STDIN_FILENO);
	exec_cmd(cmd_path, parser->args);
	//free(cmd_path);
}

void	exec_start(t_shell *shell)
{
	t_parser *current_node = shell->parser;
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
	{
		exec_echo(shell);
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
		exec_path(shell, cmd);
}
