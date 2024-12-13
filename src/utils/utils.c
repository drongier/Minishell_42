/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:35:42 by drongier          #+#    #+#             */
/*   Updated: 2024/12/13 16:32:13 by chbachir         ###   ########.fr       */
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
