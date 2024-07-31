/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 11:38:29 by efaiz             #+#    #+#             */
/*   Updated: 2024/07/31 11:58:55 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_H

# define GC_H

typedef struct s_gc
{
    void         *malloced_ptr;
    struct s_gc   *next;
} t_gc;

#endif