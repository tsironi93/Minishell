/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ffds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 20:05:31 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/04 17:27:11 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	_heredoc(char *del, int fd)
{
	char	*line;

	while ("Grapse Malaka")
	{
		line = readline("MalakaDoc> ");
		if (!line || !ft_strcmp(line, del))
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

static void	heredoc_final(t_data *data, t_heredoc *heredoc)
{
	int		i;
	char	**args;

	args[0] = "cat";
	args[1] = NULL;
	i = -1;
	while (++i < heredoc->num)
		waitpid(heredoc->pid[i], NULL, 0);
	dup2(heredoc->fd[i - 1][0], STDIN_FILENO);
	i = -1;
	while (++i < heredoc->num)
	{
		close (heredoc->fd[i][1]);
		close (heredoc->fd[i][0]);
	}
	execve("/bin/cat", args, data->env_full);
	perror("Execve failed");
}

static void	heredoc_init(t_data *data, t_heredoc *heredoc)
{
	int		i;
	int		pid;

	i = -1;
	while (++i < heredoc->num)
	{
		if (pipe(heredoc->fd[i]) == -1)
			return ((void)perror("Pipe error"));
		pid = fork();
		if (pid == -1)
			return ((void)perror("Fork error"));
		if (pid == 0)
		{
			close(heredoc->fd[i][0]);
			_heredoc(heredoc->del[i], heredoc->fd[i][1]);
		}
		else
		{
			close(heredoc->fd[i][1]);
			waitpid(pid, NULL, 0);
		}
	}
	heredoc_final(data, heredoc);
}

void	check_heredoc(t_data *data, t_token **token)
{
	int			i;
	t_token		*cur;
	t_heredoc	*doc;

	doc = malloc(sizeof(t_heredoc));
	data->heredoc = doc;
	data->heredoc->num = num_of_type(token, HERE_DOC, PIPE);
	if (data->heredoc->num < 1)
		return ;
	data->heredoc->del = malloc(data->heredoc->num * sizeof(char *));
	data->heredoc->pid = malloc(data->heredoc->num * sizeof(int));
	data->heredoc->fd = malloc(data->heredoc->num * sizeof(int *));
	i = -1;
	while (++i < data->heredoc->num)
		data->heredoc->fd[i] = malloc(2 * sizeof(int));
	i = -1;
	cur = *token;
	while (cur && cur->type != PIPE)
	{
		if (cur->type == HERE_DOC_OPT)
			data->heredoc->del[++i] = cur->value;
		cur = cur->next;
	}
	heredoc_init(data, data->heredoc);
}
