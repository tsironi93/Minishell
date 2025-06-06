/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_linked.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:14:37 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/06 17:23:45 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_linked(t_token *head)
{
	t_token	*temp;

	if (!head)
		return ;
	while (head)
	{
		temp = head;
		head = head->next;
		if (!temp->value)
			free (temp->value);
		free (temp);
	}
}

int	num_of_type(t_token **token, t_token_type type, t_token_type until)
{
	int		i;
	t_token	*temp;

	i = 0;
	temp = *token;
	while (temp)
	{
		if (temp->type == type && temp->type != until)
			i++;
		temp = temp->next;
	}
	return (i);
}

static	t_token	*_create_token(t_data *data, char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = gc_malloc(&data->gc, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = gc_strdup(&data->gc, value);
	new_token->next = NULL;
	return (new_token);
}

void	append_token(t_data *data, t_token **head, char *value,
				t_token_type type)
{
	t_token	*new_token;
	t_token	*temp;

	new_token = _create_token(data, value, type);
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
