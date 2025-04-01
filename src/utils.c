/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:38:44 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/01 14:56:14 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isspace(int c)
{
	if ((c > 8 && c < 14) || c == 32)
		return (1);
	return (0);
}

void	free2d(char **a)
{
	char	**temp;

	temp = a;
	if (!a)
		return ;
	while (*a)
	{
		free(*a);
		a++;
	}
	free(temp);
}

void	go_at_start(t_token **token)
{
	t_token	*cur;

	cur = *token;
	while (cur->previous && cur->previous->type != PIPE)
		cur = cur->previous;
	*token = cur;
}

bool	check_files(t_token **token)
{
	t_token	*current;

	current = *token;
	while (current)
	{
		if (current->type == FILENAME_INP && access(current->value, F_OK) != 0)
			return (perror("Minishell: "), false);
		current = current->next;
	}
	return (true);
}

char	*trim_to_del(char *str, char del)
{
	int		i;
	char	buf[64];
	int		pos;

	if (!str)
		return (NULL);
	if (!ft_strchr(str, del))
		return (str);
	if (str[0] != '/')
		return (str);
	i = 1;
	while (str[i])
		if (str[i++] == del)
			break ;
	pos = 0;
	while (str[i])
		buf[pos++] = str[i++];
	buf[pos] = '\0';
	return (ft_strdup(buf));
}
