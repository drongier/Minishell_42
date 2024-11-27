/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:50:30 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/11 16:26:06 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cleanup(t_shell *shell)
{
    if (!shell)
        return;

    if (shell->parser)
    {
        shell->parser->infile = STDIN_FILENO;
        shell->parser->outfile = STDOUT_FILENO;
        free_parser(shell->parser);
        shell->parser = NULL;
    }

    if (shell->lexer)
    {
        free_lexer(shell->lexer);
        shell->lexer = NULL;
    }
}

void	free_lexer(t_lexer *lexer)
{
	t_lexer *tmp;
	while (lexer)
	{
		tmp = lexer->next;
		if (lexer->input)
		{
			free(lexer->input);
			lexer->input = NULL;
		}
		free(lexer);
		lexer = tmp;
	}
}

void	free_parser(t_parser *parser)
{
	t_parser *tmp;
	
	t_list *tmp_fullcmd;
	while (parser)
	{
		tmp = parser->next;
		while (parser->args)
		{
			tmp_fullcmd = parser->args->next;
			if(parser->args)
			{
				free(parser->args);
				parser->args = NULL;
			}
			parser->args = tmp_fullcmd;
		}
		free(parser);
		parser = tmp;
	}
}
void	free_env(t_env *env)
{
	t_env *tmp;
	while(env)
	{
		tmp = env->next;
		if (env->key)
		{
			free(env->key);
			env->key = NULL;
		}
		free(env);
		env = tmp;
	}
}

void free_envp(char **envp, int idx)
{
	if (!envp)
		return ;
	while (idx--)
		free(envp[idx]);
	free(envp);
}
