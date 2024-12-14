/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 00:26:27 by emaydogd          #+#    #+#             */
/*   Updated: 2024/12/14 16:18:45 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*dup;

	dup = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	i = 0;
	if (dup == NULL)
		return (NULL);
	else
	{
		while (s1[i])
		{
			dup[i] = s1[i];
			i++;
		}
		dup[i] = '\0';
	}
	return (dup);
}
