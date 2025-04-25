/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:38:44 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/25 12:57:33 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* t_data	data;

init(ac, av, envp, &data);
while ("Malaka")
{
	clean(&data, false);
	if (isatty(fileno(stdin)))
		data.input = gc_readline(&data.gc, "~>:") */


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
