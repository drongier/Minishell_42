/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:05:03 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/12 12:31:08 by chbachir         ###   ########.fr       */
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
static void free_exec_split(char **strs)
{
    int i;

    if (!strs)
        return;
    i = 0;
    while (strs[i])
    {
        free(strs[i]);
        i++;
    }
    free(strs);
}

static char *ft_strjoin_three(char *s1, char *s2, char *s3)
{
    char *temp;
    char *result;

    temp = ft_strjoin(s1, s2);
    if (!temp)
        return NULL;
    result = ft_strjoin(temp, s3);
    free(temp);
    return result;
}

char *get_external_cmd_path(t_shell *shell, char *cmd)
{
    char **paths;
    char *path_env;
    char *cmd_path;
    int i;

    path_env = ft_getenv(shell, "PATH");
    if (!path_env)
        return NULL;
    paths = ft_exec_split(path_env, ':');
    if (!paths)
        return NULL;
    i = 0;
    while (paths[i])
    {
        cmd_path = ft_strjoin_three(paths[i], "/", cmd);
        if (access(cmd_path, X_OK) == 0)
        {
            free_exec_split(paths);
            return cmd_path;
        }
        free(cmd_path);
        i++;
    }
    free_exec_split(paths);
    return NULL;
}
