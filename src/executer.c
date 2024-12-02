/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 12:10:13 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/29 14:24:16 by drongier         ###   ########.fr       */
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
	char			*cmd_clean;
	
	cmd_clean = remove_quotes(cmd);
	is_direct_path = ft_strchr(cmd_clean, '/') != NULL;
    if (is_direct_path)
        cmd_path = ft_strdup(cmd_clean);
	else
        cmd_path = get_external_cmd_path(shell, cmd_clean);
	if (!cmd_path)
    {
        ft_error(shell, "%s: command not found\n", cmd_clean, 127);
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
		free(cmd_clean);
    }
	else
    {
        // Le fichier n'existe pas
        if (is_direct_path)
            ft_error(shell, "minishell: %s: No such file or directory\n", cmd_path, 127);
        free(cmd_path);
		free(cmd_clean);
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
	if (ft_strcmp(cmd, "echo") == 0)
		exec_echo(shell, args);
	else if (ft_strcmp(cmd, "pwd") == 0)
		exec_pwd(shell);
	else if (ft_strcmp(cmd, "cd") == 0)
		exec_cd(shell, args);
	else if (ft_strcmp(cmd, "export") == 0)
		exec_export(shell);
	else if (ft_strcmp(cmd, "unset") == 0)
		exec_unset(shell);
	else if (ft_strcmp(cmd, "env") == 0)
		exec_env(*shell);
	else if (ft_strcmp(cmd, "exit") == 0 && (shell->cmdline[4] == '\0' || shell->cmdline[4] == ' '))
		exec_exit(shell);
	else
		exec_path(shell, cmd, args);
}

void	exec_start(t_shell *shell)
{
	t_parser *parser;
	
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
