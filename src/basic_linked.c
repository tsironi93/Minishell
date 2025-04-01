/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_linked.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:26:31 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/31 10:24:37 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env(t_env **head)
{
	t_env	*temp;

	while (*head)
	{
		temp = *head;
		*head = (*head)->next;
		free (temp->str);
		free (temp);
	}
}

static t_env	*_create_node(char *value)
{
	t_env	*head;

	head = malloc(sizeof(t_env));
	if (!head)
		return (NULL);
	head->str = ft_strdup(value);
	head->next = NULL;
	return (head);
}

void	append_node(t_env **head, char *value)
{
	t_env	*new_node;
	t_env	*temp;

	new_node = _create_node(value);
	if (!new_node)
		return ;
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
}
