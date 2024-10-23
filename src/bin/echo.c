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

char *remove_quotes(char *str)
{
    if (!str)
        return NULL;

    size_t len = ft_strlen(str);
    char *result = malloc(len + 1);
    if (!result)
        return NULL;

    size_t i = 0;
    size_t j = 0;

    int inside_single = 0;
    int inside_double = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '\'' && !inside_double)
            inside_single = !inside_single;
        else if (str[i] == '\"' && !inside_single)
            inside_double = !inside_double;
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
    char        *original;
    char        *minus_n;
    int         newline = 1;


    printf("MERGE TEST");
    parser = shell->parser;
    if (parser->args != NULL)
    {
        minus_n = (char *)parser->args->content;
        minus_n = remove_quotes(minus_n);
    }
    if (parser->args != NULL && ft_strcmp(minus_n, "-n") == 0)
    {
        newline = 0;
        parser->args = parser->args->next;
    }
    while (parser->args != NULL)
    {
        original = (char *)parser->args->content;
        if (valid_quotes(original))
        {
            original = remove_quotes(original);
            if (parser->outfile != STDOUT_FILENO)
            {
                write(parser->outfile, original, ft_strlen(original));
                if (parser->args->next)
                    write(parser->outfile, " ", 1);
            }
            else
            {
                printf("%s", original);
                if (parser->args->next)
                    printf(" ");
            }
        }
        parser->args = parser->args->next;
    }
    if (newline)
    {
        if (parser->outfile != STDOUT_FILENO)
            write(parser->outfile, "\n", 1);
        else
            printf("\n");
    }
}
