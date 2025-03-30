/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:03:34 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/29 17:03:59 by itsiros          ###   ########.fr       */
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
