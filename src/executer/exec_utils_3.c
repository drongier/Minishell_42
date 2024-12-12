/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 12:12:24 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/12 12:30:40 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_command(t_shell *shell, char *cmd_path, int is_direct_path)
{
	struct stat		cmd_stat;

	if (stat(cmd_path, &cmd_stat) == 0)
	{
		if (S_ISDIR(cmd_stat.st_mode))
		{
			ft_error(shell, "minishell: %s: Is a directory\n", cmd_path, 126);
			return (0);
		}
		if (access(cmd_path, X_OK) != 0)
		{
			ft_error(shell, "minishell: %s: "PD"\n", cmd_path, 126);
			return (0);
		}
		return (1);
	}
	else
	{
		if (is_direct_path)
			ft_error(shell, "minishell: %s: "NSFOD"\n", cmd_path, 127);
		return (0);
	}
}

int	handle_redirections(t_shell *shell, int *saved_out, int *saved_in)
{
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_out == -1)
		return (0);
	*saved_in = dup(STDIN_FILENO);
	if (*saved_in == -1)
	{
		close(*saved_out);
		return (0);
	}
	if (shell->parser->outfile != STDOUT_FILENO)
		dup2(shell->parser->outfile, STDOUT_FILENO);
	if (shell->parser->infile != STDIN_FILENO)
		dup2(shell->parser->infile, STDIN_FILENO);
	return (1);
}

// ft_split.c

#include <stdlib.h>

static int	is_delim(char c, char delim)
{
	return (c == delim);
}

static int	count_words(const char *str, char delim)
{
	int	count = 0;
	int i = 0;

	while (str[i])
	{
		while (str[i] && is_delim(str[i], delim))
			i++;
		if (str[i] && !is_delim(str[i], delim))
		{
			count++;
			while (str[i] && !is_delim(str[i], delim))
				i++;
		}
	}
	return (count);
}

static char	*malloc_word(const char *str, int start, int end)
{
	char	*word;
	int		i = 0;

	word = malloc(sizeof(char) * (end - start + 1));
	if (!word)
		return (NULL);
	while (start < end)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

char	**ft_exec_split(const char *s, char c)
{
	char	**result;
	int		i = 0;
	int		j = 0;
	int		index = 0;

	if (!s)
		return (NULL);
	result = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!result)
		return (NULL);
	while (s[i])
	{
		while (s[i] && is_delim(s[i], c))
			i++;
		j = i;
		while (s[i] && !is_delim(s[i], c))
			i++;
		if (j < i)
			result[index++] = malloc_word(s, j, i);
	}
	result[index] = NULL;
	return (result);
}