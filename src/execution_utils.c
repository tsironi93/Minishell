/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:25:26 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/26 18:28:51 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*_find_exec(t_data *data, char *cmd, char **dirs, bool flag)
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
	data->exit_code = 127;
	perror("command not found");
	return (NULL);
}

void	finished_child(t_data *data, int pid)
{
	int	status;

	status = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sigint_handler);
	g_child_pid = pid;
	waitpid(pid, &status, 0);
	g_child_pid = -1;
	if (WIFEXITED(status))
		data->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_code = 128 + WTERMSIG(status);
	else
		data->exit_code = 1;
}

void	execute_buildin(t_data *data, t_token **token, t_token *temp)
{
	if (!redirections(data, token, true))
		return ;
	to_buildin(data, temp->value, &temp);
}

void	fork_single_command(t_data *data, char *cmd_path, char **cmd, int pid)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (pid == 0)
	{
		if (!redirections(data, &data->tokens, true))
			return ;
		execve(cmd_path, cmd, data->env_full);
		perror("execve failed");
		exit(errno);
	}
	else if (pid > 0)
		finished_child(data, pid);
}

void	execute_one_cmd(t_data *data, t_token *temp, char *cmd_path, char **cmd)
{
	int	pid;

	if (check_buildin(data, temp->value))
		return (execute_buildin(data, &data->tokens, temp));
	pid = fork();
	fork_single_command(data, cmd_path, cmd, pid);
}
