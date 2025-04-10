/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:56:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/06 13:16:11 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	bullshit(t_data *data, t_token **token, int *i, char *input)
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

static char	*wraper_sign(t_data *data, char *input, int *i)
{
	int		pos;
	char	buffer[256];

	pos = 0;
	buffer[pos++] = input[(*i)++];
	while (input[(*i)] && (ft_isalnum(input[(*i)]) || input[(*i)] == '$'
			|| input[(*i)] == '.' || input[(*i)] == '/' || input[(*i)] == '='
			|| input[(*i)] == '?'))
		buffer[pos++] = input[(*i)++];
	buffer[pos] = '\0';
	return (gc_strdup(&data->gc, buffer));
}

static char	*wraper_quotes(t_data *data, char *input, int *i, char c)
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

static void	bullshit2(t_data *data, t_token **token, int *i, char *input)
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

void	lexer(t_data *data, char *input, t_token **token)
{
	int		i;
	char	*buf;

	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			append_token(data, token, "", ISSPACE);
			i++;
		}
		else if (ft_isalnum(input[i]) || input[i] == '$' || input[i] == '.'
			|| input[i] == '/' || input[i] == '=' || input[i] == '?')
		{
			buf = wraper_sign(data, input, &i);
			append_token(data, token, buf, UNKNOWN);
		}
		else if (input[i] == '"' || input[i] == 39 || input[i] == '-')
			bullshit2(data, token, &i, input);
		else if ((input[i] == '|' && input[i + 1] != '|') || input[i] == '>'
			|| input[i] == '<')
			bullshit(data, token, &i, input);
		else
			return ((void)printf("Sorry cant handle this!\n"));
	}
}
