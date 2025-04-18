/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:16:10 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/18 10:35:55 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/syslimits.h>

static int	current_dir(t_data *data, t_token *cur)
{
	char	*new_temp;
	char	*tmp;

	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (perror("getcwd"), data->exit_code = errno, data->exit_code);
	if (chdir(cur->value) != 0)
		return (perror("cd"), free(tmp), data->exit_code = errno,
			data->exit_code);
	new_temp = getcwd(NULL, 0);
	if (!new_temp)
		return (perror("getcwd"), data->exit_code = errno, data->exit_code);
	update_env(data, data->env, tmp, new_temp);
	free(tmp);
	free(new_temp);
	data->exit_code = 0;
	return (data->exit_code);
}

static int	up_a_level(t_data *data, t_token *cur)
{
	char	*tmp;
	int		i;
	char	buffer[PATH_MAX];

	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (perror("getcwd"), data->exit_code = errno, data->exit_code);
	i = ft_strlen(tmp);
	while (tmp[i] != '/' && i > 0)
		i--;
	ft_strlcpy(buffer, tmp, i + 1);
	if (chdir(cur->value) != 0)
		return (perror("cd"), free(tmp), data->exit_code = errno,
			data->exit_code);
	update_env(data, data->env, tmp, buffer);
	free(tmp);
	data->exit_code = 0;
	return (data->exit_code);
}

static int	cd_helper(t_data *data, char *next_dir)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (perror("getcwd"), data->exit_code = errno, data->exit_code);
	if (chdir(next_dir) != 0)
		return (perror("cd"), free(tmp), data->exit_code = errno,
			data->exit_code);
	update_env(data, data->env, tmp, next_dir);
	free(tmp);
	data->exit_code = 0;
	return (data->exit_code);
}

int	cd_buildin(t_data *data, t_token **token)
{
	t_token	*cur;

	cur = search_tokens(token, ARGS);
	if (num_of_type(token, ARGS, PIPE) == 0)
		return (cd_helper(data, getenv("HOME")));
	if (!ft_strncmp(cur->value, "~", 1))
	{
		if (cur->value[1] != '/')
			return (cd_helper(data, gc_strjoin(&data->gc, "/Users/",
						cur->value + 1)));
		else
			return (cd_helper(data, gc_strjoin(&data->gc, getenv("HOME"),
						(cur->value + 1))));
	}
	else if (!ft_strcmp(cur->value, "-"))
		return (cd_helper(data, getenv("OLDPWD")));
	else if (!ft_strncmp(cur->value, "/", 1))
		return (cd_helper(data, cur->value));
	else if (!ft_strncmp(cur->value, "..", 2))
		return (up_a_level(data, cur));
	else
		return (current_dir(data, cur));
}
