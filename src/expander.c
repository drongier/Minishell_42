/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 17:16:34 by emaydogd          #+#    #+#             */
/*   Updated: 2024/10/15 14:32:10 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void expander(t_shell *shell)
{
    t_lexer *lexer;
    int i;
    int j;
    int single_quote;
    char *key;
    char *key_env;
    char *start;
    char *end;
    char *dest;

    dest = NULL;
    lexer = shell->lexer;
    while (lexer)
    {
        i = 0;
        single_quote = 0;
        while (lexer->input[i])
        {
            if (lexer->input[i] == '\'')
                single_quote = !single_quote;

            if (lexer->input[i] == '$' && !single_quote)
            {
                if (lexer->input[i + 1] == '?')
                {
                    key_env = ft_itoa(shell->exit_status);
                    if (!key_env)
                        return ; // Handle memory allocation failure
                    j = i + 2;
                }
                else
                {
                    // Find the end of the environment variable name
                    j = i + 1;
                    while (lexer->input[j] && lexer->input[j] != ' ' &&
                           lexer->input[j] != '"' && lexer->input[j] != '\'' &&
                           ( (lexer->input[j] >= 'A' && lexer->input[j] <= 'Z') ||
                             (lexer->input[j] >= 'a' && lexer->input[j] <= 'z') ||
                             (lexer->input[j] >= '0' && lexer->input[j] <= '9') ||
                             lexer->input[j] == '_'))
                        j++;
                    // Extract the key name
                    key = ft_substr(lexer->input, i + 1, j - i - 1);
                    if (!key)
                        return ; // Handle memory allocation failure
                    if (ft_strcmp(key, "") == 0)
                    {
                        // If the key is empty, skip the '$' symbol
                        free(key);
                        i++;
                        continue ;
                    }

                    // Retrieve the environment variable value
                    key_env = ft_getenv(shell, key);
                    if (key_env == NULL)
                        key_env = "";
                    free(key);
                }
                // Extract the parts before and after the variable
                start = ft_substr(lexer->input, 0, i);
                if (!start)
                    return ; // Handle memory allocation failure

                end = ft_substr(lexer->input, j, strlen(lexer->input) - j);
                if (!end)
                {
                    free(start);
                    return ; // Handle memory allocation failure
                }

                // Allocate memory for the new string
                dest = ft_calloc(strlen(start) + strlen(key_env) + strlen(end) + 1, sizeof(char));
                if (!dest)
                {
                    free(start);
                    free(end);
                    return ; // Handle memory allocation failure
                }

                // Concatenate start, key_env, and end to form the new string
                ft_strcat(dest, start);
                ft_strcat(dest, key_env);
                ft_strcat(dest, end);

                // Update lexer->input with the new string
                free(lexer->input);
                lexer->input = ft_strdup(dest);
                if (!lexer->input)
                {
                    free(start);
                    free(end);
                    free(dest);
                    return ; // Handle memory allocation failure
                }

                // Free temporary strings
                free(start);
                free(end);
                free(dest);

                // Move the index to continue parsing after the replaced variable
                i += strlen(key_env);
            }
            else
                i++;
        }
        lexer = lexer->next;
    }
}