/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:56:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/26 13:17:33 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	lexer_2(t_data *data, char *input, t_token **token, int *i)
{
	char	*buf;

	if (ft_isalnum(input[(*i)]) || input[(*i)] == '$' || input[(*i)] == '.'
		|| input[(*i)] == '/' || input[(*i)] == '=' || input[(*i)] == '?'
		|| input[(*i)] == '~')
	{
		buf = wraper_sign(data, input, &(*i));
		append_token(data, token, buf, UNKNOWN);
	}
	else if (input[(*i)] == '"' || input[(*i)] == 39 || input[(*i)] == '-')
		bullshit2(data, token, &(*i), input);
	else if ((input[(*i)] == '|' && input[(*i) + 1] != '|')
		|| input[(*i)] == '>' || input[(*i)] == '<')
		bullshit(data, token, &(*i), input);
	else
	{
		buf = wraper_sign(data, input, &(*i));
		append_token(data, token, buf, UNKNOWN);
	}
}

void	lexer(t_data *data, char *input, t_token **token)
{
	int		i;
	char	buffer[2];

	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			append_token(data, token, "", ISSPACE);
			i++;
		}
		else if (input[i] == '\\' && input[i + 1])
		{
			buffer[0] = input[i + 1];
			buffer[1] = '\0';
			append_token(data, token, buffer, SINGLE_QUOTES);
			i += 2;
		}
		else
			lexer_2(data, input, token, &i);
	}
}

