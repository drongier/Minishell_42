/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 10:18:44 by chbachir          #+#    #+#             */
/*   Updated: 2024/12/12 11:24:01 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_quotes(char c, int *in_quotes, char *new_str, size_t *j)
{
	if (c == '"' || c == '\'')
	{
		*in_quotes = !*in_quotes;
		new_str[(*j)++] = c;
	}
}

static void	handle_red(const char *cmd, size_t *i, char *new_str, size_t *j)
{
	if (*i > 0 && !isspace(cmd[*i - 1]))
		new_str[(*j)++] = ' ';
	new_str[(*j)++] = cmd[*i];
	if (cmd[*i] == '>' && cmd[*i + 1] == '>')
	{
		new_str[(*j)++] = cmd[++(*i)];
	}
	if (cmd[*i] == '<' && cmd[*i + 1] == '<')
	{
		new_str[(*j)++] = cmd[++(*i)];
	}
	if (cmd[*i + 1] && !isspace(cmd[*i + 1]))
		new_str[(*j)++] = ' ';
}

static char	*init_cmdline(const char *cmdline, size_t *i, size_t *j)
{
	char	*new_cmdline;

	if (!cmdline)
		return (NULL);
	new_cmdline = malloc(ft_strlen(cmdline) * 3 + 1);
	if (!new_cmdline)
		return (NULL);
	*i = 0;
	*j = 0;
	return (new_cmdline);
}

static void	process_cmdline(const char *cmd, char *new, size_t *i, size_t *j)
{
	int	in_quotes;

	in_quotes = 0;
	while (cmd[*i])
	{
		if (cmd[*i] == '"' || cmd[*i] == '\'')
			handle_quotes(cmd[*i], &in_quotes, new, j);
		else if (!in_quotes && (cmd[*i] == '|' || cmd[*i] == '<'
				|| cmd[*i] == '>'))
			handle_red(cmd, i, new, j);
		else
			new[(*j)++] = cmd[*i];
		(*i)++;
	}
	new[*j] = '\0';
}

char	*add_spaces_around_redirection(const char *cmdline)
{
	size_t	i;
	size_t	j;
	char	*new_cmdline;
	char	*result;

	new_cmdline = init_cmdline(cmdline, &i, &j);
	if (!new_cmdline)
		return (NULL);
	process_cmdline(cmdline, new_cmdline, &i, &j);
	result = ft_strdup(new_cmdline);
	free(new_cmdline);
	return (result);
}
