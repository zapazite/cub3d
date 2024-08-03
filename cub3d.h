/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 10:42:46 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/03 11:50:57 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "gc/gc.h"
# include <math.h>
# include "minilibx-linux/mlx.h"
# include "get_next_line/get_next_line_bonus.h"
# include <fcntl.h>
# include <unistd.h>

# define BUFFERSIZE 8192

typedef enum
{
	ERR_MALLOC,
	ERR_PARSING,
}					Errors;

typedef enum
{
	NO,
	SO,
	WE,
	EA,
}					TextureIndex;

typedef enum
{
	FLOOR,
	CIELLING,
}					ColorIndex;

typedef struct s_lines
{
	char			*line;
	struct s_lines	*next;

}					t_lines;

typedef struct s_cube
{
	t_gc			*gc;
	t_lines			*lines;
	void			*mlx_ptr;
	void			*win_ptr;
	float				spawn_x; //this will be position of player in map
	float				spawn_y; //this will be position of player in map
	int				map_h;
	int				map_w;
	char			*map_file;
	int				**map;
	char			paths[4][4096];
	int				colors[2];
}					t_cube;

void				clean_exit(t_cube *cube, int error_type);
void				parse(t_cube *cube);
void				parse_map(t_cube *cube, int fd);
void				check_spawn(t_cube *cube, int x, int y);
void				fill_map(t_cube *cube);
void				check_line(t_cube *cube, int *i, char *line);
void				add_line(t_cube *cube, char *line);
void				parse_parameter(t_cube *cube, int fd);
int					check_param(t_cube *cube, char *line);
int					check_rgb(t_cube *cube, char *line, int index);
int					rgb_atoi(const char *nptr);
int					check_path(t_cube *cube, char *line, int index);
size_t				ft_strlen(const char *s);
int					ft_strncmp(const char *str1, const char *str2, size_t n);
void				clean_exit(t_cube *cube, int error_type);
int					ft_isdigit(int c);
void				display_mini_map(t_cube *cube);
#endif
