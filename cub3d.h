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
# include <math.h>
# include "minilibx-linux/mlx.h"
# include <X11/X.h>
# include <X11/Xutil.h>
# include <math.h>

# define BUFFERSIZE 8192
# define MINIMAP_SCALE 15
# define WINDOW_H  1080
# define WINDOW_W  1920
# define PI 3.1415926535
# define GREEN 0x00ff00

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
	float			player_x;
	float			player_y;
	float			player_dx;
	float			player_dy;
	float			player_angle;
	float			slope;
	float			ray_h_x;
	float			ray_h_y;
	float			ray_w_x;
	float			ray_w_y;
	float			ray_angle;
	float			ray_slope;
	float			ray_dx;
	float			ray_dy;
	int				rotated_x;
	int				rotated_y;
	int				dx_rot;
	int				dy_rot;
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
int					check_player_position(float player_x, float player_y, t_cube *cube);
void				rotate_player(int keycode, t_cube *cube);
void				move_player(int keycode, t_cube *cube);
void				draw_pixel(t_cube *cube, int x, int y, int color);
void				draw_player(t_cube *cube);
void				draw_line(float rayx, float rayy, t_cube *cube);
void				draw_square(t_cube *cube, int x_scaled , int y_scaled, int color);
void				draw_minimap(t_cube *cube);
void				cast_h(int rayx, float rayy, t_cube *cube);
void				cast_w(float rayx, int rayy, t_cube *cube);
void				find_start_h(t_cube *cube);
void				find_start_w(t_cube *cube);
void				draw_nearest_ray(t_cube *cube);
void				ray_cast(t_cube *cube);
int					put_image(t_cube *cube);
void				copy_playable_map(t_cube *cube);
int					close_window(t_cube *cube);
int					key_handler(int keycode, t_cube *cube);
void				init_player(t_cube *cube);
void				init_ray(t_cube *cube);
void				render(t_cube *cube);
#endif
