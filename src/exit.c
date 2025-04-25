/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:18:10 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/24 15:11:25 by itsiros          ###   ########.fr       */
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
			ft_putstr_fd("minishell: exit: numeric argument required", 2);
			data->exit_code = 255;
			clean(data, true);
			exit(data->exit_code);
		}
	}
}

int	exit_builtin(t_data *data, t_token **token)
{
	t_token	*cur;

	ft_putendl_fd("exit", 2);
	if (num_of_type(token, ARGS, PIPE) > 1)
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		exit (data->exit_code = 1);
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
