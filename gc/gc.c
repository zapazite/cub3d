/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 11:38:06 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/01 11:38:26 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"
#include "gc.h"
#include <stdlib.h>

//GC CAN NOW FREE SPECIFIC POINTER. IF NULL IS PASSED AS PARAMETER
//FT_FREE WILL FREE THE ENTIRE LIST

void	ft_free_ptr(t_gc *gc, void *ptr_2_free)
{
	t_gc	**current;
	t_gc	**tmp;

	current = &gc;
	while(*current && (*current)->malloced_ptr != ptr_2_free)
		current = &(*current)->next;
	if(!*current)
		return ;
	tmp = current;
	*current = (*current)->next;
	free((*tmp)->malloced_ptr);
	free(*tmp);
}

void	ft_free_gc(t_gc *gc)
{
    t_gc *tmp;

    while (gc)
    {
	    free(gc->malloced_ptr);
	    tmp = gc->next;
	    free(gc);
	    gc = tmp;
	}
}

void    *ft_malloc(t_cube *cube, int size)
{
    void *ptr_2_malloc;
    t_gc *gc_new_node;

    ptr_2_malloc = malloc(size);
    if(!ptr_2_malloc)
		clean_exit(cube, ERR_MALLOC);
    gc_new_node = malloc(sizeof(t_gc));
    if(!gc_new_node)
        (free(ptr_2_malloc), clean_exit(cube, ERR_MALLOC));
    gc_new_node->malloced_ptr = ptr_2_malloc;
    gc_new_node->next = cube->gc;
    cube->gc = gc_new_node;
    return (ptr_2_malloc);
}
