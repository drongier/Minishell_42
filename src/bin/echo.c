/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:05:13 by emaydogd          #+#    #+#             */
/*   Updated: 2024/10/15 14:31:27 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int valid_quotes(char *str)
{
    if (!str)
        return 0;

    int inside_single = 0;
    int inside_double = 0;
    size_t i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '\'' && !inside_double)
        {
            inside_single = !inside_single;
        }
        else if (str[i] == '\"' && !inside_single)
        {
            inside_double = !inside_double;
        }
        i++;
    }

    return (!inside_single && !inside_double);
}

// Function to remove matching quotes from the input string
char *remove_quotes(char *str)
{
    if (!str)
        return NULL;

    size_t len = ft_strlen(str);
    char *result = malloc(len + 1); // Allocate maximum possible size
    if (!result)
        return NULL;

    size_t i = 0; // Index for input string
    size_t j = 0; // Index for output string

    int inside_single = 0;
    int inside_double = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '\'' && !inside_double)
        {
            inside_single = !inside_single;
            // Do not copy the single quote
        }
        else if (str[i] == '\"' && !inside_single)
        {
            inside_double = !inside_double;
            // Do not copy the double quote
        }
        else
        {
            result[j] = str[i];
            j++;
        }
        i++;
    }

    result[j] = '\0';

    return result;
}

void exec_echo(t_shell *shell)
{
    t_parser    *parser;
    char        *content;
    char        *original;

    parser = shell->parser;
    printf("BONJOUR TEST MERGE\n");
    if (parser->args == NULL)
        printf("\n");
    while (parser->args != NULL)
    {
        original = (char *)parser->args->content;
        // Validate quotes before removing them
        if (valid_quotes(original))
        {
            content = remove_quotes(original);
            if (parser->outfile != STDOUT_FILENO)
            {
                write(parser->outfile, content, ft_strlen(content));
                write(parser->outfile, "\n", 1);
                if (parser->args->next)
                    write(parser->outfile, "\n", 1);
            }
            else
            {
                if (ft_strcmp(content, "-n") == 0)
                    return;
                printf("%s", content);
                printf("\n");
                if (parser->args->next)
                    printf("\n");
            }
            free(content);
        }
        else
        {
            // If quotes are invalid, trigger an error
            error(shell, "bad syntax", NULL);
            return ;
        }
        parser->args = parser->args->next;
    }
    if (parser->outfile != STDOUT_FILENO)
        close(parser->outfile);
    error(shell, NULL, NULL);
}
