/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   classify.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:29:14 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/01 15:26:44 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>

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
	if (temp->next && type == REDIRECT_INP && temp->next->type == UNKNOWN)
		temp->next->type = FILENAME_INP;
	else if (temp->next && type == REDIRECT_OUT && temp->next->type == UNKNOWN)
		temp->next->type = FILENAME_OUT;
	else if (temp->next && type == APPEND && temp->next->type == UNKNOWN)
		temp->next->type = APPEND_FILENAME_OUT;
	else if (temp->next && type == HERE_DOC && temp->next->type == UNKNOWN)
		temp->next->type = HERE_DOC_OPT;
	else
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (false);
	}
	temp = temp->next;
	*token = temp;
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
