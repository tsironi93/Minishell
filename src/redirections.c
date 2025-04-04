/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 15:54:32 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/04 12:52:52 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	redirect_fds(t_token_type type, int *fd, int num_fds)
{
	int	i;

	i = -1;
	if (type == FILENAME_OUT || type == APPEND_FILENAME_OUT)
		while (++i < num_fds)
			dup2(fd[i], STDOUT_FILENO);
	else
		while (++i < num_fds)
			dup2(fd[i], STDIN_FILENO);
}

static bool	_open_files(int *ffd, t_token **token, t_token_type type)
{
	int		num_files;
	int		i;
	t_token	*cur;

	num_files = num_of_type(token, type, PIPE);
	if (num_files < 1)
		return (true);
	i = -1;
	cur = *token;
	while (cur && cur->type != PIPE)
	{
		if (cur->type == type && type == FILENAME_OUT)
			ffd[++i] = open(cur->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (cur->type == type && type == FILENAME_INP)
			ffd[++i] = open(cur->value, O_RDONLY);
		else if (cur->type == type && type == APPEND_FILENAME_OUT)
			ffd[++i] = open(cur->value, O_WRONLY | O_APPEND, 0644);
		if (!ffd && ffd[i] < 0)
			return (perror("Minishell"), false);
		cur = cur->next;
	}
	return (redirect_fds(type, ffd, num_files), true);
}

bool	redirections(t_data *data, t_token **token)
{
	go_at_start(token);
	if (!_open_files(data->input_fd, token, FILENAME_INP)
		|| !_open_files(data->output_fd, token, FILENAME_OUT)
		|| !_open_files(data->append_fd, token, APPEND_FILENAME_OUT))
		return (false);
	check_heredoc(data, token);
	return (true);
}
