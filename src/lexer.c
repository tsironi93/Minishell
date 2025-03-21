/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:56:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/22 17:37:13 by itsiros          ###   ########.fr       */
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
			append(token, buffer, UNKNOWN);
		}
		else if (input[i] == '"')
		{
			pos = 0;
			buffer[pos++] = input[i++];
			while (input[i] && input[i] != '"')
				buffer[pos++] = input[i++];
			if (input[i] == '"')
			{
				buffer[pos] = input[i++];
				buffer[pos + 1] = '\0';
			}
			else
				buffer[pos] = '\0';
			append(token, buffer, DOUBLE_QUOTES);
		}
		else if (input[i] == 39)
		{
			pos = 0;
			buffer[pos++] = input[i++];
			while (input[i] && input[i] != 39)
				buffer[pos++] = input[i++];
			if (input[i] == 39)
			{
				buffer[pos] = input[i++];
				buffer[pos + 1] = '\0';
			}
			else
				buffer[pos] = '\0';
			append(token, buffer, SINGLE_QUOTES);
		}
		else if (input[i] == '-')
		{
			pos = 0;
			buffer[pos++] = input[i++];
			while (input[i] && ft_isalpha(input[i]))
				buffer[pos++] = input[i++];
			buffer[pos] = '\0';
			append(token, buffer, ARGS);
		}
		else if (input[i] == '|' && input[i + 1] != '|')
		{
			pos = 0;
			buffer[pos++] = input [i++];
			if (input[i] == input[i - 1])
				buffer[pos++] = input [i++];
			buffer[pos] = '\0';
			append(token, buffer, PIPE);
		}
		else if (input[i] == '<')
		{
			append(token, "<", REDIRECT_INP);
			i++;
		}
		else if (input[i] == '>')
		{
			append(token, ">", REDIRECT_OUT);
			i++;
		}
		else
		{
			printf("Sorry cant handle this!");
			return ;
		}
	}
}
