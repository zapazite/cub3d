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

void	draw_world(t_cube *cube)
{
	int			y;
	int			x;
	int			i;
	float		wall_idx;
	float		wall_size;
	float		wall_offset;

	y = -1;
	i = -1;
	while(++y < WINDOW_W)
	{
		x = -1;
		wall_idx = 0;
		wall_offset = 0;
		wall_size = WINDOW_W * sqrt(3) / cube->ray->hit_dist[y] / 2;
		if(wall_size > WINDOW_H)
			wall_offset = (wall_size - WINDOW_H) / 2 * cube->textures->wall_h[i] / wall_size;
		while(++x < WINDOW_H)
		{
			if(x < WINDOW_H / 2. - (wall_size / 2))
				draw_main_pixel(cube, x, y, cube->colors[CIELLING]);
			else if (x > WINDOW_H / 2. + (wall_size / 2))
				draw_main_pixel(cube, x, y, cube->colors[FLOOR]);
			else
			{
				draw_main_pixel(cube, x, y, cube->textures->wall_data[1][(int)(wall_idx + wall_offset) * cube->textures->wall_w[1]
					+ (int)(cube->ray->hit_coordn[y] * cube->textures->wall_h[1])]);
				wall_idx += cube->textures->wall_h[i] / wall_size;
			}
		}
	}
}
