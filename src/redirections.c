/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 15:54:32 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/10 16:50:35 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	_open_and_redirect(t_token *cur)
{
	int	fd;

	fd = -1;
	if (cur->type == FILENAME_OUT)
		fd = open(cur->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (cur->type == FILENAME_INP)
		fd = open(cur->value, O_RDONLY);
	else if (cur->type == APPEND_FILENAME_OUT)
		fd = open(cur->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror("Minishell"), false);
	if (cur->type == FILENAME_OUT || cur->type == APPEND_FILENAME_OUT)
		dup2(fd, STDOUT_FILENO);
	else
		dup2(fd, STDIN_FILENO);
	close (fd);
	return (true);
}

static bool	_check_redirections(t_token **token, t_token_type type)
{
	int		num_files;
	t_token	*cur;

	num_files = num_of_type(token, type, PIPE);
	if (num_files < 1)
		return (true);
	cur = *token;
	while (cur && cur->type != PIPE)
	{
		if (cur->type == type)
			if (!_open_and_redirect(cur))
				return (false);
		cur = cur->next;
	}
	return (true);
}

bool	redirections(t_data *data, t_token **token)
{
	//go_at_start(token);
	if (!_check_redirections(token, FILENAME_INP))
		return (false);
	if (!_check_redirections(token, FILENAME_OUT))
		return (false);
	if (!_check_redirections(token, APPEND_FILENAME_OUT))
		return (false);
	check_heredoc(data, token);
	return (true);
}
