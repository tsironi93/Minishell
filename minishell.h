/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:28:57 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/26 20:01:12 by itsiros          ###   ########.fr       */
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
# include <limits.h>

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

extern volatile sig_atomic_t	g_child_pid;

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
t_env	*copy_env_list(t_env *env);
char	*ft_getenv(t_env **env, char *var);
int		ft_strcmp(const char *s1, const char *s2);

//-----------------------------FREE/ERROR-------------------------------//

void	free_env(t_env **head);
//void	free_linked(t_token *head);
void	free2d(char **a);
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
void	bullshit(t_data *data, t_token **token, int *i, char *input);
char	*wraper_sign(t_data *data, char *input, int *i);
char	*wraper_quotes(t_data *data, char *input, int *i, char c);
void	bullshit2(t_data *data, t_token **token, int *i, char *input);
void	expansion(t_token **token, t_data *data);
void	expand_var_helper(t_data *data, char *input, int *i, char **exp_result);
void	save_in_buffer(t_data *data, char *input, int *i, char **exp_result);
void	_append_expansion(t_data *data, char **arg, char **name);
char	*_find_env_path(char **env_list, char *search_key);
void	dq_expansion(t_token *cur);
bool	classify_tokens(t_data *data, t_token **token);
void	merge(t_data *data, t_token **token);
void	_expand_variables(t_data *data, char *input, char **exp_result);
bool	check_quotes(char *input);

//-----------------------------SIGNALS----------------------------------//

void	sigint_handler(int sig);
void	sigtstp_handler(int sig);
void	sigchld_handler(int sig);
void	restore_terminal(void);
void	disable_echoctl(void);

//----------------------------EXECUTION---------------------------------//

void	check_heredoc(t_data *data, t_token **token, bool flag);
void	handle_pipeline(t_data *data, int num_pipes);
bool	redirections(t_data *data, t_token **token, bool flag);
void	try_to_exec(t_data *data, t_token **token);
void	execute_one_cmd(t_data *data, t_token *temp, char *cmd_path,
			char **cmd);
void	fork_single_command(t_data *data, char *cmd_path, char **cmd, int pid);
void	execute_buildin(t_data *data, t_token **token, t_token *temp);
void	finished_child(t_data *data, int pid);
char	*_find_exec(t_data *data, char *cmd, char **dirs, bool flag);
bool	check_buildin(t_data *data, char *cmd);
void	to_buildin(t_data *data, char *cmd, t_token **token);

//--------------------------BUILTINS-------------------------------//
int		env_buildin(t_data *data, t_token **t_token);
int		pwd_buildin(t_data *data);
int		cd_buildin(t_data *data, t_token **token);
void	env_reconstr(t_data *data, char ***env_full);
int		export_builtin(t_data *data, t_env **env, t_token **token);
int		is_valid_identifier(t_data *data, char *cmd, char *str);
void	update_env(t_data *data, t_env *env, char *prev_pwd, char *next_pwd);
int		unset_builtin(t_data *data, t_env **env, t_token **token);
int		exit_builtin(t_data *data, t_token **token);
int		echo_builtin(t_data *data, t_token **token);

//--------------------------TESTING_BENCH-------------------------------//
char	*get_next_line(int fd);

#endif // !MINISHELL_H
