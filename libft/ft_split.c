/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 19:27:29 by emaydogd          #+#    #+#             */
/*   Updated: 2024/10/08 13:36:51 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t count_tokens(const char *str, int *error_flag)
{
    size_t count = 0;
    size_t i = 0;
    int in_quotes = 0;
    char quote_char = '\0';

    *error_flag = 0; // Initialisation du drapeau d'erreur

    while (str[i])
    {
        // Ignorer les espaces initiaux
        while (str[i] == ' ')
            i++;

        if (str[i] == '\0')
            break;

        count++;

        // Itérer à travers le token
        while (str[i] && (in_quotes || str[i] != ' '))
        {
            if (!in_quotes && (str[i] == '"' || str[i] == '\''))
            {
                in_quotes = 1;
                quote_char = str[i];
            }
            else if (in_quotes && str[i] == quote_char)
            {
                in_quotes = 0;
                quote_char = '\0';
            }
            i++;
        }
    }

    if (in_quotes)
        *error_flag = 1; // Guillemets non appariés détectés
    return count;
}

char **custom_split(const char *str, int *error_flag)
{
    size_t tokens = count_tokens(str, error_flag);
    if (*error_flag)
        return NULL; // Retourner NULL en cas d'erreur

    char **result = malloc(sizeof(char*) * (tokens + 1));
    if (!result)
        return NULL;

    size_t i = 0, j = 0;
    int in_quotes = 0;
    char quote_char = '\0';
    size_t start = 0;
	size_t k = 0;

    while (str[i])
    {
        // Ignorer les espaces initiaux
        while (str[i] == ' ')
            i++;

        if (str[i] == '\0')
            break;

        start = i;

        // Itérer à travers le token
        while (str[i] && (in_quotes || str[i] != ' '))
        {
            if (!in_quotes && (str[i] == '"' || str[i] == '\''))
            {
                in_quotes = 1;
                quote_char = str[i];
            }
            else if (in_quotes && str[i] == quote_char)
            {
                in_quotes = 0;
                quote_char = '\0';
            }
            i++;
        }

        // Extraire le token
        size_t len = i - start;
        char *token = malloc(len + 1);
        if (!token)
        {
            // Gestion de l'échec d'allocation : libérer la mémoire déjà allouée
			while (k < j)
			{
				free(result[k]);
				k++;
			}
            /* for (size_t k = 0; k < j; k++)
                free(result[k]); */
            free(result);
            return NULL;
        }
        ft_strncpy(token, &str[start], len);
        token[len] = '\0';
        result[j++] = token;
    }

    result[j] = NULL;
    return result;
}

/*
static size_t	strcounter(char const *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '"')
		{
			i++;
			while (s[i] != '"' && s[i] != '\0')
				i++;
		}

		if (s[i] == '\'')
		{
			i++;
			while (s[i] != '\'' && s[i] != '\0')
				i++;
		}
		if (s[i] == c)
			i++;
		else
		{
			while (s[i] != c && s[i] != '\0')
				i++;
			count++;
		}
	}
	return (count);
}

static void	fillarr2(char **arr, size_t len_arr, char const *s, char c)
{
	size_t	i;
	size_t	len_word;
	size_t	in_quotes;

	in_quotes = 0;
	i = 0;
	while (i < len_arr)
	{
		len_word = 0;
		while (*s == c && *s != 0 && !in_quotes)
			s++;
		if (*s == '"' && in_quotes == 0)
		{
			in_quotes = 1;
			len_word++;
		}
		if (*s == '\'' && in_quotes == 0)
		{
			in_quotes = 2;
			len_word++;
		}
		while ((s[len_word] != c || in_quotes) && s[len_word] != '\0')
		{
			if (s[len_word] == '"' && in_quotes == 1)
				in_quotes = 0;
			if (s[len_word] == '\'' && in_quotes == 2)
				in_quotes = 0;
			len_word++;
		}
		arr[i] = ft_substr(s, 0, len_word);
		s += len_word;
		while (*s == c && !in_quotes && *s != 0)
			s++;
		i++;
	}
	arr[i] = 0;
}

char	**ft_split(char const *s, char c)
{
	size_t	len_arr;
	char	**arr;

	if (!s)
		return (0);
	len_arr = strcounter(s, c);
	arr = malloc(sizeof(char *) * (len_arr + 1));
	if (!arr)
		return (NULL);
	fillarr2(arr, len_arr, s, c);
	return (arr);
} */
