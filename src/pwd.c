/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 08:59:47 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/22 16:14:12 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd_buildin(t_data *data)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		perror("getcwd");
		return (data->exit_code = errno);
	}
	printf("%s\n", buf);
	free(buf);
	return (data->exit_code = 0);
}



/* Upon successful completion, a pointer to the pathname is returned.  
Otherwise a NULL pointer is returned and the global variable errno is set to indicate the error. 
 In addition, getwd() copies the error message associated with 
 errno into the memory referenced by buf. */