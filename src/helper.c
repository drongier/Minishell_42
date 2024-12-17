/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:10:57 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/17 18:02:51 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**list_to_array(t_list *args)
{
	char	**str;
	int		i;

	if (!args)
		return (NULL);
	str = malloc(sizeof(char *) * (ft_lstsize(args) + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (args)
	{
		str[i] = remove_quotes((char *)args->content);
		if (!str[i])
		{
			while (i > 0)
				free(str[--i]);
			free(str);
			return (NULL);
		}
		args = args->next;
		i++;
	}
	str[i] = NULL;
	return (str);
}
