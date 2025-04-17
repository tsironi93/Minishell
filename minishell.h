/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:28:57 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/16 12:12:15 by turmoil          ###   ########.fr       */
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
# include <errno.h>
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
	FILENAME_INP,
	REDIRECT_OUT,
	FILENAME_OUT,
	UNKNOWN,
	ARGS,
	HERE_DOC,
	HERE_DOC_OPT,
	APPEND,
	APPEND_FILENAME_OUT,
	EXPAND,
	COMMAND_EX,
	NULLL,
	ISSPACE
}	t_token_type;

typedef struct s_gcobj
{
	void			*ptr;
	int				marked;
	struct s_gcobj	*next;
}	t_gcobj;

typedef struct s_gc
{
	t_gcobj	*objects;
	void	**roots[100];
	int		root_count;
}	t_gc;

typedef struct s_env
{
	char			*str;
	struct s_env	*next;
}				t_env;

typedef struct s_tocken
{
	t_token_type	type;
	char			*value;
	struct s_tocken	*next;
	struct s_tocken	*previous;
}				t_token;

typedef struct s_heredoc
{
	char	**del;
	int		num;
	int		**fd;
}				t_heredoc;

typedef struct s_data
{
	t_token		*tokens;
	char		**buildins;
	t_gc		gc;
	char		*input;
	char		**env_full;
	char		**env_cmd_paths;
	t_env		*env;
	int			exit_code;
	t_heredoc	*heredoc;
}				t_data;

//-------------------------------UTILS----------------------------------//

int		ft_isspace(int c);
void	clean(t_data *data, bool exit_);
void	go_at_start(t_token **token);
bool	check_files(t_data *data, t_token **token);
char	*trim_to_del(t_data *data, char *str, char del);

//-----------------------------FREE/ERROR-------------------------------//

void	free_env(t_env **head);
void	free_linked(t_token *head);
void	free2d(char **a);
void	free_fds(t_data *data);
void	*gc_malloc(t_gc *gc, size_t size);
void	gc_destroy(t_gc *gc);
t_gc	gc_new(void);
void	gc_collect(t_gc *gc);
char	*gc_strdup(t_gc *gc, const char *s);
char	*gc_readline(t_gc *gc, const char *prompt);
void	gc_add_root(t_gc *gc, void **ptr);
char	*gc_strjoin(t_gc *gc, char *s1, char *s2);

//------------------------LINKED LIST FUNCTIONS-------------------------//

void	append_node(t_env **head, char *value);
void	append_token(t_data *data, t_token **head, char *value,
			t_token_type type);
t_token	*search_tokens(t_token **token, t_token_type type);
int		num_of_type(t_token **token, t_token_type type, t_token_type until);

//------------------------------INIT------------------------------------//

void	init(int ac, char **av, char **envp, t_data *data);
void	lexer(t_data *data, char *input, t_token **token);
void	expansion(t_token **token, t_data *data);
bool	classify_tokens(t_token **token);
void	merge(t_data *data, t_token **token);

//----------------------------EXECUTION---------------------------------//

void	check_heredoc(t_data *data, t_token **token, bool flag);
void	handle_pipeline(t_data *data, int num_pipes);
bool	redirections(t_data *data, t_token **token, bool flag);
void	try_to_exec(t_data *data, t_token **token);

//--------------------------TESTING_BENCH-------------------------------//

void	print_tokens(t_token **token);
void	print_linked(t_env **ll);
void	print_tokens(t_token **token);
void	check_leaks(void);
void	p(void);

//--------------------------BUILD_INS-------------------------------//
int		env_buildin(t_data *data);
int		pwd_buildin(void);
int		cd_buildin(t_data *data, t_token **token);
void	env_reconstr(t_data *data, char ***env_full);
int 	export_builtin(t_data *data, t_env **env, t_token **token);
int		is_valid_identifier(char *cmd, char *str);
void	update_env(t_data *data, t_env *env, char *prev_pwd, char *next_pwd);
int 	unset_builtin(t_data *data, t_env **env, t_token **token);
int		exit_builtin(t_data *data, t_token **token);

#endif // !MINISHELL_H
