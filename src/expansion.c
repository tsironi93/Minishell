/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:41:19 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/26 10:20:12 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*_get_path(t_env **envp, char *exp)
{
	char	*path;
	t_env	*temp;

	temp = *envp;
	path = NULL;
	while (temp)
	{
		if (ft_strnstr(temp->str, exp, ft_strlen(exp))
			&& temp->str[ft_strlen(exp) + 1 == '='])
			path = ft_strnstr(temp->str, exp, ft_strlen(exp));
		if (path != NULL)
			return (path + 5);
		temp = temp->next;
	}
	return ("");
}

char	*prepare_exp(t_env **env, char *str)
{
	char	buffer[256];
	int		i;
	int		pos;
	char	*arg;
	char	*res;

	res = ft_strdup("");
	i = 0;
	while (str[i])
	{
		pos = 0;
		while (str[i] && str[i] != '$')
			buffer[pos++] = str[i++];
		buffer[pos] = '\0';
		ft_strlcat(res + ft_strlen(res), buffer, pos + 1);
		pos = 0;
		while (!ft_isspace(str[++i]) && str[i])
			buffer[pos++] = str[i];
		buffer[pos] = '\0';
		arg = _get_path(env, buffer);
		ft_strlcat(res + ft_strlen(res), arg, ft_strlen(arg) + 1);
	}
	return (res);
}

void	expansion(t_token **token, t_env **env)
{
	t_token	*temp;
	char	*str;

	temp = *token;
	while (temp)
	{
		if (temp->type == COMMAND || temp->type == DOUBLE_QUOTES
			|| temp->type == ARGS)
		{
			if (ft_strchr(temp->value, '$'))
			{
				str = prepare_exp(env, temp->value);
				free (temp->value);
				temp->value = str;
				if (temp->type == DOUBLE_QUOTES)
					temp->type = ARGS;
			}
		}
		if (temp->type == SINGLE_QUOTES)
			temp->type = ARGS;
		temp = temp->next;
	}
}
