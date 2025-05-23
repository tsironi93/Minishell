/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:38:44 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/25 17:01:58 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isspace(int c)
{
	if ((c > 8 && c < 14) || c == 32)
		return (1);
	return (0);
}

void	merge(t_data *data, t_token **token)
{
	t_token	*current;
	char	*temp;

	current = *token;
	while (current && current->next)
	{
		if (current->type == ARGS && current->next->type == ARGS)
		{
			temp = gc_strdup(&data->gc, current->value);
			current->value = gc_strjoin(&data->gc, temp, current->next->value);
			current->next = current->next->next;
		}
		else
			current = current->next;
	}
}

void	go_at_start(t_token **token)
{
	t_token	*cur;

	cur = *token;
	while (cur->previous && cur->previous->type != PIPE)
		cur = cur->previous;
	*token = cur;
}

bool	check_files(t_data *data, t_token **token)
{
	t_token	*current;
	int		cmd;
	int		pipe;

	pipe = 0;
	cmd = 0;
	current = *token;
	while (current)
	{
		if (current->type == COMMAND || current->type == COMMAND_EX)
			cmd++;
		if (current->type == PIPE)
			pipe++;
		if (current->type == FILENAME_INP && access(current->value, F_OK) != 0)
		{
			if (num_of_type(token, PIPE, NULLL) != 0)
				return (true);
			else
				return (perror("Minishell: "), data->exit_code = 1, false);
		}
		current = current->next;
	}
	if (cmd <= pipe)
		return (data->exit_code = 2, redirections(data, token, false), false);
	return (true);
}

char	*trim_to_del(t_data *data, char *str, char del)
{
	int		i;
	char	buf[64];
	int		pos;
	int		start;

	start = 0;
	if (!str)
		return (NULL);
	if (!ft_strchr(str, del))
		return (str);
	if (str[0] != '/')
		return (str);
	i = 1;
	while (str[i])
		if (str[i++] == del)
			start = i;
	pos = 0;
	while (str[start])
		buf[pos++] = str[start++];
	buf[pos] = '\0';
	return (gc_strdup(&data->gc, buf));
}
