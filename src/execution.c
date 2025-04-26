/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:45:42 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/26 18:29:36 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_buildin(t_data *data, char *cmd)
{
	int	i;

	i = -1;
	while (data->buildins[++i])
		if (!ft_strcmp(data->buildins[i], cmd))
			return (true);
	return (false);
}

void	to_buildin(t_data *data, char *cmd, t_token **token)
{
	if (!ft_strcmp(cmd, "pwd"))
		pwd_buildin(data);
	else if (!ft_strcmp(cmd, "env"))
		env_buildin(data, token);
	else if (!ft_strcmp(cmd, "cd"))
		cd_buildin(data, token);
	else if (!ft_strcmp(cmd, "export"))
		export_builtin(data, &data->env, token);
	else if (!ft_strcmp(cmd, "unset"))
		unset_builtin(data, &data->env, token);
	else if (!ft_strcmp(cmd, "exit"))
		exit_builtin(data, token);
	else if (!ft_strcmp(cmd, "echo"))
		echo_builtin(data, token);
}

static void	do_i_fork(t_data *data, t_token **token, char **cmd, char *cmd_path)
{
	t_token	*temp;
	int		status;

	status = 0;
	temp = search_tokens(token, COMMAND);
	if (num_of_type(&data->tokens, COMMAND, NULLL) != 1)
	{
		if (!redirections(data, token, true))
			return ;
		if (check_buildin(data, temp->value))
		{
			to_buildin(data, temp->value, &temp);
			exit (data->exit_code);
		}
		else
		{
			execve(cmd_path, cmd, data->env_full);
			data->exit_code = WEXITSTATUS(status);
			perror("execve failed\n");
			exit (data->exit_code);
		}
	}
	else
		execute_one_cmd(data, temp, cmd_path, cmd);
	cmd = NULL;
}

static void	not_a_buildin(t_data *data, t_token **token, t_token *temp,
		bool flag)
{
	char	*cmd_path;
	char	**cmd;
	int		i;

	i = 0;
	cmd = NULL;
	cmd_path = NULL;
	cmd_path = _find_exec(data, temp->value, data->env_cmd_paths, flag);
	if (!cmd_path)
		return ;
	cmd = gc_malloc(&data->gc, (num_of_type(token, ARGS, NULLL) + 2)
			* sizeof(char *));
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

void	try_to_exec(t_data *data, t_token **token)
{
	t_token			*temp;
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
	if (!temp || !ft_strcmp(temp->value, ""))
		return ;
	if (temp->type == COMMAND_EX)
		flag = true;
	else
		flag = false;
	if (!check_buildin(data, temp->value))
	{
		not_a_buildin(data, token, temp, flag);
		return ;
	}
	do_i_fork(data, token, NULL, NULL);
}
