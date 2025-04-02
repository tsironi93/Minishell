/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ffds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 20:05:31 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/02 17:15:47 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	heredoc(char *del, int fd)
{
	char	*line;

	while ("Grapse Malaka")
	{
		line = readline("MalakaDoc> ");
		if (!line)
			break ;
		if (!ft_strcmp(line, del))
		{
			free (line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free (line);
	}
	close (fd);
	exit(0);
}

void	heredoc_init(t_data *data, char *del)
{
	int		pipe_fd[2];
	int		pid;
	char	*args[] = {"cat", NULL};

	if (pipe(pipe_fd) == -1)
		return ((void)perror("Pipe error"));
	pid = fork();
	if (pid == -1)
		return ((void)perror("Fork error"));
	if (pid == 0)
	{
		close (pipe_fd[0]);
		heredoc(del, pipe_fd[1]);
	}
	waitpid(pid, NULL, 0);
	close (pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close (pipe_fd[0]);
	execve("/bin/cat", args, data->env_full);
	perror("Execve failed");
}

void	check_hedoc(t_data *data, t_token **token)
{
	int		num_files;
	int		i;
	t_token	*cur;

	num_files = num_of_type(token, HERE_DOC, PIPE);
	if (num_files < 1)
		return ;
	i = -1;
	cur = *token;
	while (cur && cur->type != PIPE)
	{
		if (cur->type == HERE_DOC_OPT)
			heredoc_init(data, cur->value);
		cur = cur->next;
	}
}
