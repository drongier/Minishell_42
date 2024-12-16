/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:35:42 by drongier          #+#    #+#             */
/*   Updated: 2024/12/16 15:25:34 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_quotes(char c, t_quotes *q)
{
	if (c == '\'' && !q->double_q)
		q->single = !q->single;
	else if (c == '\"' && !q->single)
		q->double_q = !q->double_q;
}

char	*remove_quotes(char *str)
{
	size_t		i;
	size_t		j;
	char		*result;
	t_quotes	q;

	i = 0;
	j = 0;
	result = malloc(ft_strlen(str) + 1);
	q.double_q = 0;
	q.single = 0;
	if (!str)
		return (NULL);
	if (!result)
		return (NULL);
	while (str[i])
	{
		update_quotes(str[i], &q);
		if ((str[i] != '\'' || q.double_q) && (str[i] != '\"' || q.single))
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

int	skip_sp(const char *in, int i)
{
	while (in[i] && ft_isspace(in[i]))
		i++;
	return (i);
}

int	handle_redirection(const char *in, int i)
{
	char	current;
	char	next;

	current = in[i];
	next = in[i + 1];
	if ((current == '>' || current == '<') && next == current)
		i++;
	i++;
	i = skip_sp(in, i);
	if (!in[i] || in[i] == '>' || in[i] == '<')
		return (-1);
	return (i);
}

int	is_valid_syntax(const char *in)
{
	int	i;

	i = 0;
	while (in[i])
	{
		i = skip_sp(in, i);
		if (in[i] == '>' || in[i] == '<')
		{
			i = handle_redirection(in, i);
			if (i == -1)
				return (0);
		}
		else
		{
			while (in[i] && in[i] != '>' && in[i] != '<')
				i++;
		}
	}
	return (1);
}
