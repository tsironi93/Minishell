/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:25:32 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/19 16:30:12 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_env(t_data *data, t_env *env, char *prev_pwd, char *next_pwd)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		if (!ft_strncmp("PWD", cur->str, 3))
		{
			free(cur->str);
			cur->str = ft_strjoin("PWD=", next_pwd);
		}
		if (!ft_strncmp("OLDPWD", cur->str, 6))
		{
			free(cur->str);
			cur->str = ft_strjoin("OLDPWD=", prev_pwd);
		}
		cur = cur->next;
	}
	free2d(data->env_full);
	env_reconstr(data, &data->env_full);
}

void	env_reconstr(t_data *data, char ***env_full)
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
	*env_full = malloc((i + 1) * sizeof(char *));
	i = -1;
	cur = data->env;
	while (cur)
	{
		(*env_full)[++i] = ft_strdup(cur->str);
		cur = cur->next;
	}
	(*env_full)[i + 1] = NULL;
}

int	env_buildin(t_data *data)
{
	t_env	*cur;

	if (!data->env)
		return (data->exit_code = 1);
	cur = data->env;
	while (cur)
	{
		printf("%s\n", cur->str);
		cur = cur->next;
	}
	return (data->exit_code = 0);
}
