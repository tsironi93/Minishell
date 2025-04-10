/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:26:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/10 16:49:36 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

static void	init(int ac, char **av, char **envp, t_data *data)
{
	int		i;

	i = 0;
	atexit(check_leaks);
	(void)ac;
	(void)av;
	data->gc = gc_new();
	data->env = NULL;
	data->tokens = NULL;
	data->env_full = envp;
	data->input = NULL;
	data->exit_code = 0;
	while (envp[i])
		append_node(&data->env, envp[i++]);
	data->env_cmd_paths = ft_split(getenv("PATH"), ':');
	printf(CYAN "\n\n\t\tHello Malaka\n\n");
}

static void	asd(t_data *data)
{
	if (*data->input)
		add_history(data->input);
	if (!ft_strcmp(data->input, "exit"))
		clean(data, true);
	if (!ft_strcmp(data->input, "print env"))
		print_linked(&data->env);
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	init(ac, av, envp, &data);
	while ("Malaka")
	{
		clean(&data, false);
		//if (isatty(fileno(stdin)))
			data.input = gc_readline(&data.gc, "~>:");
		asd(&data);
		lexer(&data, data.input, &data.tokens);
		// print_tokens(&data.tokens);
		if (!classify_tokens(&data.tokens))
			continue ;
		// print_tokens(&data.tokens);
		expansion(&data.tokens, &data);
		// print_tokens(&data.tokens);
		merge(&data, &data.tokens);
		if (!check_files(&data, &data.tokens))
		{
			p();
			continue ;
		}
		if (num_of_type(&data.tokens, PIPE, NULLL))
			handle_pipeline(&data, num_of_type(&data.tokens, PIPE, NULLL));
		else
			try_to_exec(&data, &data.tokens);
	}
	exit(EXIT_SUCCESS);
}
