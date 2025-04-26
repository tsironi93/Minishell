/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:33:49 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/26 15:04:26 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_child_pid = -1;

void	disable_echoctl(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_terminal(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	sigint_handler(int sig)
{
	(void)sig;
	if (g_child_pid > 0)
		kill(g_child_pid, SIGINT);
	else
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sigtstp_handler(int sig)
{
	(void)sig;
	if (g_child_pid > 0)
		kill(g_child_pid, SIGTSTP);
	else
		printf("\n");
}

void	sigchld_handler(int sig)
{
	int	i;

	i = 0;
	(void)sig;
	while (waitpid(-1, NULL, WNOHANG) > 0)
		i++;
}
