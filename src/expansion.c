/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:41:19 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/24 16:10:12 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*_get_path(t_env **envp, char *exp)
{
	char	*path;
	t_env	*temp;

	temp = *envp;
	while (temp)
	{
		path = ft_strnstr(temp->str, exp, ft_strlen(exp));
		if (path != NULL)
			break ;
		temp = temp->next;
	}
	return (path + 5);
}

static char	*expand(char *str, t_env **env)
{
	int		i;
	char	*pos;
	char	buffer[256];
	char	*exp;

	i = 0;
	if (!str)
		return (NULL);
	pos = ft_strchr(str, '$');
	pos++;
	while (*pos != '\0' && !ft_isspace(*pos) && *pos != '"')
		buffer[i++] = *pos++;
	buffer[i++] = '=';
	buffer[i] = '\0';
	exp = _get_path(env, buffer);
	return (exp);
}

char	*prepare_exp(t_env **env, char *str)
{
	char	buffer_bef[256];
	char	buffer_aft[256];
	char	buffer_exp[256];
	int		i;
	int		pos;

	pos = 0;
	i = 0;
	while (str[i] && str[i] != '$')
		buffer_bef[pos++] = str[i++];
	(void)buffer_aft;
	(void)buffer_exp;
	(void)env;
	return (str);
}

void	expansion(t_token **token, t_env **env)
{
	t_token	*temp;
	char	*tmp;

	temp = *token;
	while (search_tokens(&temp, SINGLE_QUOTES))
	{
		temp = search_tokens(&temp, SINGLE_QUOTES);
		temp->type = ARGS;
	}
	temp = *token;
	while (search_tokens(&temp, DOUBLE_QUOTES))
	{
		temp = search_tokens(&temp, DOUBLE_QUOTES);
		if (ft_strchr(temp->value, '$'))
		{
			tmp = expand(temp->value, env);
			free(temp->value);
			temp->value = ft_strdup(tmp);
		}
		temp->type = ARGS;
	}
	temp = *token;
	while (search_tokens(&temp, EXPAND))
	{
		temp = search_tokens(&temp, EXPAND);
		tmp = expand(temp->value, env);
		free(temp->value);
		temp->value = ft_strdup(tmp);
		temp->type = ARGS;
	}
}
