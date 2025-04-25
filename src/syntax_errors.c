/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:38:44 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/25 13:00:13 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_quotes(char *input)
{
	int	single_qc;
	int	double_qc;
	int	i;

	i = 0;
	single_qc = 0;
	double_qc = 0;
	while (input[i])
	{
		if (input[i] == '\'')
			single_qc++;
		else if (input[i] == '"')
			double_qc++;
		i++;
	}
	if (single_qc % 2 != 0 || double_qc % 2 != 0)
		return (false);
	else
		return (true);
}
