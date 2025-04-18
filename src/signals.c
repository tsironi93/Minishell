/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:33:49 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/18 16:52:53 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_child_pid = -1;

// Forward Ctrl+C to child
void	sigint_handler(int sig)
{
	(void)sig;
	if (g_child_pid > 0)
		kill(g_child_pid, SIGINT);
	else
		printf("\n");
}

// Forward Ctrl+Z to child
void	sigtstp_handler(int sig)
{
	(void)sig;
	if (g_child_pid > 0)
		kill(g_child_pid, SIGTSTP);
	else
		printf("\n");
}

// Reap all dead children
void	sigchld_handler(int sig)
{
	(void)sig;
	while (waitpid(-1, NULL, WNOHANG) > 0);
}
