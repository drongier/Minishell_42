/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:50:30 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/12 23:32:19 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup(t_shell *shell)
{
	if (!shell)
		return ;
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
	t_lexer	*tmp;

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

void    free_parser(t_parser *parser)
{
    t_parser    *tmp_parser;
    t_list      *tmp_args;
    t_list      *current_arg;

    while (parser)
    {
        tmp_parser = parser->next;
        
        // Libérer la liste des arguments
        current_arg = parser->args;
        while (current_arg)
        {
            tmp_args = current_arg->next;
            if (current_arg->content)
                free(current_arg->content);
            free(current_arg);
            current_arg = tmp_args;
        }
        
        // Fermer les descripteurs de fichiers si nécessaire
        if (parser->infile != STDIN_FILENO)
            close(parser->infile);
        if (parser->outfile != STDOUT_FILENO)
            close(parser->outfile);
            
        free(parser);
        parser = tmp_parser;
    }
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_envp(char **envp, int idx)
{
	if (!envp)
		return ;
	while (idx--)
		free(envp[idx]);
	free(envp);
}
