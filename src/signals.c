/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:50:44 by chbachir          #+#    #+#             */
/*   Updated: 2024/11/18 16:44:17 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sigint(int sig)
{
    (void)sig; // Éviter l'avertissement pour un paramètre non utilisé
    write(STDOUT_FILENO, "\n", 1);
    rl_replace_line("", 0); // Efface la ligne actuelle de readline
    rl_on_new_line();        // Prépare une nouvelle ligne
    rl_redisplay();          // Réaffiche le prompt
}

void setup_signal_handlers(void)
{
    struct sigaction sa;

    // Configurer le gestionnaire pour SIGINT
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // Pour redémarrer les appels système interrompus

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Optionnel: Ignorer SIGQUIT (Ctrl+\)
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}
