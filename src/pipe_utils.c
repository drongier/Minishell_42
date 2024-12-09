/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:26:33 by drongier          #+#    #+#             */
/*   Updated: 2024/12/09 14:28:03 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(t_parser *parser)
{
	if (parser->infile != STDIN_FILENO)
		close(parser->infile);
	if (parser->outfile != STDOUT_FILENO)
		close(parser->outfile);
}
