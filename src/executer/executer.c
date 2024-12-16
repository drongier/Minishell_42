/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 12:10:13 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/16 17:35:52 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_cmd(char *path, t_list *args, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		child_process(path, args, shell);
	else
		parent_process(pid, shell);
}

static void	execute_command_path(t_shell *shell, char *cmd_path, t_list *args)
{
	int		saved_out;
	int		saved_in;

	if (!handle_redirections(shell, &saved_out, &saved_in))
	{
		ft_error(shell, "minishell: redirection error\n", NULL, 1);
		return ;
	}
	exec_cmd(cmd_path, args, shell);
	restore_redirections(saved_out, saved_in);
}

/* static void	exec_path(t_shell *shell, char *cmd, t_list *args)
{
	char	*cmd_clean;
	char	*cmd_path;
	int		is_direct_path;

	if (!cmd)
		return ;
	cmd_clean = remove_quotes(cmd);
	if (!cmd_clean)
		return (ft_error(shell, "Memory allocation error\n", NULL, 1));
	is_direct_path = ft_strchr(cmd_clean, '/') != NULL;
	cmd_path = get_command_path(shell, cmd_clean, is_direct_path);
	if (!cmd_path)
	{
		ft_error(shell, "%s: command not found\n", cmd_clean, 127);
		free(cmd_clean);
		return ;
	}
	if (validate_command(shell, cmd_path, is_direct_path))
		execute_command_path(shell, cmd_path, args);
	free(cmd_path);
	free(cmd_clean);
} */
static void	exec_path(t_shell *shell, char *cmd, t_list *args)
{
	char	*cmd_clean;
	char	*cmd_path;
	int		is_direct_path;

	if (!cmd || !*cmd)
		return ;
	cmd_clean = remove_quotes(cmd);
	if (!cmd_clean)
		return (ft_error(shell, "Memory allocation error\n", NULL, 1));
	if (!*cmd_clean)
		return (free(cmd_clean));
	is_direct_path = ft_strchr(cmd_clean, '/') != NULL;
	cmd_path = get_command_path(shell, cmd_clean, is_direct_path);
	if (!cmd_path)
	{
		ft_error(shell, "%s: command not found\n", cmd_clean, 127);
		free(cmd_clean);
		return ;
	}
	if (validate_command(shell, cmd_path, is_direct_path))
		execute_command_path(shell, cmd_path, args);
	free(cmd_path);
	free(cmd_clean);
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
	else if (ft_strcmp(cmd, "exit") == 0 && \
		(shell->cmdline[4] == '\0' || shell->cmdline[4] == ' '))
		exec_exit(shell);
	else
		exec_path(shell, cmd, args);
}

void	exec_start(t_shell *shell)
{
	t_parser	*parser;
	t_list		*args;
	char		*cmd_name;

	parser = shell->parser;
	if (shell->flag_pipe == 1)
		exec_with_pipe(shell);
	else
	{
		while (parser)
		{
			args = parser->args;
			while (args)
			{
				cmd_name = (char *)args->content;
				args = args->next;
				exec_bin(shell, cmd_name, args);
				break ;
			}
			parser = parser->next;
		}
	}
}
