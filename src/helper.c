/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:10:57 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/18 21:18:28 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim(char *str)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
		{
			new[j] = str[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
	return (new);
}

void print_cmdtable(t_shell shell)
{
	t_parser *current_node = shell.parser;
	int cmd_num = 1;

	printf("Command Table:\n");
	while (current_node)
	{
		printf("\n	Command %d:\n", cmd_num++);
		t_parser *cmd = (t_parser *)current_node;
		printf("		infile: %d\n", cmd->infile);
		printf("		outfile: %d\n", cmd->outfile);
		int i = 0;
		if (cmd->args)
		{
			while (cmd->args)
			{
				printf("		Arg[%d]: %s\n", i, (char *)cmd->args->content);
				i++;
				cmd->args = cmd->args->next;
			}
		}
		else
			printf("  No command arguments.\n");
		current_node = current_node->next;
	}
	printf("------------------------------------------------\n");
}
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