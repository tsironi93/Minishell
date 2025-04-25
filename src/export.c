/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:23:51 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/25 16:56:26 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	sort_export(t_env **env)
{
	t_env	*cur;
	char	*temp;
	int		is_sorted;

	if (!env)
		return ;
	is_sorted = 0;
	while (!is_sorted)
	{
		is_sorted = 1;
		cur = *env;
		while (cur->next)
		{
			if (ft_strcmp(cur->str, cur->next->str) > 0)
			{
				temp = cur->str;
				cur->str = cur->next->str;
				cur->next->str = temp;
				is_sorted = 0;
			}
			cur = cur->next;
		}
	}
}

static void	update_export(t_env **env, t_token *token)
{
	t_env	*cur;
	int		i;

	cur = *env;
	i = 0;
	while (token->value[i] && token->value[i] != '=')
		i++;
	while (cur)
	{
		if (!ft_strncmp(cur->str, token->value, i))
		{
			free (cur->str);
			cur->str = ft_strdup(token->value);
			return ;
		}
		cur = cur->next;
	}
	append_node(env, token->value);
}

int	is_valid_identifier(t_data *data, char *cmd, char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (data->exit_code = 1, ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd, 2),
			ft_putstr_fd(": `", 2), ft_putstr_fd(str, 2),
			ft_putendl_fd("': not a valid identifier", 2), 0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (data->exit_code = 1, ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd, 2),
				ft_putstr_fd(": `", 2), ft_putstr_fd(str, 2),
				ft_putendl_fd("': not a valid identifier", 2), 0);
		i++;
	}
	if (!ft_strcmp(cmd, "unset") && str[i] == '=')
		return (data->exit_code = 1, ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd, 2),
			ft_putstr_fd(": `", 2), ft_putstr_fd(str, 2),
			ft_putendl_fd("': not a valid identifier", 2), 0);
	return (1);
}

static void	print_export(t_env **env)
{
	t_env	*cur;
	char	**temp;
	t_env	*envcopy;

	envcopy = copy_env_list(*env);
	sort_export(&envcopy);
	cur = envcopy;
	while (cur)
	{
		temp = ft_split(cur->str, '=');
		if (temp[1] == NULL)
			printf("declare -x %s\n", temp[0]);
		else
			printf("declare -x %s=\"%s\"\n", temp[0], temp[1]);
		free2d(temp);
		cur = cur->next;
	}
	free_env(&envcopy);
}

int	export_builtin(t_data *data, t_env **env, t_token **token)
{
	t_token	*cur;

	cur = *token;
	if (num_of_type(&cur, ARGS, PIPE) == 0)
		print_export(env);
	else
	{
		while (cur && cur->type != PIPE)
		{
			if (cur->type == ARGS)
			{
				if (is_valid_identifier(data, "export", cur->value))
					update_export(env, cur);
				else
					return (data->exit_code = 1);
			}
			cur = cur->next;
		}
		free2d(data->env_full);
		env_reconstr(data, &data->env_full);
	}
	return (data->exit_code = 0);
}
