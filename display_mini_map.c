#include "cub3d.h"
#include "gc/gc.h"
#include "minilibx-linux/mlx.h"
#include <X11/Xutil.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>

int check_player_position(float player_x, float player_y, t_cube *cube)
{
	if(cube->map[(int)((player_x + cube->radius))][(int)((player_y + cube->radius))] == '#')
		return (0);
	if(cube->map[(int)((player_x + cube->radius))][(int)((player_y - cube->radius))] == '#')
		return (0);
	if(cube->map[(int)((player_x - cube->radius))][(int)((player_y + cube->radius))] == '#')
		return (0);
	if(cube->map[(int)((player_x - cube->radius))][(int)((player_y - cube->radius))] == '#')
		return (0);
	return (1);
}

void rotate_player(int keycode, t_cube *cube)
{
	if(keycode == XK_Right)
	{
		cube->player_angle -= 0.01;
		if(cube->player_angle < 0)
			cube->player_angle += 2*PI;
		cube->player_dx = cos(cube->player_angle);
		cube->player_dy = sin(cube->player_angle);
	}
	else if(keycode == XK_Left)
	{
		cube->player_angle += 0.01;
		if(cube->player_angle > 2*PI)
			cube->player_angle -= 2*PI;
		cube->player_dx = cos(cube->player_angle);
		cube->player_dy = sin(cube->player_angle);
	}
}

void move_player(int keycode, t_cube *cube)
{
	float move_x;
	float move_y;

	move_x = cube->player_x;
	move_y = cube->player_y;
	if(keycode == XK_Up)
	{
		move_x += cube->player_dx/MINIMAP_SCALE;
		move_y += cube->player_dy/MINIMAP_SCALE;
	}
	else if(keycode == XK_Down)
	{
		move_x -= cube->player_dx/MINIMAP_SCALE;
		move_y -= cube->player_dy/MINIMAP_SCALE;
	}
	if(check_player_position(move_x, move_y, cube))
	{
		cube->player_x = move_x;
		cube->player_y = move_y;
	}
}

void draw_pixel(t_cube *cube, int x, int y, int color)
{
	int pixel_index;
	if(x < 0 || y < 0 || x > cube->map_h*MINIMAP_SCALE || y > cube->map_w*MINIMAP_SCALE)
		return ;
	pixel_index = (x * cube->mlx->size_line) + (y * (cube->mlx->pixel_bits / 8));
	cube->mlx->img_data[pixel_index] = color & 0xFF;
    cube->mlx->img_data[pixel_index + 1] = (color >> 8) & 0xFF;
    cube->mlx->img_data[pixel_index + 2] = (color >> 16) & 0xFF;
}

void player_movement_calc(t_cube *cube, int x, int y)
{
	double  cos_a;
    double  sin_a;

    cos_a = cos(cube->player_angle);
    sin_a = sin(cube->player_angle);
    cube->dx_rot = x - cube->radius * MINIMAP_SCALE;
    cube->dy_rot = y - cube->radius * MINIMAP_SCALE;
	cube->rotated_x = cos_a * cube->dx_rot - sin_a * cube->dy_rot;
    cube->rotated_y = sin_a * cube->dx_rot + cos_a * cube->dy_rot;
}

void draw_player(t_cube *cube, int color)
{
	int		x;
	int		y;

	x = -1;
	while(++x < MINIMAP_SCALE)
	{
		y = -1;
		while(++y < MINIMAP_SCALE)
		{
			player_movement_calc(cube, x, y);
			if(pow(cube->radius*MINIMAP_SCALE - x, 2) + pow(cube->radius*MINIMAP_SCALE - y, 2) <= pow(cube->radius*MINIMAP_SCALE, 2))
			{
				if(x >= MINIMAP_SCALE / 2)
					color = 0xFFFFFF;
				draw_pixel(cube, cube->rotated_x + cube->player_x * MINIMAP_SCALE, cube->rotated_y + cube->player_y * MINIMAP_SCALE, color);
			}
		}
	}
}

void	draw_line(float rayx, float rayy, t_cube *cube)
{
	float	deltax;
	float	deltay;
	float		xi;
	float		yi;
	int			i = -1;

	deltax = (rayx - cube->player_x) / 1000;
	deltay = (rayy - cube->player_y) / 1000;
	// cube->slope = (rayy - cube->player_y) / (rayx - cube->player_x);
	xi = cube->player_x;
	yi = cube->player_y;
	while(++i < 1000)
	{
		draw_pixel(cube, xi * MINIMAP_SCALE, yi * MINIMAP_SCALE, 0xffffff);
		xi += deltax;
		yi += deltay;
	}
}

void draw_square(t_cube *cube, int x_scaled , int y_scaled, int color)
{
	int pixel_size_x;
	int pixel_size_y;

	pixel_size_x = -1;
	while(++pixel_size_x < MINIMAP_SCALE)
	{
		pixel_size_y = -1;
		while(++pixel_size_y < MINIMAP_SCALE)
		{
			draw_pixel(cube, pixel_size_x + x_scaled, pixel_size_y + y_scaled, color);
			if (pixel_size_x == 0 || pixel_size_y == 0)
							draw_pixel(cube, pixel_size_x + x_scaled, pixel_size_y + y_scaled, 0x000000);
		}
	}
}

void draw_minimap(t_cube *cube)
{
	int x = 0;
	int y = 0;

	x = cube->map_h;
	while(x-- > 0)
	{
		y = cube->map_w;
		while(y-- > 0)
		{
			if(cube->map[x][y] == '!')
				draw_square(cube, x * MINIMAP_SCALE, y * MINIMAP_SCALE, 0x0066b2);
			else if(cube->map[x][y] == '#')
				draw_square(cube, x * MINIMAP_SCALE, y * MINIMAP_SCALE, 0x990000);
		}
	}
}

void	raycast_h(int rayx, float rayy, t_cube *cube)
{
	while(rayx < cube->map_h && (int)rayy < cube->map_w && rayx > 0 && rayy > 0 &&cube->map[rayx - (cube->ray_dx < 0)][(int)rayy] != '#')
	{
		if(cube->ray_dx > 0)
		{
			rayx++;
			rayy += cube->ray_slope;
		}
		else
		{
			rayx--;
			rayy -= cube->ray_slope;
		}
	}
	cube->ray_h_x = rayx;
	cube->ray_h_y = rayy;
}

void	raycast_w(float rayx, int rayy, t_cube *cube)
{
	while((int)rayx < cube->map_h && rayy < cube->map_w && rayx > 0 && rayy > 0 &&cube->map[(int)rayx][rayy - (cube->ray_dy < 0)] != '#')
	{
		if(cube->ray_dy > 0)
		{
			rayy++;
			rayx += cube->ray_slope;
		}
		else
		{
			rayy--;
			rayx -= cube->ray_slope;
		}
	}
	cube->ray_w_x = rayx;
	cube->ray_w_y = rayy;
}

void	find_start_h(t_cube *cube)
{
	int		rayx;
	float	rayy;

	rayx = 0;
	rayy = 0;
	cube->ray_slope = cube->ray_dy / cube->ray_dx;
	if(cube->ray_dx == 0)
		return ;
	if(cube->ray_dx > 0)
		rayx = (int)(cube->player_x + 1);
	else
		rayx = (int)cube->player_x;
	rayy = (rayx - cube->player_x) * cube->ray_slope + cube->player_y;
	raycast_h(rayx, rayy, cube);
}

void	find_start_w(t_cube *cube)
{
	int		rayy;
	float	rayx;

	rayx = 0;
	rayy = 0;
	cube->ray_slope = cube->ray_dx / cube->ray_dy;
	if(cube->ray_dy == 0)
		return ;
	if(cube->ray_dy > 0)
		rayy = (int)(cube->player_y + 1);
	else
		rayy = (int)cube->player_y;
	rayx = (rayy - cube->player_y) * cube->ray_slope + cube->player_x;
	raycast_w(rayx, rayy, cube);
}


void	draw_nearest_ray(t_cube *cube)
{

	if(pow(cube->player_x - cube->ray_h_x, 2) + pow(cube->player_y - cube->ray_h_y, 2) <=  pow(cube->player_x - cube->ray_w_x, 2) + pow(cube->player_y - cube->ray_w_y, 2))
		draw_line(cube->ray_h_x, cube->ray_h_y, cube);
	else
		draw_line(cube->ray_w_x, cube->ray_w_y, cube);
}

void	ray_cast(t_cube *cube)
{
	cube->ray_angle = cube->player_angle;
	cube->ray_angle += 0.523599;
	cube->ray_dx = cos(cube->ray_angle);
	cube->ray_dy = sin(cube->ray_angle);
	while(cube->ray_angle > cube->player_angle - 0.523599)
	{
		find_start_h(cube);
		find_start_w(cube);
		draw_nearest_ray(cube);
		cube->ray_angle -= 0.01;
		cube->ray_dx = cos(cube->ray_angle);
		cube->ray_dy = sin(cube->ray_angle);
	}
}

int put_image(t_cube *cube)
{
	draw_minimap(cube);
	draw_player(cube, 0x46eb34);
	ray_cast(cube);
	mlx_put_image_to_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr, cube->mlx->image, 0, 0);
	return 0;
}

void copy_player_map(t_cube *cube)
{
	int x;
	int	y;

	cube->map_h = (cube->prs->max_x - cube->prs->min_x) + 1;
	cube->map_w = (cube->prs->max_y - cube->prs->min_y) + 1;
	cube->map = (int **)ft_malloc(cube, sizeof(int *) *cube->map_h);
	x = -1;
	while(++x < cube->map_h)
	{
		cube->map[x] = (int *)ft_malloc(cube, sizeof(int) * cube->map_w);
		y = -1;
		while(++y < cube->map_w)
		{
			cube->map[x][y] = cube->prs->prs_map[cube->prs->min_x + x][cube->prs->min_y + y];
			if(cube->map[x][y] == '1')
				cube->map[x][y] = '#';
		}
	}
}

int close_window(t_cube *cube)
{
	ft_free_gc(cube->gc);
	mlx_destroy_image(cube->mlx->mlx_ptr, cube->mlx->image);
	mlx_destroy_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr);
	mlx_destroy_display(cube->mlx->mlx_ptr);
	free(cube->mlx->mlx_ptr);
	exit(0);
}

int key_handler(int keycode, t_cube *cube)
{
	//make it more modular
	move_player(keycode, cube);
	if(keycode == XK_Escape)
		close_window(cube);
	if(keycode == XK_Up || keycode == XK_Down)
		move_player(keycode, cube);
	if(keycode == XK_Left || keycode == XK_Right)
		rotate_player(keycode, cube);
	return (0);
}

void render(t_cube *cube)
{
	copy_player_map(cube);
	for (int i = 0; i < cube->map_h; i++)
	{
	 	for (int j = 0; j < cube->map_w; j++)
			printf("%c", cube->map[i][j]);
		printf("\n");
	}
	cube->mlx->mlx_ptr = mlx_init();
	cube->mlx->win_ptr = mlx_new_window(cube->mlx->mlx_ptr, WINDOW_W, WINDOW_H, "diplsay mini_map");
	cube->mlx->image = mlx_new_image(cube->mlx->mlx_ptr, cube->map_w*MINIMAP_SCALE, cube->map_h*MINIMAP_SCALE);
	cube->mlx->img_data = mlx_get_data_addr(cube->mlx->image, &cube->mlx->pixel_bits, &cube->mlx->size_line, &cube->mlx->endian);
	cube->player_x += 0.5;
	cube->player_y += 0.5;
	cube->radius = 0.3;
	cube->player_angle = 0;
	cube->player_dx = cos(cube->player_angle);
	cube->player_dy = sin(cube->player_angle);

	mlx_hook(cube->mlx->win_ptr, 17, 0, close_window, cube);
	mlx_hook(cube->mlx->win_ptr, KeyPress,KeyPressMask, key_handler, cube);
	mlx_loop_hook(cube->mlx->mlx_ptr, put_image, cube);
	mlx_loop(cube->mlx->mlx_ptr);
}
