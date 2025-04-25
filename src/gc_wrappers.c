/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_wrappers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckappe <ckappe@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:47:25 by ckappe            #+#    #+#             */
/*   Updated: 2025/04/25 11:50:40 by ckappe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	gc_add_root(t_gc *gc, void **ptr)
{
	if (gc->root_count >= 100)
	{
		printf("Too many roots!\n");
		exit(1);
	}
	gc->roots[gc->root_count++] = ptr;
}

void	*gc_malloc(t_gc *gc, size_t size)
{
	void	*mem;
	t_gcobj	*obj;

	mem = malloc(size);
	if (!mem)
	{
		perror("malloc failed");
		exit(1);
	}
	obj = malloc(sizeof(t_gcobj));
	if (!obj)
	{
		perror("malloc t_gcobj failed");
		exit(1);
	}
	obj->ptr = mem;
	obj->marked = 0;
	obj->next = gc->objects;
	gc->objects = obj;
	return (mem);
}

char	*gc_strdup(t_gc *gc, const char *s)
{
	char	*dup;
	t_gcobj	*obj;

	dup = ft_strdup(s);
	if (!dup)
	{
		perror("strdup failed");
		exit(1);
	}
	obj = malloc(sizeof(t_gcobj));
	if (!obj)
	{
		perror("malloc GCObject failed");
		exit(1);
	}
	obj->ptr = dup;
	obj->marked = 0;
	obj->next = gc->objects;
	gc->objects = obj;
	return (dup);
}

char	*gc_readline(t_gc *gc, const char *prompt)
{
	t_gcobj	*obj;
	char	*line;

	line = readline(prompt);
	if (!line)
		return (NULL);
	obj = malloc(sizeof(t_gcobj));
	if (!obj)
	{
		perror("malloc GCObject failed");
		exit(1);
	}
	obj->ptr = line;
	obj->marked = 0;
	obj->next = gc->objects;
	gc->objects = obj;
	return (line);
}

char	*gc_strjoin(t_gc *gc, char *s1, char *s2)
{
	char	*joined;
	t_gcobj	*obj;

	joined = ft_strjoin(s1, s2);
	if (!joined)
		return (NULL);
	obj = malloc(sizeof(t_gcobj));
	if (!obj)
	{
		perror("malloc GCObject failed");
		exit(1);
	}
	obj->ptr = joined;
	obj->marked = 0;
	obj->next = gc->objects;
	gc->objects = obj;
	return (joined);
}
