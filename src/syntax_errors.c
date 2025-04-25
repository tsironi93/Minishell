/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:38:44 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/25 16:24:58 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_quotes(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
		{
			i++;
			while (input[i] && input[i] != '\'')
				i++;
			if (!input[i])
				return (false);
		}
		else if (input[i] == '"')
		{
			i++;
			while (input[i] && input[i] != '"')
				i++;
			if (!input[i])
				return (false);
		}
		i++;
	}
	return (true);
}
