/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:31:29 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/26 14:55:22 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_with_pipe(t_shell *shell)
{
	t_parser 	*parser;
	int 		status;

	parser = shell->parser;
    while (parser)
    {
        pid_t pid = fork();
        
        if (pid == 0) 
		{
            if (parser->infile != STDIN_FILENO) 
			{
                dup2(parser->infile, STDIN_FILENO);
                close(parser->infile);
            }
            if (parser->outfile != STDOUT_FILENO)
			{
                dup2(parser->outfile, STDOUT_FILENO);
                close(parser->outfile);
            }
            char **args = list_to_array(parser->args);
			char *path = get_external_cmd_path(shell, args[0]);
            if (ft_strncmp(args[0], "env", 3) == 0)
		        exec_env(*shell);
            else
                if (execve(path, args, NULL) == -1)
				{
                    ft_error(shell, "%s : command not found\n", args[0], -1);
					exit(127);
				}
			exit(EXIT_SUCCESS);
        }
        if (parser->infile != STDIN_FILENO)
            close(parser->infile);
        if (parser->outfile != STDOUT_FILENO)
            close(parser->outfile);
        parser = parser->next;
    }
	while (wait(&status) > 0) 
	{
		if (WIFEXITED(status)) 
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status)) 
			shell->exit_status = 128 + WTERMSIG(status);
		else 
			shell->exit_status = 1;
		shell->flag_pipe = 0;
	}
}


