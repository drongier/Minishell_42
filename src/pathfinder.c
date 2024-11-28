/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:45:30 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/28 13:05:34 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define MAX_PATH_LENGTH 1024

char *create_env_str(t_env *env)
{
	char 	*env_str;
	size_t	key_len;
	size_t	val_len;
	
	key_len = ft_strlen(env->key);
	val_len = ft_strlen(env->value);
	env_str = malloc(sizeof(char *) + key_len + val_len + 2); // +1 for = + 1 for '\0'
	if (!env_str)
		return NULL;
	ft_strcpy(env_str, env->key);
	env_str[key_len] = '=';
	ft_strcpy(env_str + key_len + 1, env->value);
	return env_str;
}

int	ft_env_size(t_env *env)
{
	int	size = 0;
	while (env)
	{
		env = env->next;
		size++;
	}
	return (size);
}

char	**convert_env_to_array(t_env *env)
{
	char **envp = malloc(sizeof(char *) * (ft_env_size(env)) + 1);
	if(!envp)
		return NULL;
	int i = 0;
	while(env)
	{
		envp[i] = create_env_str(env);
		if (!envp[i])
		{
			free_envp(envp, i);
			return NULL;
		}
		i++;
		env = env->next;
	}
	envp[i] = NULL;
	return envp;
}

char*	get_external_cmd_path(t_shell *shell, char * cmd)
{
    static char cmd_path[MAX_PATH_LENGTH];
    int dir_length;
    char* current_dir;
    if (ft_getenv(shell, "PATH") == NULL)
        return NULL;
	current_dir = ft_getenv(shell, "PATH");
    while (*current_dir != '\0')
	{
        dir_length = 0;
        while ((current_dir[dir_length] != ':') && (current_dir[dir_length] != '\0'))
            dir_length++;
        if (dir_length + 4 < MAX_PATH_LENGTH)
		{
            ft_strncpy(cmd_path, current_dir, dir_length);
            cmd_path[dir_length] = '/';
            ft_strcpy(cmd_path + dir_length + 1, cmd);
            if (access(cmd_path, X_OK) == 0)
                return cmd_path;
        }
        current_dir += dir_length;
        if (*current_dir == ':')
            current_dir++;
    }
    return (NULL);
}

void exec_cmd(char *path, t_list *args, t_shell *shell)
{
    pid_t pid;
    int status;
    t_list *arg_node = args;
    int i;
    char *cmd_name;
    char *argv[100];

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {	
        cmd_name = (char *)shell->parser->args->content;
        char **envp = convert_env_to_array(shell->env);
        if (!envp)
            return;
        argv[0] = cmd_name;
        i = 1;
        while (arg_node)
        {
            argv[i] = (char *)arg_node->content;
            i++;
            arg_node = arg_node->next;
        }
        argv[i] = NULL;
        if (execve(path, argv, envp) == -1)
        {
            if (errno == EACCES)
                exit(126);
            else if (errno == ENOENT)
                exit(127);
            else
            {
                ft_error(shell, "%s : command not found\n", (char *)shell->parser->args->content, -1);
                exit(127);
            }
        }
        exit(EXIT_SUCCESS);
    }
else
{
    waitpid(pid, &status, 0);
    
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
            write(STDOUT_FILENO, "\n", 1);
            g_signal = SIGINT;
    }
    else if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
}
}


