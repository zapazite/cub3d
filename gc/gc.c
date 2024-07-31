/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 11:38:06 by efaiz             #+#    #+#             */
/*   Updated: 2024/07/31 13:05:22 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include <stdio.h>
#include <stdlib.h>

void ft_free(t_gc *gc)
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

void    *ft_malloc(t_gc **gc, int size)
{
    void *ptr_2_malloc;
    t_gc *gc_new_node;

    ptr_2_malloc = malloc(size);
    if(!ptr_2_malloc)
        return (NULL);
    gc_new_node = malloc(sizeof(t_gc));
    if(!gc_new_node)
        return (free(ptr_2_malloc), NULL);
    gc_new_node->malloced_ptr = ptr_2_malloc;
    gc_new_node->next = *gc;
    *gc = gc_new_node;
    return (ptr_2_malloc);
}
