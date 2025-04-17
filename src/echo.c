/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chiarakappe <chiarakappe@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:24:35 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/18 01:25:18 by chiarakappe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "/../minishell.h"


/* 
echo hallo       -n
hallo -n */

int	echo_builtin(t_token **token)
{
	t_token *cur;
	int		newline;
	int		first_arg;

	cur = token;
	newline = 1;
	first_arg = 1;
	// skip echo command
	while (cur && cur->type != ARGS)
		cur = cur->next;
	// check for -n flag
	if (cur && cur->type == ARGS && !ft_strcmp(cur->value, "-n"))
	{
		newline = 0;
		cur = cur->next;
	}
	// print args
	while (cur && )
	
	if (newline)
		printf("\n");
	return (EXIT_SUCCESS);	
}

