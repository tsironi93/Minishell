/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:26:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/22 18:06:59 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*static char	*_find_exec(char *cmd, char **dirs)
{
	char	*path;
	char	*tmp;

	while (*dirs)
	{
		tmp = ft_strjoin(*dirs, "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		dirs++;
	}
	//error_handler("command not found", errno);
	return (NULL);
}*/

/*void	try_to_exec(t_data *data, t_token **token, char **env)
{
	t_token	*temp;
	char	**cmd;
	char	*cmd_path;
	int		pid;

	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_COMMAND)
			break ;
		temp = temp->next;
	}
	if (!temp)
		return ;
	cmd_path = _find_exec(temp->value, data->env_paths);
	if (!cmd_path)
		return ;
	cmd = malloc(3 * sizeof(char *));
	cmd[0] = ft_strdup(temp->value);
	while (temp)
	{
		if (temp->type == 1)
			break ;
		temp = temp->next;
	}
	if (temp->type != 1)
		return ;
	cmd[1] = ft_strdup(temp->value);
	cmd[2] = NULL;
	pid = fork();
	if (pid == 0)
		execve(cmd_path, cmd, env);
	waitpid(pid, NULL, 0);
	free2d(cmd);
	cmd = NULL;
}*/

void	asd()

int	main(int ac, char **av, char **env)
{
	t_token	*token;
	t_data	data;

	(void)ac;
	(void)av;
	(void)env;
	token = NULL;
	data.tokens = token;
	data.env_paths = ft_split(getenv("PATH"), ':');
	printf(CYAN "\n\n\t\tHello Malaka\n\n");
	while (1)
	{
		data.input = readline("~>:");
		if (!data.input)
			break ;
		if (*data.input)
			add_history(data.input);
		if (!ft_strncmp(data.input, "exit", 5))	//TODO NEEDS to BE strcmp
			close_pros(&data);
		lexer(data.input, &token);
		print_tokens(&token);
		//try_to_exec(&data, &token, env);
		free_linked(token);
		token = NULL;
		free (data.input);
	}
	exit(EXIT_SUCCESS);
}
