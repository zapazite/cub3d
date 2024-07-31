/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 11:38:29 by efaiz             #+#    #+#             */
/*   Updated: 2024/07/31 13:05:27 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_H

# define GC_H

typedef struct s_gc
{
    void		*malloced_ptr;
    struct s_gc	*next;
} t_gc;

void	ft_free(t_gc *gc);
void    *ft_malloc(t_gc **gc, int size);

#endif
