/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:41:19 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/25 13:04:35 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	dq_expansion(t_token *cur)
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

static void	_append_expansion(t_data *data, char **arg, char **name)
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

void	_expand_variables(t_data *data, char *input, char **exp_result)
{
	char	buffer[256];
	int		i;
	int		pos;
	char	*arg;

	i = 0;
	while (input[i])
	{
		pos = 0;
		while (input[i] && input[i] != '$')
			buffer[pos++] = input[i++];
		buffer[pos] = '\0';
		if (pos > 0)
		{
			arg = gc_strdup(&data->gc, buffer);
			_append_expansion(data, &arg, exp_result);
		}
		if (input[i] == '$' && input[i + 1] == '?')
		{
			arg = ft_itoa(data->exit_code);
			_append_expansion(data, &arg, exp_result);
			i += 2;
			continue ;
		}
		pos = 0;
		if (input[i] != '$')
			return ;
		while (input[i] && input[i] != '\'' && !ft_isspace(input[i]))
			buffer[pos++] = input[i++];
		buffer[pos] = '\0';
		arg = gc_strdup(&data->gc, _find_env_path(data->env_full, buffer));
		_append_expansion(data, &arg, exp_result);
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
		if (temp->type == COMMAND || temp->type == DOUBLE_QUOTES || temp->type == ARGS
			|| temp->type == FILENAME_INP || temp->type == APPEND_FILENAME_OUT
			|| temp->type == APPEND_FILENAME_OUT || temp->type == HERE_DOC_OPT)
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
