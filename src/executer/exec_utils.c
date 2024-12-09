/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:05:03 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/09 13:34:01 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	restore_redirections(int saved_out, int saved_in)
{
	dup2(saved_out, STDOUT_FILENO);
	dup2(saved_in, STDIN_FILENO);
	close(saved_out);
	close(saved_in);
}

int	handle_redirections(t_shell *shell, int *saved_out, int *saved_in)
{
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_out == -1)
		return (0);
	*saved_in = dup(STDIN_FILENO);
	if (*saved_in == -1)
	{
		close(*saved_out);
		return (0);
	}
	if (shell->parser->outfile != STDOUT_FILENO)
		dup2(shell->parser->outfile, STDOUT_FILENO);
	if (shell->parser->infile != STDIN_FILENO)
		dup2(shell->parser->infile, STDIN_FILENO);
	return (1);
}

int	validate_command(t_shell *shell, char *cmd_path, int is_direct_path)
{
	struct stat		cmd_stat;

	if (stat(cmd_path, &cmd_stat) == 0)
	{
		if (S_ISDIR(cmd_stat.st_mode))
		{
			ft_error(shell, "minishell: %s: Is a directory\n", cmd_path, 126);
			return (0);
		}
		if (access(cmd_path, X_OK) != 0)
		{
			ft_error(shell, "minishell: %s: "PD"\n", cmd_path, 126);
			return (0);
		}
		return (1);
	}
	else
	{
		if (is_direct_path)
			ft_error(shell, "minishell: %s: "NSFOD"\n", cmd_path, 127);
		return (0);
	}
}

char	*get_external_cmd_path(t_shell *shell, char *cmd)
{
	static char		cmd_path[MAX_PATH_LENGTH];
	int				dir_length;
	char			*cur_dir;

	if (ft_getenv(shell, "PATH") == NULL)
		return (NULL);
	cur_dir = ft_getenv(shell, "PATH");
	while (*cur_dir != '\0')
	{
		dir_length = 0;
		while ((cur_dir[dir_length] != ':') && (cur_dir[dir_length] != '\0'))
			dir_length++;
		if (dir_length + 4 < MAX_PATH_LENGTH)
		{
			ft_strncpy(cmd_path, cur_dir, dir_length);
			cmd_path[dir_length] = '/';
			ft_strcpy(cmd_path + dir_length + 1, cmd);
			if (access(cmd_path, X_OK) == 0)
				return (cmd_path);
		}
		cur_dir += dir_length;
		if (*cur_dir == ':')
			cur_dir++;
	}
	return (NULL);
}