/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:45:42 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/29 08:28:16 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*_find_exec(char *cmd, char **dirs)
{
	char	*path;
	char	*tmp;

	while (*dirs)
	{
		tmp = ft_strjoin(*dirs, "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		dirs++;
	}
	//error_handler("command not found", errno);
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

static void	do_i_fork(t_data *data, char **cmd, char *cmd_path)
{
	int	pid;

	if (!(num_of_type(&data->tokens, COMMAND) == 1))
	{
		execve(cmd_path, cmd, data->env_paths);
		printf("execve failed");
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			execve(cmd_path, cmd, data->env_paths);
			printf("execve failed");
		}
		waitpid(pid, NULL, 0);
	}
	free2d(cmd);
	free(cmd_path);
	cmd = NULL;
}

void	try_to_exec(t_data *data, t_token **token)
{
	t_token			*temp;
	char			**cmd;
	char			*cmd_path;
	unsigned int	i;

	i = 0;
	temp = *token;
	temp = search_tokens(token, COMMAND);
	if (!temp)
		return ;
	cmd_path = _find_exec(temp->value, data->env_paths);
	if (!cmd_path)
		return ;
	cmd = (char **)malloc((_num_of_args(token, ARGS) + 2) * sizeof(char *));
	cmd[i++] = ft_strdup(temp->value);
	while (temp)
	{
		if (temp->type == PIPE)
			break ;
		if (temp->type == ARGS)
			cmd[i++] = ft_strdup(temp->value);
		temp = temp->next;
	}
	cmd[i] = NULL;
	do_i_fork(data, cmd, cmd_path);
}
