/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:26:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/23 18:03:11 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	classify_tokens(t_token **token)
{
	t_token	*temp;

	temp = *token;
	while (search_tokens(&temp, REDIRECT_INP))
	{
		temp = search_tokens(&temp, REDIRECT_INP);
		temp->next->type = FILENAME_INP;
		temp = temp->next;
	}
	temp = *token;
	while (search_tokens(&temp, REDIRECT_OUT))
	{
		temp = search_tokens(&temp, REDIRECT_OUT);
		temp->next->type = FILENAME_OUT;
		temp = temp->next;
	}
	temp = *token;
	while (search_tokens(&temp, APPEND))
	{
		temp = search_tokens(&temp, APPEND);
		temp->next->type = APPEND_FILENAME_OUT;
		temp = temp->next;
	}
	temp = *token;
	while (search_tokens(&temp, HERE_DOC))
	{
		temp = search_tokens(&temp, HERE_DOC);
		temp->next->type = HERE_DOC_OPT;
		temp = temp->next;
	}
	temp = *token;
	temp = search_tokens(token, UNKNOWN);
	if (temp)
		temp->type = COMMAND;
	while (search_tokens(&temp, PIPE))
	{
		temp = search_tokens(&temp, PIPE);
		temp = search_tokens(&temp, UNKNOWN);
		temp->type = COMMAND;
	}
	while (search_tokens(token, UNKNOWN))
	{
		temp = search_tokens(token, UNKNOWN);
		temp->type = ARGS;
	}
}

static void	save_env(t_env **ll, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		append_node(ll, env[i]);
		i++;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_token	*token;
	t_data	data;
	t_env	*env;

	(void)ac;
	(void)av;
	env = NULL;
	token = NULL;
	save_env(&env, envp);
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
		if (!ft_strncmp(data.input, "print env", 9))
			print_linked(&env);
		lexer(data.input, &token);
		print_tokens(&token);
		classify_tokens(&token);
		print_tokens(&token);
		expansion(&token);
		print_tokens(&token);
		try_to_exec(&data, &token, envp);
		free_linked(token);
		token = NULL;
		free (data.input);
	}
	exit(EXIT_SUCCESS);
}
