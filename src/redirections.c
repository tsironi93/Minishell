/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 15:54:32 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/14 12:32:26 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	_open_and_redirect(t_token *cur, bool flag)
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
	if (!flag)
		return (true);
	if (cur->type == FILENAME_OUT || cur->type == APPEND_FILENAME_OUT)
		dup2(fd, STDOUT_FILENO);
	else
		dup2(fd, STDIN_FILENO);
	close (fd);
	return (true);
}

static bool	_check_redirections(t_token **token, t_token_type type, bool flag)
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
			if (!_open_and_redirect(cur, flag))
				return (false);
		cur = cur->next;
	}
	return (true);
}

bool	redirections(t_data *data, t_token **token, bool flag)
{
	if (flag)
		go_at_start(token);
	if (!_check_redirections(token, FILENAME_INP, flag))
		return (false);
	if (!_check_redirections(token, FILENAME_OUT, flag))
		return (false);
	if (!_check_redirections(token, APPEND_FILENAME_OUT, flag))
		return (false);
	check_heredoc(data, token, flag);
	return (true);
}
