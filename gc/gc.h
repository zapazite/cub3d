/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 11:38:29 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/01 08:39:32 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_H

# define GC_H
# include <stdio.h>
# include <stdlib.h>

typedef struct s_cube t_cube ;

typedef struct s_gc
{
    void		*malloced_ptr;
    struct s_gc	*next;
    struct s_gc *prev;
} t_gc;

void	ft_free_gc(t_gc *gc);
void	ft_free_ptr(t_gc *gc, void *ptr_2_free);
void    *ft_malloc(t_cube *cube, int size);

#endif
