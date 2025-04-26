/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:41:19 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/26 19:11:03 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	_expand_variables(t_data *data, char *input, char **exp_result)
{
	int		i;
	char	*arg;

	i = 0;
	while (input[i])
	{
		save_in_buffer(data, input, &i, exp_result);
		if (input[i] == '$' && input[i + 1] == '?')
		{
			arg = ft_itoa(data->exit_code);
			_append_expansion(data, &arg, exp_result);
			i += 2;
			continue ;
		}
		expand_var_helper(data, input, &i, exp_result);
	}
}

void	expansion(t_token **token, t_data *data)
{
	t_token	*temp;
	char	*str;

	temp = *token;
	while (temp)
	{
		str = NULL;
		if (temp->type != SINGLE_QUOTES)
		{
			if (!ft_strcmp(temp->value, "$") && temp->next && (temp->next->type
					== DOUBLE_QUOTES || temp->next->type == SINGLE_QUOTES)
				&& ft_strcmp(temp->next->value, "$"))
				dq_expansion(temp);
			if (ft_strchr(temp->value, '$'))
			{
				_expand_variables(data, temp->value, &str);
				temp->value = str;
				if (temp->type == COMMAND)
					temp->type = COMMAND_EX;
			}
		}
		if (temp->type == SINGLE_QUOTES || temp->type == DOUBLE_QUOTES)
			temp->type = ARGS;
		temp = temp->next;
	}
}
