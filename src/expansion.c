/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:41:19 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/23 17:08:43 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*expand(char *str)
{
	return (str);
}

void	expansion(t_token **token)
{
	t_token *temp;

	temp = *token;
	while (search_tokens(&temp, SINGLE_QUOTES))
	{
		temp = search_tokens(&temp, SINGLE_QUOTES);
		temp->value = ft_strtrim(temp->value, "'");
		temp->type = ARGS;
	}
	temp = *token;
	while (search_tokens(&temp, DOUBLE_QUOTES))
	{
		temp = search_tokens(&temp, DOUBLE_QUOTES);
		if (ft_strchr(temp->value, '$'))
			temp->value = expand(temp->value);
		temp->value = ft_strtrim(temp->value, "\"");
		temp->type = ARGS;
	}
}
