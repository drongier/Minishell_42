/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 12:10:13 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/26 15:22:43 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_path(t_shell *shell, char *cmd, t_list *args)
{
	char 			*cmd_path;
	int				saved_out;
	int				saved_in;
	struct stat		cmd_stat;
	int             is_direct_path;
	

	is_direct_path = ft_strchr(cmd, '/') != NULL;
    if (is_direct_path)
        cmd_path = ft_strdup(cmd);
	else
        cmd_path = get_external_cmd_path(shell, cmd);
	if (!cmd_path)
    {
        ft_error(shell, "%s: Command not found\n", cmd, 127);
        return ;
    }
	if (stat(cmd_path, &cmd_stat) == 0)
    {
        // Vérifier si c'est un répertoire
        if (S_ISDIR(cmd_stat.st_mode))
        {
            ft_error(shell, "minishell: %s: Is a directory\n", cmd_path, 126);
            free(cmd_path); // Ne pas oublier de libérer la mémoire
            return;
        }

        // Vérifier les permissions d'exécution
        if (access(cmd_path, X_OK) != 0)
        {
            ft_error(shell, "minishell: %s: Permission denied\n", cmd_path, 126);
            free(cmd_path); // Ne pas oublier de libérer la mémoire
            return;
        }
    }
	else
    {
        // Le fichier n'existe pas
        if (is_direct_path)
            ft_error(shell, "minishell: %s: No such file or directory\n", cmd_path, 127);
        free(cmd_path);
        return;
    }
	
	/* if (ft_strchr(cmd, '/'))
		cmd_path = ft_strdup(cmd); // Copier le chemin de la commande */
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
