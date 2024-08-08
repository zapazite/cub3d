/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 10:42:46 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/08 10:26:48 by mde-prin         ###   ########.fr       */
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
# include <math.h>
# include "minilibx-linux/mlx.h"
# include <X11/X.h>
# include <X11/Xutil.h>
# include <math.h>

# define BUFFERSIZE 8192
# define MINIMAP_SCALE 20
# define WINDOW_H  500
# define WINDOW_W  500
# define PI 3.1415926535

typedef union {
    int32_t i;     // 32-bit integer representing the fixed-point number
    struct {
        int16_t lo; // 16-bit fractional part (lower bits)
        int16_t hi; // 16-bit integer part (higher bits)
    };
} fixed_point;

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

typedef struct s_parse
{
	int				min_x;
	int				max_x;
	int				min_y;
	int				max_y;
	int				**prs_map;
	char			*map_file;
} t_parse;

typedef struct s_mlx
{
	void			*mlx_ptr;
	void			*win_ptr;
	void			*image;
	int				pixel_bits;
	int				pixel_index;
	int				size_line;
	char			*img_data;
	int				endian;
} t_mlx;

typedef struct s_cube
{
	t_gc			*gc;
	t_parse			*prs;
	t_lines			*lines;
	t_mlx			*mlx;
	float			radius;
	fixed_point		player_x;
	fixed_point		player_y;
	fixed_point		rayx;
	fixed_point		rayy;
	float		player_dx;
	float		player_dy;
	fixed_point		rotated_x;
	fixed_point		rotated_y;
	float			player_angle;
	int32_t			dx_rot;
	int32_t			dy_rot;
	int				map_h;
	int				map_w;
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
void				render(t_cube *cube);
void				draw_pixel(t_cube *cube, int x, int y, int color);
void 				line_algo(t_cube * cube);
void				draw_square(t_cube *cube, int x_scaled , int y_scaled, int color);
#endif
