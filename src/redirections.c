/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 15:54:32 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/30 16:49:14 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	_check_redirects(int *redirect_fd, t_token **token, t_token_type type)
{
	t_token	*cur;
	int		i;
	int		num_redirects;

	num_redirects = 0;
	cur = *token;
	while (cur && cur->type != PIPE)
	{
		if (cur->type == type)
			num_redirects++;
		cur = cur->next;
	}
	if (num_redirects == 0)
		return ;
	redirect_fd = malloc(num_redirects * sizeof(int));
	if (!redirect_fd)
		return ((void)perror("alloc failed"));
	if (type == REDIRECT_OUT)
	{
		cur = *token;
		i = -1;
		while (cur && cur->type != PIPE)
		{
			if (cur->type == FILENAME_OUT)
			{
				redirect_fd[++i] = open(cur->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (redirect_fd[i] < 0)
					return ((void)perror("file didnt open"));
			}
			cur = cur->next;
		}
		i = -1;
		while (++i < num_redirects)
		{
			dup2(redirect_fd[i], STDOUT_FILENO);
			close(redirect_fd[i]);
		}
	}
	else if (type == REDIRECT_INP)
	{
		cur = *token;
		i = -1;
		while (cur && cur->type != PIPE)
		{
			if (cur->type == FILENAME_INP)
			{
				redirect_fd[++i] = open(cur->value, O_RDONLY);
				if (redirect_fd[i] < 0)
					return ((void)perror("file didnt open"));
			}
			cur = cur->next;
		}
		i = -1;
		while (++i < num_redirects)
		{
			dup2(redirect_fd[i], STDIN_FILENO);
			close(redirect_fd[i]);
		}
	}
	else if (type == APPEND)
	{
		cur = *token;
		i = -1;
		while (cur && cur->type != PIPE)
		{
			if (cur->type == APPEND_FILENAME_OUT)
			{
				redirect_fd[++i] = open(cur->value, O_WRONLY | O_APPEND, 0644);
				if (redirect_fd[i] < 0)
					return ((void)perror("file didnt open"));
			}
			cur = cur->next;
		}
		i = -1;
		while (++i < num_redirects)
		{
			dup2(redirect_fd[i], STDOUT_FILENO);
			close(redirect_fd[i]);
		}
	}
}

static void	_go_at_start(t_token **token)
{
	t_token	*cur;

	cur = *token;
	while (cur->previous && cur->previous->type != PIPE)
		cur = cur->previous;
	*token = cur;
}

void	redirections(t_data *data, t_token **token)
{
	_go_at_start(token);
	_check_redirects(data->input_fd, token, REDIRECT_INP);
	_check_redirects(data->output_fd, token, REDIRECT_OUT);
	_check_redirects(data->append_fd, token, APPEND);
}
