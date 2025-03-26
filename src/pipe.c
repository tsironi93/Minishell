/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:36:48 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/26 17:07:53 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	asd(t_token **token, t_data *data, size_t num)
{
	t_token	*temp;
	int		i;
	int		fd[2];
	int		pid1;
	int		pid2;
	int		pid[num];

	i = num;
	temp = search_tokens(token, COMMAND);
	if (i == 0)
		return ;
	if (pipe(fd) == -1)
		return ;
	pid1 = fork();
	if (pid1 < 0)
		return ;
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		try_to_exec(data, &temp, data->env_paths);
	}
	temp = search_tokens(token, PIPE);
	temp = search_tokens(&temp, COMMAND);
	pid2 = fork();
	if (pid2 < 0)
		return ;
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		try_to_exec(data, &temp, data->env_paths);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}
