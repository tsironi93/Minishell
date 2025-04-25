/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:33:49 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/25 10:29:45 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t   g_child_pid = -1;

void disable_echoctl(void)
{
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);        // Get current terminal settings
    term.c_lflag &= ~ECHOCTL;               // Disable echoing control characters like ^C
    tcsetattr(STDIN_FILENO, TCSANOW, &term); // Apply changes immediately

    signal(SIGQUIT, SIG_IGN);  // Ignore Ctrl+"\"

	// struct termios term;

    // // Get current terminal settings
    // if (tcgetattr(STDIN_FILENO, &term) == -1)
    // {
    //     perror("tcgetattr");
    //     return;
    // }

    // // Disable ECHOCTL (echoing control characters like ^C)
    // term.c_lflag &= ~ECHOCTL;

    // // Apply changes immediately
    // if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
    // {
    //     perror("tcsetattr");
    //     return;
    // }

    // // Ignore SIGQUIT (Ctrl+\)
    // signal(SIGQUIT, SIG_IGN);
}

void restore_terminal(void)
{
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Forward Ctrl+C to child
void    sigint_handler(int sig)
{
    (void)sig;
    if (g_child_pid > 0)
        kill(g_child_pid, SIGINT);
    else
    {
        write(1, "\n", 1);               // Print newline
        rl_replace_line("", 0);         // Clear current input line
        rl_on_new_line();               // Move cursor to start of new line
        rl_redisplay();                 // Redraw the prompt
    }
}

// Forward Ctrl+Z to child
void    sigtstp_handler(int sig)
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
