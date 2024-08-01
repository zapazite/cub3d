/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 10:42:46 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/01 08:43:14 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <fcntl.h>
# include <unistd.h>
# include "gc/gc.h"
# include "get_next_line/get_next_line_bonus.h"

# define BUFFERSIZE 8192

typedef struct s_cube
{
    t_gc	*gc;
    char	*no_path;
    char	*so_path;
    char	*we_path;
    char	*ea_path;

} t_cube;

#endif
