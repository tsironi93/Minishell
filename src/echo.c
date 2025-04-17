/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chiarakappe <chiarakappe@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:24:35 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/18 01:57:55 by chiarakappe      ###   ########.fr       */
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

int	echo_builtin(t_token **token)
{
	t_token *cur;
	int		new_line;

	cur = *token;
	new_line = 1;
	// skip echo command
	while (cur && cur->type != ARGS)
		cur = cur->next;
	// check for -n flag
	if (cur && cur->type == ARGS && !ft_strcmp(cur->value, "-n")
		&& _is_valid_n(cur->value))
	{
		new_line = 0;
		cur = cur->next;
	}
	// print args
	while (cur && cur->type != PIPE && cur->type == ARGS)
	{
		printf("%s", cur->value);
		if (cur->next && cur->next->type == ARGS)
			printf(" ");
		cur = cur->next;
	}
	if (new_line)
		printf("\n");
	return (EXIT_SUCCESS);
}
