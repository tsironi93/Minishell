/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:25:32 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/13 09:37:16 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_buildin(t_data *data)
{
	t_env	*cur;

	if (!data->env)
		return (EXIT_FAILURE);
	cur = data->env;
	while (cur)
	{
		printf("%s\n", cur->str);
		cur = cur->next;
	}
	return (EXIT_SUCCESS);
}
