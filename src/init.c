/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:32:55 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/19 16:00:29 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_buildins(char ***buildins)
{
	*buildins = malloc(8 * sizeof(char *));
	(*buildins)[0] = ft_strdup("exit");
	(*buildins)[1] = ft_strdup("cd");
	(*buildins)[2] = ft_strdup("pwd");
	(*buildins)[3] = ft_strdup("export");
	(*buildins)[4] = ft_strdup("unset");
	(*buildins)[5] = ft_strdup("env");
	(*buildins)[6] = ft_strdup("echo");
	(*buildins)[7] = NULL;
}

static void	update_env_value(t_env **env, const char *key, const char *value)
{
	t_env	*cur;
	size_t	key_len;
	char	*new_entry;

	key_len = ft_strlen(key);
	cur = *env;
	while (cur)
	{
		if (!ft_strncmp(cur->str, key, key_len) && cur->str[key_len] == '=')
		{
			// Free the old entry and update it
			free(cur->str);
			new_entry = ft_strjoin3(key, "=", value); // Custom function to concatenate key=value
			cur->str = new_entry;
			return;
		}
		cur = cur->next;
	}
	// If the key does not exist, append it to the list
	new_entry = ft_strjoin3(key, "=", value);
	append_node(env, new_entry);
	free(new_entry); // Free temporary string after appending
}

static char	*get_env_value(t_env *env, const char *key)
{
	t_env	*cur;
	size_t	key_len;

	key_len = ft_strlen(key);
	cur = env;
	while (cur)
	{
		if (!ft_strncmp(cur->str, key, key_len) && cur->str[key_len] == '=')
			return (cur->str + key_len + 1);
		cur = cur->next;
	}
	return (NULL);
}

static void	init_env(t_data *data, char **envp)
{
	int		i;
	char	*shlvl;
	int		shlvl_value;

	if (!envp || !*envp)
		return ;
	i = -1;
	while (envp[++i])
		append_node(&data->env, envp[i]);
	shlvl = get_env_value(data->env, "SHLVL");
	if (shlvl)
	{
		shlvl_value = ft_atoi(shlvl) + 1;
		if (shlvl_value < 0)
			shlvl_value = 0;
		update_env_value(&data->env, "SHLVL", ft_itoa(shlvl_value));
	}
	else
		append_node(&data->env, "SHLVL=2");
	env_reconstr(data, &data->env_full);
}

void	init(int ac, char **av, char **envp, t_data *data)
{
	//atexit(check_leaks);
	(void)ac;
	(void)av;
	data->gc = gc_new();
	data->env = NULL;
	data->tokens = NULL;
	data->input = NULL;
	data->exit_code = 0;
	init_env(data, envp);
	data->env_cmd_paths = ft_split(getenv("PATH"), ':'); //NEEDS TO CHANGE
	init_buildins(&data->buildins);
	signal(SIGINT, sigint_handler);
	signal(SIGTSTP, sigtstp_handler);
	signal(SIGCHLD, sigchld_handler);
	// printf(CYAN "\n\n\t\tHello Malaka\n\n");
}
