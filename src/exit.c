/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:18:10 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/17 13:32:06 by turmoil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exit_param(t_data *data, char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i++]))
		{
			printf("minishell: exit: %s: numeric argument required\n", str);
			data->exit_code = 255;
			clean(data, true);
			exit(data->exit_code);
		}
	}
}

int	exit_builtin(t_data *data, t_token **token)
{
	t_token	*cur;

	printf("exit\n");
	if (num_of_type(token, ARGS, PIPE) > 1)
	{
		printf("minishell: exit: too many arguments\n");
		data->exit_code = 1;
	}
	if (num_of_type(token, ARGS, PIPE) == 1)
	{
		cur = search_tokens(token, ARGS);
		exit_param(data, cur->value);
		data->exit_code = ft_atoi(cur->value);
	}
	clean(data, true);
	exit(data->exit_code);
}
