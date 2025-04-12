/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:45:42 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/11 18:05:48 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*_find_exec(t_data *data, char *cmd, char **dirs, bool flag)
{
	char	*path;
	char	*tmp;
	char	*tmp_cmd;
	char	buf[1024];

	tmp_cmd = NULL;
	if (!flag && cmd[0] == '/')
		tmp_cmd = trim_to_del(data, cmd, '/');
	else if (cmd[0] == '.' && cmd[1] == '/')
		return (gc_strjoin(&data->gc, getcwd(buf, 1024), ++cmd));
	else
		tmp_cmd = gc_strdup(&data->gc, cmd);
	while (*dirs)
	{
		tmp = ft_strjoin(*dirs, "/");
		path = gc_strjoin(&data->gc, tmp, tmp_cmd);
		free(tmp);
		if (access(path, X_OK) == 0)
			return (path);
		dirs++;
	}
	printf("minishell: %s: command not found\n", tmp_cmd);
	return (NULL);
}

static unsigned int	_num_of_args(t_token **token, t_token_type type)
{
	t_token			*temp;
	unsigned int	i;

	temp = *token;
	i = 0;
	while (temp)
	{
		if (temp->type == type)
			i++;
		temp = temp->next;
	}
	return (i);
}

static void	do_i_fork(t_data *data, t_token **token, char **cmd, char *cmd_path)
{
	int	pid;
	int	status;

	status = 0;
	if (num_of_type(&data->tokens, COMMAND, NULLL) != 1)
	{
		if (!redirections(data, token))
			return ;
		execve(cmd_path, cmd, data->env_full);
		exit (errno);
		perror("execve failed\n");
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			if (!redirections(data, token))
				return ;
			execve(cmd_path, cmd, data->env_full);
			perror("execve failed");
			exit(errno);
		}
		waitpid(pid, &status, 0);
		data->exit_code = WEXITSTATUS(status);
	}
	cmd = NULL;
}

void	try_to_exec(t_data *data, t_token **token)
{
	t_token			*temp;
	char			**cmd;
	char			*cmd_path;
	unsigned int	i;
	bool			flag;

	i = 0;
	flag = false;
	temp = *token;
	while (temp)
	{
		if (temp->type == COMMAND || temp->type == COMMAND_EX)
			break ;
		temp = temp->next;
	}
	if (!temp)
		return ;
	if (temp->type == COMMAND_EX)
		flag = true;
	else
		flag = false;
	cmd_path = _find_exec(data, temp->value, data->env_cmd_paths, flag);
	if (!cmd_path)
		return ;
	cmd = gc_malloc(&data->gc, (_num_of_args(token, ARGS) + 2) * sizeof(char *));
	cmd[i++] = gc_strdup(&data->gc, temp->value);
	while (temp && temp->type != PIPE)
	{
		if (temp->type == ARGS)
			cmd[i++] = gc_strdup(&data->gc, temp->value);
		temp = temp->next;
	}
	cmd[i] = NULL;
	do_i_fork(data, token, cmd, cmd_path);
}
