/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:51:03 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/20 10:11:16 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// expander.c
#include "minishell.h"

// Vérifie si le caractère fait partie d'un nom de variable valide
static int is_var_char(char c)
{
    return ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||
            (c == '_'));
}

// Vérifie si la variable actuelle est une variable spéciale ($?)
static int is_special_variable(const char *input, int i)
{
    return (input[i + 1] == '?');
}

// Trouve la position de fin de la variable dans la chaîne d'entrée
static int get_variable_end(const char *input, int start)
{
    int j = start;
    while (input[j] && input[j] != ' ' &&
           input[j] != '"' && input[j] != '\'' &&
           is_var_char(input[j]))
        j++;
    return j;
}

// Extrait le nom de la variable à partir des indices de début et de fin
static char *extract_variable_name(const char *input, int start, int end)
{
    char *var_name;

    var_name = ft_substr(input, start, end - start);
    if (!var_name)
        return NULL;
    return var_name;
}

// Nouvelle fonction pour gérer les variables régulières
static char *handle_regular_variable(t_shell *shell, const char *input, int i, int *end)
{
    int start = i + 1;
    char *var_value;
    char *var_name;

    *end = get_variable_end(input, start);
    var_name = extract_variable_name(input, start, *end);
    if (!var_name)
        return NULL;
    if (ft_strlen(var_name) == 0)
    {
        free(var_name);
        var_value = ft_strdup("$");
    }
    else
    {
        var_value = ft_getenv(shell, var_name);
        if (!var_value)
            var_value = ft_strdup("");
        free(var_name);
    }
    return var_value;
}

// Fonction refactorisée `get_variable_value`
static char *get_variable_value(t_shell *shell, const char *input, int i, int *end)
{
    char *var_value;

    if (is_special_variable(input, i))
    {
        var_value = ft_itoa(shell->exit_status);
        if (!var_value)
            return NULL;
        *end = i + 2;
    }
    else
    {
        var_value = handle_regular_variable(shell, input, i, end);
        if (!var_value)
            return NULL;
    }
    return var_value;
}

// Construit la nouvelle chaîne avec la variable remplacée
static char *build_new_input(const char *input, int start, int end, const char *var_value)
{
    char *start_str;
    char *end_str;
    char *temp;
    char *new_input;

    start_str = ft_substr(input, 0, start);
    if (!start_str)
        return NULL;
    end_str = ft_strdup(&input[end]);
    if (!end_str)
    {
        free(start_str);
        return NULL;
    }
    temp = ft_strjoin(start_str, var_value);
    free(start_str);
    if (!temp)
    {
        free(end_str);
        return NULL;
    }
    new_input = ft_strjoin(temp, end_str);
    free(temp);
    free(end_str);
    return new_input;
}

// Remplace la variable dans la chaîne d'entrée avec sa valeur correspondante
static int replace_variable_in_input(t_shell *shell, t_lexer *lexer, int start, int end, const char *var_value)
{
    char *new_input;

    // Utiliser lexer->input au lieu de input
    new_input = build_new_input(lexer->input, start, end, var_value);
    if (!new_input)
        return 0;
    free(lexer->input);
    lexer->input = new_input;
    return 1;
}

// Gère l'expansion d'une variable à la position actuelle dans la chaîne d'entrée
static int expand_variable_at(t_shell *shell, t_lexer *lexer, int *i)
{
    int end;
    char *var_value;

    var_value = get_variable_value(shell, lexer->input, *i, &end);
    if (!var_value)
        return 0;
    if (!replace_variable_in_input(shell, lexer, *i, end, var_value))
    {
        if (!is_special_variable(lexer->input, *i))
            free(var_value);
        return 0;
    }
    if (!is_special_variable(lexer->input, *i))
        free(var_value);
    *i += ft_strlen(var_value);
    return 1;
}

// Traite une seule entrée lexer->input et effectue les expansions nécessaires
static int process_lexer_input(t_shell *shell, t_lexer *lexer)
{
    int i = 0;
    int single_quote = 0;

    while (lexer->input[i])
    {
        if (lexer->input[i] == '\'')
            single_quote = !single_quote;
        if (lexer->input[i] == '$' && !single_quote)
        {
            if (!expand_variable_at(shell, lexer, &i))
                return 0;
        }
        else
            i++;
    }
    return 1;
}

// Fonction principale `expander` qui parcourt chaque lexer et effectue les expansions
void expander(t_shell *shell)
{
    t_lexer *lexer = shell->lexer;

    while (lexer)
    {
        if (!process_lexer_input(shell, lexer))
            return; // En cas d'erreur, arrêter le processus d'expansion
        lexer = lexer->next;
    }
}

