/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:28:57 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/19 16:24:55 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"

# define RESET   "\x1b[0m"
# define OLIVE   "\x1b[2;33m"
# define RED     "\x1b[91m"
# define BLACK   "\x1b[90m"
# define GREEN   "\x1b[92m"
# define YELLOW  "\x1b[93m"
# define BLUE    "\x1b[94m"
# define MAGENTA "\x1b[95m"
# define CYAN    "\x1b[96m"
# define WHITE   "\x1b[97m"

typedef enum token_type
{
	TOKEN_COMMAND,
	TOKEN_STRING,
	TOKEN_PIPE,
	TOKEN_REDIRECT,
	TOKEN_LOGICAL_OP,
	TOKEN_FILENAME,
	TOKEN_UNKNOWN
}	t_token_type;

typedef struct s_tocken
{
	t_token_type	type;
	char			value[256];
	struct s_tocken	*next;
}	t_token;

#endif // !MINISHELL_H
