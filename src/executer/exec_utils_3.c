/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 12:12:24 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/16 13:15:00 by drongier         ###   ########.fr       */
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

int	is_delim(char c, char delim)
{
	return (c == delim);
}

int	count_words(const char *str, char delim)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
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

char	*malloc_word(const char *str, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(sizeof(char) * (end - start + 1));
	if (!word)
		return (NULL);
	while (start < end)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}
