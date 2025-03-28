/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:28:57 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/29 08:27:25 by itsiros          ###   ########.fr       */
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
	COMMAND,
	DOUBLE_QUOTES,
	SINGLE_QUOTES,
	PIPE,
	REDIRECT_INP,
	REDIRECT_OUT,
	FILENAME_INP,
	FILENAME_OUT,
	UNKNOWN,
	ARGS,
	HERE_DOC,
	APPEND_FILENAME_OUT,
	HERE_DOC_OPT,
	APPEND,
	EXPAND
}	t_token_type;

typedef struct s_env
{
	char			*str;
	struct s_env	*next;
}	t_env;

typedef struct s_tocken
{
	t_token_type	type;
	char			*value;
	struct s_tocken	*next;
	struct s_tocken	*previous;
}	t_token;

typedef struct s_data
{
	t_token	*tokens;
	char	*input;
	char	**env_paths;
	t_env	*env;
}		t_data;

//-------------------------------UTILS----------------------------------//

int		ft_isspace(int c);

//-----------------------------FREE/ERROR-------------------------------//

void	free_linked(t_token *head);
void	free2d(char **a);

//------------------------LINKED LIST FUNCTIONS-------------------------//

void	append_node(t_env **head, char *value);
void	append_token(t_token **head, char *value, t_token_type type);
t_token	*search_tokens(t_token **token, t_token_type type);
size_t	num_of_type(t_token **token, t_token_type type);

//------------------------------INIT------------------------------------//

void	lexer(char *input, t_token **token);
void	expansion(t_token **token, t_env **env);
bool	classify_tokens(t_token **token);

//----------------------------EXECUTION---------------------------------//

void	handle_pipeline(t_data *data, int num_pipes);
void	try_to_exec(t_data *data, t_token **token);

//--------------------------TESTING_BENCH-------------------------------//

void	print_tokens(t_token **token);
void	print_linked(t_env **ll);
void	print_tokens(t_token **token);
void	check_leaks(void);
void	p(void);

#endif // !MINISHELL_H
