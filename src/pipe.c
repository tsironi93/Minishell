/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:36:48 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/22 11:16:32 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	close_fds(int fd[][2], int num_pipes)
{
	int	i;

	i = -1;
	while (++i < num_pipes)
	{
		close(fd[i][0]);
		close(fd[i][1]);
	}
}

static t_token	*search_next_command(t_token **token)
{
	t_token	*temp;

	temp = *token;
	while (temp && temp->type != PIPE)
		temp = temp->next;
	if (temp && temp->type == PIPE)
		temp = temp->next;
	while (temp && temp->type != COMMAND)
		temp = temp->next;
	return (temp);
}

static void	redirect_pipe(int fd[][2], int num_pipes, int p_index, t_data *data)
{
	t_token	*temp;
	int		i;

	temp = data->tokens;
	i = -1;
	while (++i < p_index)
		temp = search_next_command(&temp);
	if (p_index > 0)
	{
		close(fd[p_index - 1][1]);
		dup2(fd[p_index - 1][0], STDIN_FILENO);
		close(fd[p_index - 1][0]);
	}
	if (p_index < num_pipes)
	{
		close(fd[p_index][0]);
		dup2(fd[p_index][1], STDOUT_FILENO);
		close(fd[p_index][1]);
	}
	close_fds(fd, num_pipes);
	try_to_exec(data, &temp);
}

void	handle_pipeline(t_data *data, int num_pipes)
{
	int		fd[64][2];
	pid_t	pid[65];
	int		i;

	i = -1;
	while (++i < num_pipes)
		if (pipe(fd[i]) == -1)
			return ((void)perror("pipe"));
	i = -1;
	while (++i < num_pipes + 1)
	{
		pid[i] = fork();
		if (pid[i] < 0)
			return ((void)perror("fork"));
		if (pid[i] == 0)
			redirect_pipe(fd, num_pipes, i, data);
	}
	close_fds(fd, num_pipes);
	i = -1;
	while (++i < num_pipes + 1)
		waitpid(pid[i], NULL, 0);
}
