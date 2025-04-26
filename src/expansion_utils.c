/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:09:07 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/26 19:10:46 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	dq_expansion(t_token *cur)
{
	if (cur->previous && cur->next)
	{
		cur->previous->next = cur->next;
		cur = cur->next;
	}
}

char	*_find_env_path(char **env_list, char *search_key)
{
	char	*path;
	int		i;
	char	*no_dollar;

	if (search_key[1] == '\0')
		return (search_key);
	no_dollar = search_key + 1;
	i = -1;
	path = NULL;
	while (env_list[++i])
	{
		if (ft_strnstr(env_list[i], no_dollar, ft_strlen(no_dollar))
			&& env_list[i][ft_strlen(no_dollar)] == '=')
			path = ft_strnstr(env_list[i], no_dollar, ft_strlen(no_dollar));
		if (path != NULL)
			return (path + (ft_strlen(no_dollar) + 1));
	}
	if (!ft_strchr(no_dollar, '"'))
		return ("");
	while (ft_strchr(search_key, '"'))
		search_key = ft_strtrim(search_key, "\"");
	return (search_key);
}

void	_append_expansion(t_data *data, char **arg, char **name)
{
	char	*temp;

	if (*name == NULL)
		*name = gc_strjoin(&data->gc, "", *arg);
	else
	{
		temp = *name;
		*name = gc_strjoin(&data->gc, temp, *arg);
	}
	*arg = "";
}

void	save_in_buffer(t_data *data, char *input, int *i, char **exp_result)
{
	int		pos;
	char	buffer[256];
	char	*arg;

	pos = 0;
	while (input[(*i)] && input[(*i)] != '$')
		buffer[pos++] = input[(*i)++];
	buffer[pos] = '\0';
	if (pos > 0)
	{
		arg = gc_strdup(&data->gc, buffer);
		_append_expansion(data, &arg, exp_result);
	}
}

void	expand_var_helper(t_data *data, char *input, int *i, char **exp_result)
{
	int		pos;
	char	buffer[256];
	char	*arg;

	pos = 0;
	if (input[(*i)] != '$')
		return ;
	while (input[(*i)] && input[(*i)] != '\'' && !ft_isspace(input[(*i)]))
		buffer[pos++] = input[(*i)++];
	buffer[pos] = '\0';
	arg = gc_strdup(&data->gc, _find_env_path(data->env_full, buffer));
	_append_expansion(data, &arg, exp_result);
}
