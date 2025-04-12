/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 08:59:47 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/12 13:36:36 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd_buildin(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		perror("getcwd");
		return (errno);
	}
	printf("%s\n", buf);
	free(buf);
	return (EXIT_SUCCESS);
}



/* Upon successful completion, a pointer to the pathname is returned.  
Otherwise a NULL pointer is returned and the global variable errno is set to indicate the error. 
 In addition, getwd() copies the error message associated with 
 errno into the memory referenced by buf. */