/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:26:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/26 19:12:16 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	input_verification(t_data *data)
{
	if (!data->input)
	{
		if (isatty(fileno(stdin)))
			printf("exit\n");
		clean(data, true);
		exit(data->exit_code);
	}
	if (data->input && isatty(fileno(stdin)))
		add_history(data->input);
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	init(ac, av, envp, &data);
	while ("Malaka")
	{
		clean(&data, false);
		if (isatty(fileno(stdin)))
			data.input = gc_readline(&data.gc, "~>:");
		input_verification(&data);
		if (!check_quotes(data.input))
			continue ;
		lexer(&data, data.input, &data.tokens);
		if (!classify_tokens(&data, &data.tokens))
			continue ;
		expansion(&data.tokens, &data);
		merge(&data, &data.tokens);
		if (!check_files(&data, &data.tokens))
			continue ;
		if (num_of_type(&data.tokens, PIPE, NULLL))
			handle_pipeline(&data, num_of_type(&data.tokens, PIPE, NULLL));
		else
			try_to_exec(&data, &data.tokens);
	}
	exit(EXIT_SUCCESS);
}
