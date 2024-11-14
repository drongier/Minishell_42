/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:23:59 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/14 18:25:18 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    call_prompt(int key)
{
    (void)key;
    rl_replace_line("", 0);
    rl_on_new_line();
    write(1, "\n", 1);
    rl_redisplay();
    //update_variable_status_process(g_env, 130);
}
void    pass_cmd(int key)
{
    (void)key;
    rl_on_new_line();
}
void    exit_prompt(int key)
{
    (void)key;
    printf("Exit...\n");
    exit(0);
}
void    use_signals(void)
{
    signal(SIGQUIT, pass_cmd);
    signal(SIGINT, call_prompt);
}
