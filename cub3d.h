/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 10:42:46 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/01 12:03:20 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <fcntl.h>
# include <unistd.h>
# include "gc/gc.h"
# include "get_next_line/get_next_line_bonus.h"

# define BUFFERSIZE 8192

typedef enum {
	ERR_MALLOC,
	ERR_PARSING,
} Errors;

typedef enum {
    NO,
    SO,
    WE,
    EA,
} TextureIndex;

typedef enum {
    FLOOR,
    CIELLING,
} ColorIndex;

typedef struct s_parse
{
	char identifier[4];
	int (*id_parse)(t_cube *cube, char *line);
	int already_seen;
} t_parse;

typedef struct s_cube
{
    t_gc	*gc;
	int		**map;
    char	paths[4][4096];
	int		colors[2];
} t_cube;

#endif
