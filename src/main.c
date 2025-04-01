/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:26:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/01 16:45:13 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init(int ac, char **av, char **envp, t_data *data)
{
	int	i;

	i = 0;
	atexit(check_leaks);
	(void)ac;
	(void)av;
	data->env = NULL;
	data->tokens = NULL;
	data->env_full = envp;
	data->input = NULL;
	data->input_fd = NULL;
	data->append_fd = NULL;
	data->output_fd = NULL;
	while (envp[i])
		append_node(&data->env, envp[i++]);
	data->env_cmd_paths = ft_split(getenv("PATH"), ':');
	printf(CYAN "\n\n\t\tHello Malaka\n\n");
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	init(ac, av, envp, &data);
	while ("Malaka")
	{
		clean(&data, false);
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
//		print_tokens(&data.tokens);
		if (!classify_tokens(&data.tokens))
			continue ;
//		print_tokens(&data.tokens);
		expansion(&data.tokens, &data);
//		print_tokens(&data.tokens);
		if (!check_files(&data.tokens))
			continue ;
		if (num_of_type(&data.tokens, PIPE, NULLL))
			handle_pipeline(&data, num_of_type(&data.tokens, PIPE, NULLL));
		else
			try_to_exec(&data, &data.tokens);
	}
	exit(EXIT_SUCCESS);
}
