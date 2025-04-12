/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:26:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/12 15:52:51 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_buildins(char ***buildins)
{
	*buildins = malloc(8 * sizeof(char *));
	(*buildins)[0] = ft_strdup("echo");
	(*buildins)[1] = ft_strdup("cd");
	(*buildins)[2] = ft_strdup("pwd");
	(*buildins)[3] = ft_strdup("export");
	(*buildins)[4] = ft_strdup("unset");
	(*buildins)[5] = ft_strdup("env");
	(*buildins)[6] = ft_strdup("exit");
	(*buildins)[7] = NULL;
}

static void	init(int ac, char **av, char **envp, t_data *data)
{
	atexit(check_leaks);
	(void)ac;
	(void)av;
	data->gc = gc_new();
	data->env = NULL;
	data->tokens = NULL;
	data->input = NULL;
	data->exit_code = 0;
	init_env(data, envp);
	data->env_cmd_paths = ft_split(getenv("PATH"), ':');
	init_buildins(&data->buildins);
	printf(CYAN "\n\n\t\tHello Malaka\n\n");
}

static void	asd(t_data *data)
{
	if (*data->input)
		add_history(data->input);
	if (!ft_strcmp(data->input, "exit"))
		clean(data, true);
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	init(ac, av, envp, &data);
	while ("Malaka")
	{
		if (isatty(fileno(stdin)))
		data.input = gc_readline(&data.gc, "~>:");
		asd(&data);
		lexer(&data, data.input, &data.tokens);
		// print_tokens(&data.tokens);
		if (!classify_tokens(&data.tokens))
		continue ;
		// print_tokens(&data.tokens);
		expansion(&data.tokens, &data);
		print_tokens(&data.tokens);
		merge(&data, &data.tokens);
		if (!check_files(&data, &data.tokens))
		continue ;
		if (num_of_type(&data.tokens, PIPE, NULLL))
		handle_pipeline(&data, num_of_type(&data.tokens, PIPE, NULLL));
		else
		try_to_exec(&data, &data.tokens);
		clean(&data, false);
	}
	exit(EXIT_SUCCESS);
}
