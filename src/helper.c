/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:10:57 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/16 15:33:14 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**list_to_array(t_list *args)
{
	t_list	*args2;
	char	**str;
	int		i;

	args2 = args;
	if (!args2)
		return NULL;
	str = malloc(sizeof(char *) * (ft_lstsize(args2) + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (args2)
	{
		str[i] = remove_quotes((char *)args2->content);
		if (!str[i])
		{
			while (i > 0)
				free(str[--i]);
			free(str);
			return (NULL);
		}
		args2 = args2->next;
		i++;
	}
	str[i] = NULL;
	return (str);
}
