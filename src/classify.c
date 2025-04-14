/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   classify.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:29:14 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/14 12:51:10 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	find_file(t_token *token, t_token_type type)
{
	if (!token->next)
		return (printf("minishell: syntax error near unexpected token `newline'"
				"\n"), false);
	token = token->next;
	while (token && token->type == ISSPACE)
		token = token->next;
	if (token->type == UNKNOWN || token->type == DOUBLE_QUOTES
		|| token->type == SINGLE_QUOTES)
		token->type = type + 1;
	else
		return (printf("minishell: syntax error near unexpected token `newline'"
				"\n"), false);
	return (true);
}

static bool	classify_extra(t_token **token)
{
	t_token	*temp;

	temp = *token;
	temp = search_tokens(token, UNKNOWN);
	if (temp)
		temp->type = COMMAND;
	while (search_tokens(&temp, PIPE))
	{
		temp = search_tokens(&temp, PIPE);
		temp = search_tokens(&temp, UNKNOWN);
		if (temp == NULL)
			return (printf("Syntax error\n"), false);
		temp->type = COMMAND;
	}
	while (search_tokens(token, UNKNOWN))
	{
		temp = search_tokens(token, UNKNOWN);
		temp->type = ARGS;
	}
	return (true);
}

static bool	classify_util(t_token **token, t_token_type type)
{
	t_token	*temp;

	temp = search_tokens(token, type);
	if (type == REDIRECT_INP)
	{
		if (!find_file(temp, REDIRECT_INP))
			return (false);
	}
	else if (type == REDIRECT_OUT)
	{
		if (!find_file(temp, REDIRECT_OUT))
			return (false);
	}
	else if (type == APPEND)
	{
		if (!find_file(temp, APPEND))
			return (false);
	}
	else if (type == HERE_DOC)
	{
		if (!find_file(temp, HERE_DOC))
			return (false);
	}
	*token = temp->next;
	return (true);
}

bool	classify_tokens(t_token **token)
{
	t_token	*temp;

	temp = *token;
	while (search_tokens(&temp, REDIRECT_INP))
		if (!classify_util(&temp, REDIRECT_INP))
			return (false);
	temp = *token;
	while (search_tokens(&temp, REDIRECT_OUT))
		if (!classify_util(&temp, REDIRECT_OUT))
			return (false);
	temp = *token;
	while (search_tokens(&temp, APPEND))
		if (!classify_util(&temp, APPEND))
			return (false);
	temp = *token;
	while (search_tokens(&temp, HERE_DOC))
		if (!classify_util(&temp, HERE_DOC))
			return (false);
	if (!classify_extra(token))
		return (false);
	return (true);
}
