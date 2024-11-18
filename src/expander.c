/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 17:16:34 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/18 15:20:01 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Vérifie si le caractère est valide pour un nom de variable
static int is_valid_var_char(char c)
{
    if ((c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        (c >= '0' && c <= '9') ||
        c == '_')
        return (1);
    return (0);
}

// Crée une nouvelle chaîne en ajoutant un caractère à une chaîne existante
static char *ft_strjoin_char(char *s, char c)
{
    char    *new_str; 
    size_t  len;

    if (!s)
        return (NULL);
    len = ft_strlen(s);
    new_str = ft_calloc(len + 2, sizeof(char));
    if (!new_str)
        return (NULL);
    ft_memcpy(new_str, s, len);
    new_str[len] = c;
    return (new_str);
}

// Ajoute une chaîne à `new_input`
static void append_str(char **str, char *addition)
{
    char *temp;

    temp = ft_strjoin(*str, addition);
    if (!temp)
        return ;
    free(*str);
    *str = temp;
}

// Trouve la fin du nom de la variable d'environnement
static int find_var_end(char *input, int start)
{
    int j;

    j = start;
    while (input[j] && input[j] != ' ' && input[j] != '"' && input[j] != '\'' &&
           is_valid_var_char(input[j]))
        j++;
    return (j);
}

// Ajoute un caractère à la chaîne `new_input`
static void append_char(char **str, char c)
{
    char *temp;

    temp = ft_strjoin_char(*str, c);
    if (!temp)
        return ;
    free(*str);
    *str = temp;
}

// Récupère la valeur de la variable d'environnement
static char *get_env_value(t_shell *shell, char *key)
{
    char *value;

    if (ft_strcmp(key, "") == 0)
        return (ft_strdup(""));
    value = ft_getenv(shell, key);
    if (value == NULL)
        return (ft_strdup(""));
    return (ft_strdup(value));
}

// Gère l'ajout de la valeur après `$?`
static int handle_dollar_question(t_shell *shell, char **new_input, int i)
{
    char *key_env;

    key_env = ft_itoa(shell->exit_status);
    if (!key_env)
        return (i + 1);
    append_str(new_input, key_env);
    free(key_env);
    return (i + 2);
}

// Gère l'ajout de la valeur après `$VAR`
static int handle_dollar_var(t_shell *shell, t_lexer *lexer, char **new_input, int i)
{
    char    *key;
    char    *key_env;
    int     j;

    j = find_var_end(lexer->input, i + 1);
    key = ft_substr(lexer->input, i + 1, j - (i + 1));
    if (!key)
        return (j);
    if (ft_strcmp(key, "") == 0)
    {
        free(key);
        return (i + 1);
    }
    key_env = get_env_value(shell, key);
    if (!key_env)
        key_env = ft_strdup("");
    append_str(new_input, key_env);
    free(key);
    free(key_env);
    return (j);
}

// Gère l'expansion après un symbole `$`
static int handle_dollar(t_shell *shell, t_lexer *lexer, char **new_input, int i)
{
    if (lexer->input[i + 1] == '?')
        return handle_dollar_question(shell, new_input, i);
    else
    {
        return handle_dollar_var(shell, lexer, new_input, i);
    }
}

// Gère le basculement de l'état de la citation simple
static int toggle_single_quote(int single_quote)
{
    if (single_quote)
        return (0);
    return (1);
}

// Gère l'expansion de la chaîne après le traitement du symbole `$`
static void process_dollar(t_shell *shell, t_lexer *lexer, char **new_input, int *i)
{
    int new_i;

    new_i = handle_dollar(shell, lexer, new_input, *i);
    if (new_i <= *i)
        return ; // Gestion d'erreur ou autre traitement
    *i = new_i;
}

// Gère l'expansion des variables dans une chaîne d'entrée spécifique
static void expand_input(t_shell *shell, t_lexer *lexer)
{
    int     i;
    int     single_quote;
    char    *new_input;

    i = 0;
    single_quote = 0;
    new_input = ft_strdup("");
    if (!new_input)
        return ;
    while (lexer->input[i])
    {
        if (lexer->input[i] == '\'')
            single_quote = toggle_single_quote(single_quote);
        if (lexer->input[i] == '$' && !single_quote)
            process_dollar(shell, lexer, &new_input, &i);
        else
        {
            append_char(&new_input, lexer->input[i]);
            i++;
        }
    }
    free(lexer->input);
    lexer->input = new_input;
}

// Fonction principale
void expander(t_shell *shell)
{
    t_lexer *lexer;

    lexer = shell->lexer;
    while (lexer)
    {
        expand_input(shell, lexer);
        lexer = lexer->next;
    }
}

// Redéfinitions des fonctions statiques











/* void expander(t_shell *shell)
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
                        return ;
                    j = i + 2;
                }
                else
                {
                    j = i + 1;
                    while (lexer->input[j] && lexer->input[j] != ' ' &&
                           lexer->input[j] != '"' && lexer->input[j] != '\'' &&
                           ( (lexer->input[j] >= 'A' && lexer->input[j] <= 'Z') ||
                             (lexer->input[j] >= 'a' && lexer->input[j] <= 'z') ||
                             (lexer->input[j] >= '0' && lexer->input[j] <= '9') ||
                             lexer->input[j] == '_'))
                        j++;
                    key = ft_substr(lexer->input, i + 1, j - i - 1);
                    if (!key)
                        return ;
                    if (ft_strcmp(key, "") == 0)
                    {
                        free(key);
                        i++;
                        continue ;
                    }
                    key_env = ft_getenv(shell, key);
                    if (key_env == NULL)
                        key_env = "";
                    free(key);
                }
                start = ft_substr(lexer->input, 0, i);
                if (!start)
                    return ;

                end = ft_substr(lexer->input, j, strlen(lexer->input) - j);
                if (!end)
                {
                    free(start);
                    return ;
                }
                dest = ft_calloc(strlen(start) + strlen(key_env) + strlen(end) + 1, sizeof(char));
                if (!dest)
                {
                    free(start);
                    free(end);
                    return ;
                }
                ft_strcat(dest, start);
                ft_strcat(dest, key_env);
                ft_strcat(dest, end);
                free(lexer->input);
                lexer->input = ft_strdup(dest);

                if (!lexer->input)
                {
                    free(start);
                    free(end);
                    free(dest);
                    return ;
                }
                free(start);
                free(end);
                free(dest);
                i += strlen(key_env);
            }
            else
                i++;
        }
        lexer = lexer->next;
    }
} */


/*
OLD OLD 
void	expander(t_shell *shell)
{
	t_lexer * lexer;
	int			i;
	int			j;
	int			single_quote;
	char		*key;
	char		*key_env;
	char		*start;
	char		*end;
	char		*dest;
	
	dest = NULL;
	lexer = shell->lexer;

	//correct_single_quotes(shell->cmdline); // Chakib : not supposed to write here, should be done in executer
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
				start = ft_substr(lexer->input, 0,  i);
				j = i;
				while (lexer->input[j] != ' ' && lexer->input[j] != '"'  && lexer->input[j] != '\'' && lexer->input[j])
					j++;
				if (lexer->input[i + 1] == '?')
				{
					key_env = ft_itoa(shell->exit_status);
				}
				else
				{
					key = ft_substr(lexer->input, i + 1, j - i - 1);
					if (ft_strcmp(key, "") == 0)
					{
						i++;
						continue ;
					}
					key_env = ft_getenv(shell, key);
					if (key_env == NULL)
						key_env = "";
					free(key);
				}
				end = ft_substr(lexer->input,j, ft_strlen(lexer->input) - j + 1);
				dest = ft_calloc((ft_strlen(start) + ft_strlen(key_env) + ft_strlen(end) + 1), sizeof(char));
				if (!dest)
				 	return ;
				dest = ft_strcat(dest, start);
				dest = ft_strcat(dest, key_env);
				dest = ft_strcat(dest, end);
				lexer->input = ft_strdup(dest);

				free(start);
				//free(key_env); // Chakib : ATTENTION with SEGFAULT
				free(end);
				free(dest);
				i = j;
			}
			i++;
		}
		lexer = lexer->next;
	}
} */
