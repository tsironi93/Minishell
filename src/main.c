/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:26:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/19 17:05:15 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <ctype.h>	// For isspace! //TODO
#include <stdio.h>

void	close_pros(void)
{
	exit (EXIT_SUCCESS);
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	ft_strlcpy(new_token->value, value, ft_strlen(value));
	new_token->next = NULL;
	return (new_token);
}

void	append(t_token **head, t_token_type type, char *value)
{
	t_token	*new_token;
	t_token	*temp;

	new_token = create_token(type, value);
	if (!new_token)
		return ;
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
}

void	lexer(char *input, t_token *token)
{
	int		i;
	char	buffer[256];
	int		pos;

	i = 0;
	while (input[i])
	{
		if (isspace(input[i])) // TODO isspace function
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
			append(&token, TOKEN_COMMAND, buffer);
		}
		else if (input[i] == '"')
		{
			i++;
			pos = 0;
			while (input[i] && input[i] != '"')
				buffer[pos++] = input[i++];
			buffer[pos] = '\0';
			i++;
			append(&token, TOKEN_STRING, buffer);
		}
		else if (input[i++] == '|')
			append(&token, TOKEN_PIPE, buffer);
		else if (input[i++] == '>')
			append(&token, TOKEN_REDIRECT, buffer);
		else if (input [i++] == '&')
			append(&token, TOKEN_LOGICAL_OP, buffer);
		else
		{
			pos = 0;
			while (input[i] && !isspace(input[i]) && input[i] != '|' && input[i] != '&' && input[i] != '>')
				buffer[pos++] = input[i++];
			buffer[pos] = '\0';
			append(&token, TOKEN_FILENAME, buffer);
		}
	}
}

int	main(void)
{
	t_token	*token;
	char	*input;

	token = NULL;
	printf(CYAN "\n\n\t\tHello Malaka\n\n");
	while (1)
	{
		input = readline("~>:");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		if (!ft_strncmp(input, "exit", 5))	//TODO NEEDS to BE strcmp
			close_pros();
		lexer(input, token);
		while (token->next)
			printf("%s\n", token->value);
		free (input);
	}
	return (0);
}
