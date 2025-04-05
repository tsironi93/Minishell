/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_colector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itsiros <itsiros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:26:35 by itsiros           #+#    #+#             */
/*   Updated: 2025/04/05 13:30:35 by itsiros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Allocate memory and track it
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

// Register a root (a pointer to a pointer)
void	gc_add_root(t_gc *gc, void **ptr)
{
	if (gc->root_count >= 100)
	{
		printf("Too many roots!\n");
		exit(1);
	}
	gc->roots[gc->root_count++] = ptr;
}

// Mark reachable memory
void	gc_mark_all(t_gc *gc)
{
	void	*root_ptr;
	t_gcobj	*cur;
	int		i;

	i = -1;
	while (++i < gc->root_count)
	{
		root_ptr = *gc->roots[i];
		cur = gc->objects;
		while (cur)
		{
			if (cur->ptr == root_ptr)
			{
				cur->marked = 1;
				break ;
			}
			cur = cur->next;
		}
	}
}

// Free unmarked memory
void	gc_sweep(t_gc *gc)
{
	t_gcobj	**cur;
	t_gcobj	*unreached;

	cur = &gc->objects;
	while (*cur)
	{
		if (!(*cur)->marked)
		{
			free((*cur)->ptr);
			unreached = *cur;
			*cur = unreached->next;
			free(unreached);
		}
		else
		{
			(*cur)->marked = 0;
			cur = &(*cur)->next;
		}
	}
}

// Perform t_gc
void	gc_collect(t_gc *gc)
{
	gc_mark_all(gc);
	gc_sweep(gc);
}

// Initialize the t_gc
t_gc	gc_new(void)
{
	t_gc	gc = {0};

	return (gc);
}

// Cleanup the t_gc (free everything)
void	gc_destroy(t_gc *gc)
{
	t_gcobj	*cur;
	t_gcobj	*next;

	cur = gc->objects;
	while (cur)
	{
		free(cur->ptr);
		next = cur->next;
		free(cur);
		cur = next;
	}
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
