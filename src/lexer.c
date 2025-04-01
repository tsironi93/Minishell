/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:56:40 by itsiros           #+#    #+#             */
/*   Updated: 2025/03/31 09:18:40 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	bullshit(t_token **token, int *i, char *input)
{
	if (input[*i] == '|' && input[*i + 1] != '|')
	{
		append_token(token, "|", PIPE);
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		if (input[++(*i)] == '<')
		{
			append_token(token, "<<", HERE_DOC);
			(*i)++;
		}
		else
			append_token(token, "<", REDIRECT_INP);
	}
	else if (input[*i] == '>')
	{
		if (input[++(*i)] == '>')
		{
			append_token(token, ">>", APPEND);
			(*i)++;
		}
		else
			append_token(token, ">", REDIRECT_OUT);
	}
}

static char	*wraper_sign(char *input, int *i)
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
	return (ft_strdup(buffer));
}

static char	*wraper_quotes(char *input, int *i, char c)
{
	char	buffer[256];
	int		pos;

	pos = 0;
	while (input[++(*i)] && input[*i] != c)
		buffer[pos++] = input[*i];
	buffer[pos] = '\0';
	(*i)++;
	return (ft_strdup(buffer));
}

static void	bullshit2(t_token **token, int *i, char *input)
{
	char	*buf;

	if (input[*i] == '"')
	{
		buf = wraper_quotes(input, i, '\"');
		append_token(token, buf, DOUBLE_QUOTES);
		free(buf);
	}
	else if (input[*i] == 39)
	{
		buf = wraper_quotes(input, i, 39);
		append_token(token, buf, SINGLE_QUOTES);
		free(buf);
	}
	else if (input[*i] == '-')
	{
		buf = wraper_sign(input, i);
		append_token(token, buf, ARGS);
		free(buf);
	}
}

void	lexer(char *input, t_token **token)
{
	int		i;
	char	*buf;

	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		if (ft_isalnum(input[i]) || input[i] == '$' || input[i] == '.'
			|| input[i] == '/' || input[i] == '=' || input[i] == '?')
		{
			buf = wraper_sign(input, &i);
			append_token(token, buf, UNKNOWN);
			free(buf);
		}
		else if (input[i] == '"' || input[i] == 39 || input[i] == '-')
			bullshit2(token, &i, input);
		else if ((input[i] == '|' && input[i + 1] != '|') || input[i] == '>'
			|| input[i] == '<')
			bullshit(token, &i, input);
		else
			return ((void)printf("Sorry cant handle this!\n"));
	}
}
