/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:50:30 by chbachir          #+#    #+#             */
/*   Updated: 2024/09/20 18:15:49 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup(t_shell *shell)
{
	if (shell->lexer)
	{
		free_lexer(shell->lexer);
		shell->lexer = NULL;
	}
	if (shell->parser)
	{
		free_parser(shell->parser);
		shell->parser = NULL;
	}
/* 	
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	if (shell->cmdline)
	{
		free(shell->cmdline);
		shell->cmdline = NULL;
	}
	 */
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
