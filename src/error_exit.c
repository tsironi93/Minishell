/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:03:34 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/26 12:05:07 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean(t_data *data, bool exit_)
{
	gc_collect(&data->gc);
	data->tokens = NULL;
	if (exit_)
	{
		free2d(data->env_cmd_paths);
		restore_terminal();
		free2d(data->buildins);
		free_env(&data->env);
		gc_destroy(&data->gc);
		clear_history();
	}
}

void	free2d(char **a)
{
	int	i;

	i = 0;
	while (a[i])
	{
		free(a[i]);
		i++;
	}
	free(a);
}
