/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:45:42 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/05 13:32:05 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*_find_exec(t_data *data, char *cmd, char **dirs, bool flag)
{
	char	*path;
	char	*tmp;
	char	*tmp_cmd;

	tmp_cmd = NULL;
	if (!flag && cmd[0] == '/')
		tmp_cmd = trim_to_del(data, cmd, '/');
	else
		tmp_cmd = gc_strdup(&data->gc, cmd);
	while (*dirs)
	{
		tmp = ft_strjoin(*dirs, "/");
		path = ft_strjoin(tmp, tmp_cmd);
		free(tmp);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
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

	if (num_of_type(&data->tokens, COMMAND, NULLL) != 1)
	{
		redirections(data, token);
		execve(cmd_path, cmd, data->env_full);
		printf("execve failed\n");
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			redirections(data, token);
			execve(cmd_path, cmd, data->env_full);
			printf("execve failed");
		}
		waitpid(pid, NULL, 0);
	}
	free(cmd_path);
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
