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
#include <stdlib.h>

//GC CAN NOW FREE SPECIFIC POINTER. IF NULL IS PASSED AS PARAMETER
//FT_FREE WILL FREE THE ENTIRE LIST

void	cut_and_join(t_gc *gc_runner)
{
	t_gc *tmp;

	if(!gc_runner->prev)
	{
		tmp = gc_runner->next;
		free(gc_runner->malloced_ptr);
	}
	else if(!gc_runner->next)
	{
		tmp = gc_runner->prev;
		free(gc_runner->malloced_ptr);
	}
	else
	{
		gc_runner->prev->next = gc_runner->next;
		free(gc_runner->malloced_ptr);
		gc_runner->next->prev = gc_runner->prev;
		return ;
	}
	free(gc_runner);
	gc_runner = tmp;
}

void	ft_free_ptr(t_gc *gc, void *ptr_2_free)
{
	t_gc *gc_runner;

	gc_runner = gc;
	while(gc_runner && gc_runner->malloced_ptr != ptr_2_free)
		gc_runner = gc_runner->next;
	if(!gc_runner)
		printf("Cannot find pointer in gc");
	else
		cut_and_join(gc_runner);
}

void	ft_free(t_gc *gc, void *ptr_2_free)
{
    t_gc *tmp;

    tmp = NULL;
    if(!ptr_2_free)
    {
	    while (gc)
	    {
	        free(gc->malloced_ptr);
	        tmp = gc->next;
	        free(gc);
	        gc = tmp;
		}
		return ;
    }
    else
    	ft_free_ptr(gc, ptr_2_free);
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
    if(!cube->gc)
    	gc_new_node->prev = NULL;
    else
    	gc_new_node->prev = cube->gc;
    return (ptr_2_malloc);
}
