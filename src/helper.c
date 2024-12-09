/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:10:57 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/09 13:35:11 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim(char *str)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
		{
			new[j] = str[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
	return (new);
}

char	**list_to_array(t_list *args)
{
	t_list	*args2;
	char	**str;
	int		i;

	args2 = args;
	str = malloc(sizeof(char *) * (ft_lstsize(args2) + 2));
	i = 0;
	while (args2)
	{
		str[i] = remove_quotes((char *)args2->content);
		args2 = args2->next;
		i++;
	}
	str[i] = NULL;
	return (str);
}
