/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ffds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 20:05:31 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/01 10:13:57 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	_open_files(int *ffd, t_token **token, t_token_type type)
{
	int		num_files;
	int		i;
	t_token	*cur;

	num_files = num_of_type(token, type, PIPE);
	if (num_files < 1)
		return (true);
	ffd = malloc(num_files * sizeof(int));
	if (!ffd)
		return (perror("alloc failed"), false);
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
	return (true);
}

bool	setup_fds(t_data *data, t_token **token)
{
	go_at_start(token);
	print_tokens(token);
	if (!_open_files(data->input_fd, token, FILENAME_INP)
		|| !_open_files(data->output_fd, token, FILENAME_OUT)
		|| !_open_files(data->append_fd, token, APPEND_FILENAME_OUT))
		return (free_fds(data), false);
	return (true);
}
