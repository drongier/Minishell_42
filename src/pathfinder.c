/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:45:30 by chbachir          #+#    #+#             */
/*   Updated: 2024/10/15 11:40:11 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"
#include <sys/wait.h>
#include "../libft/libft.h"

#define MAX_PATH_LENGTH 1024

char*	get_external_cmd_path(char * cmd)
{
    static char cmd_path[MAX_PATH_LENGTH];
    int dir_length;
    char* current_dir;

    if (getenv("PATH") == NULL)
        return NULL;
	current_dir = getenv("PATH");
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

void	exec_cmd(char *path, t_list *args)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		char **str;
		str = malloc(sizeof(char *) * ft_lstsize(args) + 2);
		str[0] = path;
		int i = 1;
		while (args)
		{
			str[i] = (char *)args->content;
			args = args->next;
			i++;
		}
		str[i] = NULL;
    	char *envp[] = {NULL};
		if (execve(path, str, envp) == -1)
		{
			//perror(args);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
	}
	
	
}

/* int main() 
{
	char* arg[] = {"cd", "", NULL};
	char* env[] = {""};
	if (execve("export", arg, env) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
	char *cmd = "wc";
    char* path = get_external_cmd_path(cmd);
	
    if (path)
	{
    	printf("Path of %s: %s\n", cmd, path);
	//exec_cmd("cd", "cd");
	//is_builtin("cd");
	}
    else
        printf("%s not found in PATH\n", cmd);
    return 0;
} */
