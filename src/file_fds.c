/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ffds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 20:05:31 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/02 12:46:13 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
	if (type == HERE_DOC)
	{
		cur = *token;
		i = -1;
		while (cur && cur->type != PIPE)
		{
			if (cur->type == HERE_DOC_OPT)
			{
				redirect_fd[++i] = open(".heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (redirect_fd[i] < 0)
					return (perror("file didnt open"), false);
				break ;
			}
		}
		p();
		char *heredoc_inp = NULL;
		heredoc_inp = readline("> ");
		printf("heredoc input '%s'\n", heredoc_inp);
		while (!ft_strcmp(heredoc_inp, cur->value))
		{
			p();
			free(heredoc_inp);
			heredoc_inp = readline("> ");
			printf("heredoc input '%s'\n", heredoc_inp);
		}
		p();
		close (redirect_fd[i]);
		return (false);
//			cur = cur->next;
//			i = -1;
//			while (++i < num_redirects)
//			{
//				dup2(redirect_fd[i], STDIN_FILENO);
//				close(redirect_fd[i]);
//			}

	}


*/

