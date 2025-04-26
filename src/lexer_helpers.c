/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:15:10 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/26 13:17:25 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	bullshit(t_data *data, t_token **token, int *i, char *input)
{
	if (input[*i] == '|' && input[*i + 1] != '|')
	{
		append_token(data, token, "|", PIPE);
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		if (input[++(*i)] == '<')
		{
			append_token(data, token, "<<", HERE_DOC);
			(*i)++;
		}
		else
			append_token(data, token, "<", REDIRECT_INP);
	}
	else if (input[*i] == '>')
	{
		if (input[++(*i)] == '>')
		{
			append_token(data, token, ">>", APPEND);
			(*i)++;
		}
		else
			append_token(data, token, ">", REDIRECT_OUT);
	}
}

char	*wraper_sign(t_data *data, char *input, int *i)
{
	int		pos;
	char	buffer[256];

	pos = 0;
	buffer[pos++] = input[(*i)++];
	while (input[(*i)] && (ft_isalnum(input[(*i)]) || input[(*i)] == '.'
			|| input[(*i)] == '/' || input[(*i)] == '='
			|| input[(*i)] == '?' || input[(*i)] == '~' || input[(*i)] == '*'))
		buffer[pos++] = input[(*i)++];
	buffer[pos] = '\0';
	return (gc_strdup(&data->gc, buffer));
}

char	*wraper_quotes(t_data *data, char *input, int *i, char c)
{
	char	buffer[256];
	int		pos;

	pos = 0;
	while (input[++(*i)] && input[*i] != c)
		buffer[pos++] = input[*i];
	buffer[pos] = '\0';
	(*i)++;
	return (gc_strdup(&data->gc, buffer));
}

void	bullshit2(t_data *data, t_token **token, int *i, char *input)
{
	char	*buf;

	if (input[*i] == '"')
	{
		buf = wraper_quotes(data, input, i, '\"');
		append_token(data, token, buf, DOUBLE_QUOTES);
	}
	else if (input[*i] == 39)
	{
		buf = wraper_quotes(data, input, i, 39);
		append_token(data, token, buf, SINGLE_QUOTES);
	}
	else if (input[*i] == '-')
	{
		buf = wraper_sign(data, input, i);
		append_token(data, token, buf, ARGS);
	}
}

