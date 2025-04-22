/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:24:35 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/21 15:10:18 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	_is_valid_n(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	echo_builtin(t_data *data, t_token **token)
{
	t_token	*cur;
	int		new_line;

	data->exit_code = 0;
	cur = *token;
	new_line = 1;
	while (cur && cur->type != ARGS)
		cur = cur->next;
	while (cur && (cur->type == ARGS || cur->type == ISSPACE))
	{
		if (_is_valid_n(cur->value))
			new_line = 0;
		if (cur->type == ARGS && !_is_valid_n(cur->value))
			break ;
		cur = cur->next;
	}
	while (cur && cur->type != PIPE)
	{
		if (cur->type == ARGS)
			printf("%s", cur->value);
		if (cur->next && cur->next->type == ISSPACE)
			printf(" ");
		cur = cur->next;
	}
	if (new_line)
		printf("\n");
	return (data->exit_code);
}
