/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:31:29 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/12 17:31:32 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**list_to_array(t_list *args)
{
	t_list *args2;
	args2 = args;
	char **str = malloc(sizeof(char *) * (ft_lstsize(args2) + 2));

    int i = 0;
    while (args2) 
    {
        str[i] = (char *)args2->content;
        args2 = args2->next;
        i++;
	}
    str[i] = NULL;
	return(str);
}

void exec_with_pipe(t_shell *shell)
{
	t_parser *parser;

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
            if (execve(path, args, NULL) == -1)
                error(shell, "%s: command not found\n", args[0], 127);
            exit(EXIT_FAILURE);
        }     
        if (parser->infile != STDIN_FILENO)
            close(parser->infile);
        if (parser->outfile != STDOUT_FILENO)
            close(parser->outfile);
        parser = parser->next;
    }
    while (wait(NULL) > 0);
	shell->flag_pipe = 0;
}


