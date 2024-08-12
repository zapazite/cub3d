#include "cub3d.h"

void	draw_map_pixel(t_cube *cube, int x, int y, int color)
{
	int pixel_index;
	if(x < 0 || y < 0 || x > cube->map_h*MINIMAP_SCALE || y > cube->map_w*MINIMAP_SCALE)
		return ;
	if(x > WINDOW_H || y > WINDOW_W)
		return ;
	pixel_index = (x * cube->mlx->map_size_line) + (y * (cube->mlx->map_p_bits / 8));
	cube->mlx->map_data[pixel_index] = color & 0xFF;
    cube->mlx->map_data[pixel_index + 1] = (color >> 8) & 0xFF;
    cube->mlx->map_data[pixel_index + 2] = (color >> 16) & 0xFF;
}

void	draw_main_pixel(t_cube *cube, int x, int y, int color)
{
	int pixel_index;
	if(x < 0 || y < 0 || x > WINDOW_H || y > WINDOW_W)
		return ;
	pixel_index = (x * cube->mlx->main_size_line) + (y * (cube->mlx->main_p_bits / 8));
	cube->mlx->main_data[pixel_index] = color & 0xFF;
    cube->mlx->main_data[pixel_index + 1] = (color >> 8) & 0xFF;
    cube->mlx->main_data[pixel_index + 2] = (color >> 16) & 0xFF;
}

void	draw_player(t_cube *cube)
{
	int		x;
	int		y;

	x = -1;
	while(++x < MINIMAP_SCALE)
	{
		y = -1;
		while(++y < MINIMAP_SCALE)
			if(pow(cube->radius*MINIMAP_SCALE - x, 2) + pow(cube->radius*MINIMAP_SCALE - y, 2) <= pow(cube->radius*MINIMAP_SCALE, 2))
				draw_map_pixel(cube, (cube->player_x - cube->radius) * MINIMAP_SCALE + x, (cube->player_y - cube->radius) * MINIMAP_SCALE + y, GREEN);
	}
}

void	draw_line(float rayx, float rayy, t_cube *cube)
{
	float		xi;
	float		yi;
	int			i;

	xi = cube->player_x;
	yi = cube->player_y;
	i = -1;
	while(++i < 1000)
	{
		draw_map_pixel(cube, xi * MINIMAP_SCALE, yi * MINIMAP_SCALE, 0xffffff);
		xi += (rayx - cube->player_x) / 1000; //calue to be changed based on map dimensions. Defines how many pixels to color on a line
		yi += (rayy - cube->player_y) / 1000;
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
			draw_map_pixel(cube, pixel_size_x + x_scaled, pixel_size_y + y_scaled, color);
			if (pixel_size_x == 0 || pixel_size_y == 0)
				draw_map_pixel(cube, pixel_size_x + x_scaled, pixel_size_y + y_scaled, 0x000000);
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

void	save_ray_distance(t_cube *cube, int i, float d_player_screen)
{
	float h_hit;
	float w_hit;

	h_hit = pow(cube->player_x - cube->ray->h_x, 2) + pow(cube->player_y - cube->ray->h_y, 2);
	w_hit = pow(cube->player_x - cube->ray->w_x, 2) + pow(cube->player_y - cube->ray->w_y, 2);
	if(h_hit <= w_hit)
		cube->ray->hit_points[i] = sqrt(h_hit) * d_player_screen / (sqrt(pow(cube->ray->dx, 2) + pow(cube->ray->dy, 2)));
	else
		cube->ray->hit_points[i] = sqrt(w_hit) * d_player_screen / (sqrt(pow(cube->ray->dx, 2) + pow(cube->ray->dy, 2)));
}
