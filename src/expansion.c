/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:41:19 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/30 17:52:38 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*_find_env_path(t_env **env_list, char *search_key)
{
	char	*path;
	t_env	*temp;

	temp = *env_list;
	path = NULL;
	while (temp)
	{
		if (ft_strnstr(temp->str, search_key, ft_strlen(search_key))
			&& temp->str[ft_strlen(search_key)] == '=')
			path = ft_strnstr(temp->str, search_key, ft_strlen(search_key));
		if (path != NULL)
			return (path + ft_strlen(search_key) + 1);
		temp = temp->next;
	}
	return ("");
}

static void	_append_expansion(char **arg, char **name)
{
	char	*temp;

	if (*name == NULL)
		*name = ft_strjoin("", *arg);
	else
	{
		temp = *name;
		*name = ft_strjoin(temp, *arg);
		free (temp);
	}
	free (*arg);
	*arg = "";
}

static void	_expand_variables(t_env **env, char *input, char **exp_result)
{
	char	buffer[256];
	int		i;
	int		pos;
	char	*arg;

	arg = "";
	i = 0;
	while (input[i])
	{
		pos = 0;
		while (input[i] && input[i] != '$')
			buffer[pos++] = input[i++];
		buffer[pos] = '\0';
		if (pos > 0)
		{
			arg = ft_strdup(buffer);
			_append_expansion(&arg, exp_result);
		}
		pos = 0;
		while (!ft_isspace(input[++i]) && input[i])
			buffer[pos++] = input[i];
		buffer[pos] = '\0';
		arg = ft_strdup(_find_env_path(env, buffer));
		_append_expansion(&arg, exp_result);
	}
}

void	expansion(t_token **token, t_env **env)
{
	t_token	*temp;
	char	*str;

	str = NULL;
	temp = *token;
	while (temp)
	{
		if (temp->type == COMMAND || temp->type == DOUBLE_QUOTES
			|| temp->type == ARGS)
		{
			if (ft_strchr(temp->value, '$'))
			{
				_expand_variables(env, temp->value, &str);
				free(temp->value);
				temp->value = str;
			}
			if (temp->type == DOUBLE_QUOTES)
				temp->type = ARGS;
		}
		if (temp->type == SINGLE_QUOTES)
			temp->type = ARGS;
		temp = temp->next;
	}
}
