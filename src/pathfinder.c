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

char    *get_external_cmd_path(char *cmd)
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

void exec_cmd(char *path, t_list *arg_node, t_shell *shell) 
{
    pid_t pid;
    int status;
    t_pipex pipe = shell->parser->pipex;
    
    pid = fork();
    if (pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) 
	{
        char **str = malloc(sizeof(char *) * (ft_lstsize(arg_node) + 2));
        str[0] = path;
        int i = 1;
        while (arg_node) 
        {
            str[i] = (char *)arg_node->content;
            arg_node = arg_node->next;
            i++;
        }
        str[i] = NULL;
        if (shell->parser->infile != STDIN_FILENO) {
            dup2(shell->parser->infile, STDIN_FILENO);
            close(shell->parser->infile);
        }
        
        if (shell->parser->outfile != STDOUT_FILENO) {
            dup2(shell->parser->outfile, STDOUT_FILENO);
            close(shell->parser->outfile);
        }
        if (pipe.read_fd > 2)
            close(pipe.read_fd);
        if (pipe.write_fd > 2)
            close(pipe.write_fd);

        char **envp = {NULL};
        if (execve(path, str, envp) == -1)
        perror("execve");
        exit(EXIT_FAILURE);
    } 
	else 
	{
        if (pipe.read_fd > 2)
            close(pipe.read_fd);
        if (pipe.write_fd > 2)
            close(pipe.write_fd);
            
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }
}