/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   classify.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:29:14 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/25 17:19:48 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	find_file(t_data *data, t_token **token, t_token_type type)
{
	t_token	*cur;

	cur = *token;
	if (!cur->next)
		return (ft_putstr_fd("minishell: syntax error near unexpected token ",
				2), ft_putendl_fd("`newline'", 2), data->exit_code = 2, false);
	cur = cur->next;
	if (cur->type == ISSPACE && !cur->next)
		return (ft_putstr_fd("minishell: syntax error near unexpected token ",
				2), ft_putendl_fd("`newline'", 2), data->exit_code = 2, false);
	while (cur && cur->type == ISSPACE)
		cur = cur->next;
	if (cur->type == UNKNOWN || cur->type == DOUBLE_QUOTES
		|| cur->type == SINGLE_QUOTES)
		cur->type = type + 1;
	// else (cur->type == APPEND || cur->type == REDIRECT_INP
	// 	|| cur->type == REDIRECT_OUT || cur->type == HERE_DOC)
	else
		return (ft_putendl_fd("minishell: syntax error near unexpected token",
				2), data->exit_code = 2, false);
	return (true);
}

static bool	classify_extra(t_token **token)
{
	t_token	*temp;

	temp = *token;
	while (temp)
	{
		if (temp->type == PIPE)
			temp = temp->next;
		while (temp && temp->type != PIPE)
		{
			if (temp->type == UNKNOWN || temp->type == DOUBLE_QUOTES)
			{
				temp->type = COMMAND;
				break ;
			}
			temp = temp->next;
		}
		while (temp && temp->type != PIPE)
		{
			if (temp->type == UNKNOWN)
				temp->type = ARGS;
			temp = temp->next;
		}
	}
	return (true);
}

static bool	classify_util(t_data *data, t_token **token, t_token_type type)
{
	t_token	*temp;

	temp = search_tokens(token, type);
	if (type == REDIRECT_INP)
	{
		if (!find_file(data, &temp, REDIRECT_INP))
			return (false);
	}
	else if (type == REDIRECT_OUT)
	{
		if (!find_file(data, &temp, REDIRECT_OUT))
			return (false);
	}
	else if (type == APPEND)
	{
		if (!find_file(data, &temp, APPEND))
			return (false);
	}
	else if (type == HERE_DOC)
	{
		if (!find_file(data, &temp, HERE_DOC))
			return (false);
	}
	if (temp->next)
		*token = temp->next;
	return (true);
}

bool	classify_tokens(t_data *data, t_token **token)
{
	t_token	*temp;

	temp = *token;
	while (search_tokens(&temp, REDIRECT_INP))
		if (!classify_util(data, &temp, REDIRECT_INP))
			return (false);
	temp = *token;
	while (search_tokens(&temp, REDIRECT_OUT))
		if (!classify_util(data, &temp, REDIRECT_OUT))
			return (false);
	temp = *token;
	while (search_tokens(&temp, APPEND))
		if (!classify_util(data, &temp, APPEND))
			return (false);
	temp = *token;
	while (search_tokens(&temp, HERE_DOC))
		if (!classify_util(data, &temp, HERE_DOC))
			return (false);
	if (!classify_extra(token))
		return (false);
	return (true);
}
