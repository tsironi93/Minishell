/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:26:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/21 15:52:09 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	lexer(char *input, t_token **token)
{
	int		i;
	char	buffer[256];
	int		pos;

	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		if (ft_isalpha(input[i]))
		{
			pos = 0;
			while (ft_isalpha(input[i]) || input[i] == '/')
				buffer[pos++] = input[i++];
			buffer[pos] = '\0';
			append(token, TOKEN_COMMAND, buffer);
		}
		else if (input[i] == '"')
		{
			pos = 0;
			buffer[pos++] = input[i++];
			while (input[i] && input[i] != '"')
				buffer[pos++] = input[i++];
			buffer[pos] = '"';
			buffer[++pos] = '\0';
			i++;
			append(token, TOKEN_STRING, buffer);
		}
		else if (input[i] == '-')
		{
			pos = 0;
			buffer[pos++] = input[i++];
			while (input[i] && ft_isalpha(input[i]))
				buffer[pos++] = input[i++];
			buffer[pos] = '\0';
			append(token, TOKEN_ARGS, buffer);
		}
		else if (input[i++] == '|')
			append(token, TOKEN_PIPE, "|");
		else if (input[i++] == '>')
			append(token, TOKEN_REDIRECT, ">");
		else if (input [i++] == '&')
			append(token, TOKEN_LOGICAL_OP, "&");
		else
		{
			pos = 0;
			while (input[i] && !ft_isspace(input[i]) && input[i] != '|' && input[i] != '&' && input[i] != '>')
				buffer[pos++] = input[i++];
			buffer[pos] = '\0';
			append(token, TOKEN_FILENAME, buffer);
		}
	}
}

int	main(void)
{
	t_token	*token;
	t_data	data;

	token = NULL;
	data.tokens = token;
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
		while (token)
		{
			printf("%s\n", token->value);
			token = token->next;
		}
		free_linked(token);
		free (data.input);
	}
	exit(EXIT_SUCCESS);
}
