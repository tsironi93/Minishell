/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:25:59 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/25 11:31:22 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_unset(t_env **env, t_token *token)
{
	t_env	*cur;
	t_env	*safe_spot;

	cur = *env;
	safe_spot = *env;
	while (cur)
	{
		if (!ft_strncmp(cur->str, token->value, ft_strlen(token->value)))
		{
			safe_spot->next = cur->next;
			free (cur->str);
			free (cur);
			return ;
		}
		safe_spot = cur;
		cur = cur->next;
	}
}

int	unset_builtin(t_data *data, t_env **env, t_token **token)
{
	t_token	*cur;

	cur = *token;
	if (num_of_type(&cur, ARGS, PIPE) == 0)
		return (EXIT_SUCCESS);
	else
	{
		while (cur && cur->type != PIPE)
		{
			if (cur->type == ARGS && is_valid_identifier("unset", cur->value))
				update_unset(env, cur);
			cur = cur->next;
		}
		free2d(data->env_full);
		env_reconstr(data, &data->env_full);
	}
	return (EXIT_SUCCESS);
}
