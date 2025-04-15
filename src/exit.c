/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:18:10 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/15 15:53:30 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_builtin(t_data *data, t_token **token)
{
	int		status;
	t_token	*cur;
	int i  = 0;

	status = 0;
	printf("exit\n");
	if (num_of_type(token, ARGS, PIPE) > 1)
	{
		printf("minishell: exit: too many arguments");
		status = 1;
	}
	if (num_of_type(token, ARGS, PIPE) == 1)
	{
		cur = *token;
		while (cur)
		{
			if (cur->type == ARGS)
				break ;
			cur = cur->next;
		}
		if (cur->value[i] == '-' || cur->value[i] == '+')
			i++;
		while (cur->value[i])
		{
			if (!ft_isdigit(cur->value[i++]))
			{
				printf("minishell: exit: %s: numeric argument required", cur->value);
				status = 255;
				clean(data, true);
				exit(status);
			}
		}
		data->exit_code = ft_atoi(cur->value);
	}
	printf("%d\n\n\n", data->exit_code);
	clean(data, true);
	exit(data->exit_code);
}