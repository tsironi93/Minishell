/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:03:34 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/31 21:59:39 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean(t_data *data, bool exit_)
{
	free_linked(data->tokens);
	free_env(&data->env);
	data->tokens = NULL;
	free (data->input);
	if (exit_)
	{
		clear_history();
		exit (EXIT_SUCCESS);
	}
}

void	free_fds(t_data *data)
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
}
