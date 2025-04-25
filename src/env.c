/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:25:32 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/25 15:50:27 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*copy_env_list(t_env *env)
{
	t_env	*copy = NULL;
	t_env	*new_node;
	t_env	*last = NULL;

	while (env)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->str = strdup(env->str);
		new_node->next = NULL;
		if (!copy)
			copy = new_node;
		else
			last->next = new_node;
		last = new_node;
		env = env->next;
	}
	return (copy);
}

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

int	env_buildin(t_data *data, t_token **token)
{
	t_env	*cur;

	if (!data->env)
		return (data->exit_code = 1);
	if (num_of_type(token, ARGS, PIPE) > 0 && !search_tokens(token, PIPE))
	 	return (data->exit_code = 1);
	cur = data->env;
	while (cur)
	{
		if (ft_strchr(cur->str, '='))
			printf("%s\n", cur->str);
		cur = cur->next;
	}
	return (data->exit_code = 0);
}
