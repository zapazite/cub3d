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
#include <X11/Xlib.h>
# include "minilibx-linux/mlx.h" // IWYU pragma: keep
# include "get_next_line/get_next_line_bonus.h"
# include <fcntl.h>
#include <stdint.h>
# include <unistd.h>
# include <string.h> // IWYU pragma: keep
# include <math.h> // IWYU pragma: keep
# include <X11/X.h>
#include <dirent.h>
# include <X11/Xutil.h>

# define BUFFERSIZE 8192
# define WINDOW_H  1000
# define WINDOW_W  1920
# define PI 3.1415926535
# define GREEN 0x00ff00
# define FOV 90 * (PI / 180)
# define	MOUSE_SENS 0.001
# define	CLOSE_DOOR 1001.
# define	OPEN_DOOR 2000.
# define	MINIMAP_SCALE 15
# define	DOOR_SPEED 50
# define   ANIM_FRAMES 20

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
	int				map_p_bits;
	int				map_size_line;
	int				map_endian;
	int				main_p_bits;
	int				main_size_line;
	int				main_endian;
	int				mini_p_bits;
	int				mini_size_line;
	int				mini_endian;
	char			*main_data;
	char			*map_data;
	char			*mini_map_data;
	void			*mlx_ptr;
	void			*win_ptr;
	void			*map_img;
	void			*main_img;
	void			*mini_map_img;
} t_mlx;

typedef struct s_keys
{
	Bool mouse_left;
	Bool key_up;
	Bool key_down;
	Bool key_left;
	Bool key_right;
	Bool key_open;
	Bool key_close;
} t_keys;

typedef struct s_ray
{
	float			h_x;
	float			h_y;
	float			w_x;
	float			w_y;
	float			angle;
	float			slope;
	float			dx;
	float			dy;
	float			d_angle;
	int				hit_door_h;
	int				hit_door_w;
	int				door_state;
	int				door_check_flag;
	int				hit_door[WINDOW_W];
	float			hit_direction[WINDOW_W];
	float			hit_coordn[WINDOW_W];
	float			hit_dist[WINDOW_W];
}t_ray;

typedef struct s_textures
{
	char	wall_paths[7][4096];
	void	*wall_ptr[7];
	int 	wall_w[7];
	int		wall_h[7];
	int		*wall_data[7];
	int		wall_p_bits[7];
	int		wall_size_line[7];
	int		wall_endian[7];
} t_textures;

typedef struct s_anim
{
	char	paths[ANIM_FRAMES][4096];
	void	*ptr[ANIM_FRAMES];
	int 	w[ANIM_FRAMES];
	int		h[ANIM_FRAMES];
	int		*data[ANIM_FRAMES];
	int		p_bits[ANIM_FRAMES];
	int		size_line[ANIM_FRAMES];
	int		endian[ANIM_FRAMES];
	int		counter;
} t_anim;

typedef struct s_cube
{
	t_gc			*gc;
	t_parse			*prs;
	t_lines			*lines;
	t_mlx			*mlx;
	t_ray			*ray;
	t_keys			*keys;
	t_textures		*textures;
	t_anim			*anim;
	int				door_state;
	float			radius;
	float			pixel_player_dist;
	float			player_jump;
	float			player_x;
	float			player_y;
	float			player_dx;
	float			player_dy;
	float			player_angle;
	float			slope;
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
void				check_map(t_cube *cube, int x, int y);
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
void				rotate_player(t_cube *cube);
void				move_player(t_cube *cube);
void				draw_player(t_cube *cube);
void				draw_line(float rayx, float rayy, t_cube *cube);
void				draw_square(t_cube *cube, int x_scaled , int y_scaled, int color);
void				draw_minimap(t_cube *cube);
void				cast_h(int rayx, float rayy, t_cube *cube);
void				cast_w(float rayx, int rayy, t_cube *cube);
void				find_start_h(t_cube *cube);
void				find_start_w(t_cube *cube);
void				save_ray_info(t_cube *cube, int i, float d_player_screen);
void				ray_cast(t_cube *cube);
int					put_image(t_cube *cube);
void				copy_playable_map(t_cube *cube);
int					close_window(t_cube *cube);
int					key_handler(int keycode, t_cube *cube);
void				init_player(t_cube *cube);
void				ray_init(t_cube *cube);
void				render(t_cube *cube);
void				draw_main_pixel(t_cube *cube, int x, int y, int color);
void				draw_walls(t_cube *cube);
void				init_keyes(t_cube *cube);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
int					door_check(t_cube *cube, float x, float y, char line);
void				open_door(t_cube *cube);
void				close_door(t_cube *cube);
int					is_door(t_cube *cube);
#endif
