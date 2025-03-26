/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   classify.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:29:14 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/26 10:18:53 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	classify_extra(t_token **token)
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
		temp->type = COMMAND;
	}
	while (search_tokens(token, UNKNOWN))
	{
		temp = search_tokens(token, UNKNOWN);
		temp->type = ARGS;
	}
}

static void	classify_util(t_token **token, t_token_type type, bool *syntax)
{
	t_token	*temp;

	temp = search_tokens(token, type);
	if (!temp->next)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		*syntax = false;
		*token = temp->next;
		return ;
	}
	if (type == REDIRECT_INP)
		temp->next->type = FILENAME_INP;
	else if (type == REDIRECT_OUT)
		temp->next->type = FILENAME_OUT;
	else if (type == APPEND)
		temp->next->type = APPEND_FILENAME_OUT;
	else if (type == HERE_DOC)
		temp->next->type = HERE_DOC_OPT;
	temp = temp->next;
	*token = temp;
}

bool	classify_tokens(t_token **token)
{
	t_token	*temp;
	bool	syntax;

	syntax = true;
	temp = *token;
	while (search_tokens(&temp, REDIRECT_INP))
		classify_util(&temp, REDIRECT_INP, &syntax);
	temp = *token;
	while (search_tokens(&temp, REDIRECT_OUT))
		classify_util(&temp, REDIRECT_OUT, &syntax);
	temp = *token;
	while (search_tokens(&temp, APPEND))
		classify_util(&temp, APPEND, &syntax);
	temp = *token;
	while (search_tokens(&temp, HERE_DOC))
		classify_util(&temp, HERE_DOC, &syntax);
	classify_extra(token);
	return (syntax);
}
