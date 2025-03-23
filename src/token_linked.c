/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_linked.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:14:37 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/23 17:44:59 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_linked(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free (temp->value);
		free (temp);
	}
}

void	close_pros(t_data *data)
{
	free_linked(data->tokens);
	free (data->input);
	exit (EXIT_SUCCESS);
}

static t_token	*_create_token(char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	return (new_token);
}

void	append_token(t_token **head, char *value, t_token_type type)
{
	t_token	*new_token;
	t_token	*temp;

	new_token = _create_token(value, type);
	if (!new_token)
		return ;
	if (*head == NULL)
	{
		new_token->previous = NULL;
		*head = new_token;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
	new_token->previous = temp;
}

t_token	*search_tokens(t_token **token, t_token_type type)
{
	t_token	*temp;

	if (!token || !*token)
		return (NULL);
	temp = *token;
	while (temp)
	{
		if (temp->type == type)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}
