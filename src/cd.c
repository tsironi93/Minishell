/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:16:10 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/25 11:34:04 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	create_oldpwd(t_env **env)
{
	t_env	*cur;

	cur = *env;
	while (cur)
	{
		if (!ft_strncmp(cur->str, "OLDPWD=", 7))
			return ;
		cur = cur->next;
	}
	append_node(env, "OLDPWD=");
}

static int	current_dir(t_data *data, t_token *cur)
{
	char	*new_temp;
	char	*tmp;

	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (perror("getcwd"), data->exit_code = 1, data->exit_code);
	if (chdir(cur->value) != 0)
		return (perror("cd"), free(tmp), data->exit_code = 1,
			data->exit_code);
	new_temp = getcwd(NULL, 0);
	if (!new_temp)
		return (perror("getcwd"), data->exit_code = 1, data->exit_code);
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
		return (perror("getcwd"), data->exit_code = 1, data->exit_code);
	i = ft_strlen(tmp);
	while (tmp[i] != '/' && i > 0)
		i--;
	ft_strlcpy(buffer, tmp, i + 1);
	if (chdir(cur->value) != 0)
		return (perror("cd"), free(tmp), data->exit_code = 1,
			data->exit_code);
	update_env(data, data->env, tmp, buffer);
	free(tmp);
	data->exit_code = 0;
	return (data->exit_code);
}

char	*ft_getenv(t_env **env, char *var)
{
	t_env	*cur;

	cur = *env;
	while (cur)
	{
		if (!ft_strncmp(cur->str, var, ft_strlen(var))
			&& cur->str[ft_strlen(var)] == '=')
		{
			return (cur->str + ft_strlen(var) + 1);
		}
		cur = cur->next;
	}
	return ("");
}

static int	cd_helper(t_data *data, char *next_dir, bool flag)
{
	char	*tmp;
	char	*next_dir_expand;

	if (flag)
	{
		next_dir_expand = ft_getenv(&data->env, next_dir);
		if (!ft_strcmp(next_dir_expand, ""))
			return (printf("minishell: cd: %s not set\n", next_dir),
				data->exit_code = 1);
		if (!ft_strcmp(next_dir, "OLDPWD"))
			printf("%s\n", ft_getenv(&data->env, next_dir));
	}
	else
		next_dir_expand = next_dir;
	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (perror("getcwd"), data->exit_code = 1, data->exit_code);
	if (chdir(next_dir_expand) != 0)
		return (perror("cd"), free(tmp), data->exit_code = 1);
	update_env(data, data->env, tmp, next_dir_expand);
	free(tmp);
	data->exit_code = 0;
	return (data->exit_code);
}

int	cd_buildin(t_data *data, t_token **token)
{
	t_token	*cur;

	cur = search_tokens(token, ARGS);
	create_oldpwd(&data->env);
	if (num_of_type(token, ARGS, PIPE) == 0 || !ft_strcmp(cur->value, "--"))
		return (cd_helper(data, "HOME", true));
	else if (!ft_strncmp(cur->value, "~", 1))
	{
		if (!ft_strcmp(cur->value, "~"))
			return (cd_helper(data, "HOME", true));
		else if (cur->value[1] != '/')
			return (cd_helper(data, gc_strjoin(&data->gc, "/Users/",
						cur->value + 1), false));
		else
			return (cd_helper(data, gc_strjoin(&data->gc, ft_getenv(&data->env,
							"HOME"), (cur->value + 1)), false));
	}
	else if (!ft_strcmp(cur->value, "-"))
		return (cd_helper(data, "OLDPWD", true));
	else if (!ft_strncmp(cur->value, "/", 1))
		return (cd_helper(data, cur->value, false));
	else if (!ft_strncmp(cur->value, "..", 2))
		return (up_a_level(data, cur));
	else
		return (current_dir(data, cur));
}
