/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:03:34 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/15 14:20:43 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean(t_data *data, bool exit_)
{
	//free (data->input);
	gc_collect(&data->gc);
	data->tokens = NULL;
	if (exit_)
	{
		//free_env(&data->env);
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

/*void	free_fds(t_data *data)
{
	if (data->input_fd)
		free(data->input_fd);
	if (data->append_fd)
		free(data->append_fd);
	if (data->output_fd)
		free(data->output_fd);
	data->input_fd = NULL;
	data->output_fd = NULL;
	data->append_fd = NULL;
}*/
