/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:25:32 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/11 18:05:09 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	env_reconstr(t_data *data, char **env_full)
{
	t_env	*cur;
	int		i;

	cur = data->env;
	i = 0;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	env_full = gc_malloc(&data->gc, ((i + 1) * sizeof(char *)));
	i = -1;
	cur = data->env;
	while (cur)
	{
		env_full[++i] = gc_strdup(&data->gc, cur->str);
		cur = cur->next;
	}
	env_full[i + 1] = NULL;
}

void init_env(t_data *data, char **envp)
{
	int		i;

	if (!envp || !*envp)
		return ;
	i = -1;
	while (envp[++i])
		append_node(&data->env, envp[i]);
	env_reconstr(data, data->env_full);
}

int env_buildin(t_data *data)
{
	t_env *cur;

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