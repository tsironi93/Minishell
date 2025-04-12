/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:16:10 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/12 16:14:59 by ckappe           ###   ########.fr       */
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
			cur->str = ft_strjoin("PWD=", next_pwd);
		if (!ft_strncmp("OLDPWD", cur->str, 6))
			cur->str = ft_strjoin("OLDPWD=", prev_pwd);
		cur = cur->next;
	}
	env_reconstr(data, data->env_full);
}

int	cd_buildin(t_data *data, t_token **token)
{
	t_token	*cur;
	char	*tmp;

	cur = *token;
	
	if (cur->type == COMMAND)
	{
		while (cur)
		{
			if (cur->type == ARGS)
				break;
			cur = cur->next;
		}
	}
	if (num_of_type(token, ARGS, PIPE) == 0 || !ft_strcmp(cur->value, "~"))
	{
		tmp = getcwd(NULL, 0);
		if (chdir(getenv("HOME")) != 0)
		{
			perror("cd");
			free(tmp);
			return (errno);
		}
		update_env(data, data->env, tmp, getenv("HOME"));
		free(tmp);
		return (EXIT_SUCCESS);
	}
	if (!ft_strcmp(cur->value, "-"))
	{
		tmp = getcwd(NULL, 0);
		if (chdir(getenv("OLDPWD")) != 0)
		{
			perror("cd");
			free(tmp);
			return (errno);
		}
		update_env(data, data->env, tmp, getenv("OLDPWD"));
		free(tmp);
		return (EXIT_SUCCESS);
	}
	if (!ft_strcmp(cur->value, "/"))
	// strncmp, 1, look below!
	{
		tmp = getcwd(NULL, 0);
		if (chdir("/") != 0)
		{
			perror("cd");
			free(tmp);
			return (errno);
		}
		update_env(data, data->env, tmp, "/");
		free(tmp);
		return (EXIT_SUCCESS);
	}
	if (!ft_strncmp(cur->value, "./", 2))
	{
		tmp = getcwd(NULL, 0);
		if (chdir(cur->value) != 0)
		{
			perror("cd");
			free(tmp);
			return (errno);
		}
		update_env(data, data->env, tmp, getcwd(NULL, 0));
		free(tmp);
		return (EXIT_SUCCESS);
	}
	
	return (EXIT_SUCCESS);
}
