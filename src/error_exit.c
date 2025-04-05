/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:03:34 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/05 12:12:55 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean(t_data *data, bool exit_)
{
	//free_linked(data->tokens);
	free_env(&data->env);
	free (data->input);
	gc_collect(&data->gc);
	data->tokens = NULL;
	if (exit_)
	{
		gc_destroy(&data->gc);
		clear_history();
		exit (EXIT_SUCCESS);
	}
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
