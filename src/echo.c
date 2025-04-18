/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:24:35 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/18 16:01:31 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 
echo hallo       -n
hallo -n */

static int	_is_valid_n(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	while(str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	echo_builtin(t_data *data, t_token **token)
{
	t_token *cur;
	int		new_line;

	data->exit_code = 0;
	cur = *token;
	new_line = 1;
	// skip echo command
	while (cur && cur->type != ARGS)
		cur = cur->next;
	// check for -n flag
	while (cur && cur->type == ARGS	&& _is_valid_n(cur->value))
	{
		new_line = 0;
		cur = cur->next;
	}
	// print args
	while (cur && cur->type != PIPE)
	{
		if (cur->type == ARGS)
		{
			printf("%s", cur->value);
			if (cur->next && cur->next->type == ISSPACE)
				printf(" ");
		}
		cur = cur->next;
	}
	if (new_line)
		printf("\n");
	exit (data->exit_code);
}
