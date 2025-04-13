/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:26:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/13 15:05:54 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		// print_tokens(&data.tokens);
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
