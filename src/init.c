/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:32:55 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/18 10:53:58 by itsiros          ###   ########.fr       */
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

static void	init_env(t_data *data, char **envp)
{
	int		i;

	if (!envp || !*envp)
		return ;
	i = -1;
	while (envp[++i])
		append_node(&data->env, envp[i]);
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
	printf(CYAN "\n\n\t\tHello Malaka\n\n");
}
