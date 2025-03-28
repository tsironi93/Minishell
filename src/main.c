/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:26:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/28 19:59:48 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean(t_data *data, bool flag)
{
	free_linked(data->tokens);
	data->tokens = NULL;
	free (data->input);
	if (flag)
		exit (EXIT_SUCCESS);
}

static void	init(int ac, char **av, char **envp, t_data *data)
{
	int	i;

	i = 0;
	atexit(check_leaks);
	(void)ac;
	(void)av;
	data->env = NULL;
	data->tokens = NULL;
	while (envp[i])
		append_node(&data->env, envp[i++]);
	data->env_paths = ft_split(getenv("PATH"), ':');
	printf(CYAN "\n\n\t\tHello Malaka\n\n");
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	init(ac, av, envp, &data);
	while (1)
	{
		data.input = readline("~>:");
		if (!data.input)
			break ;
		if (*data.input)
			add_history(data.input);
		if (!ft_strcmp(data.input, "exit"))
			clean(&data, true);
		if (!ft_strcmp(data.input, "print env"))
			print_linked(&data.env);
		lexer(data.input, &data.tokens);
		print_tokens(&data.tokens);
		if (!classify_tokens(&data.tokens))
		{
			clean(&data, false);
			continue ;
		}
		print_tokens(&data.tokens);
		expansion(&data.tokens, &data.env);
		//print_tokens(&data.tokens);
		if (num_of_type(&data.tokens, PIPE))
			handle_pipeline(&data.tokens, &data, num_of_type(&data.tokens, PIPE));
		else
			try_to_exec(&data, &data.tokens, envp);
		clean(&data, false);
	}
	exit(EXIT_SUCCESS);
}
