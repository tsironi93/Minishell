/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:16:10 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/14 18:08:36 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cd_buildin(t_data *data, t_token **token)
{
	t_token	*cur;
	char	*tmp;
	char	*new_temp;
	char	buffer[256];

	cur = *token;
	if (cur->type == COMMAND)
	{
		while (cur)
		{
			if (cur->type == ARGS)
				break ;
			cur = cur->next;
		}
	}
	if (num_of_type(token, ARGS, PIPE) == 0 || !ft_strncmp(cur->value, "~", 1))
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
	else if (!ft_strcmp(cur->value, "-"))
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
	else if (!ft_strncmp(cur->value, "/", 1))
	{
		tmp = getcwd(NULL, 0);
		if (chdir(cur->value) != 0)
		{
			perror("cd");
			free(tmp);
			return (errno);
		}
		update_env(data, data->env, tmp, cur->value);
		free(tmp);
		return (EXIT_SUCCESS);
	}

	else if (!ft_strncmp(cur->value, "..", 2))
	{
		tmp = getcwd(NULL, 0);
		int i = ft_strlen(tmp);

		while (tmp[i] != '/')
			i--;
		ft_strlcpy(buffer, tmp, i + 1);
		if (chdir(cur->value) != 0)
		{
			perror("cd");
			free(tmp);
			return (errno);
		}
		update_env(data, data->env, tmp, buffer);
		free(tmp);
		return (EXIT_SUCCESS);
	}
	else // if (!ft_strncmp(cur->value, "./", 2))
	{
		tmp = getcwd(NULL, 0);
		if (chdir(cur->value) != 0)
		{
			perror("cd");
			free(tmp);
			return (errno);
		}
		new_temp = getcwd(NULL, 0);
		update_env(data, data->env, tmp, new_temp);
		free(tmp);
		free(new_temp);
		return (EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}