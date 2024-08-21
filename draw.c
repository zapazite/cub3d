/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <@student.42luxembourg.lu>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 08:55:06 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/20 08:55:12 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_map_pixel(t_cube *cube, int x, int y, int color)
{
	int	pixel_idx;

	if (x < 0 || y < 0 || x > cube->map_h * MINIMAP_SCALE
		|| y > cube->map_w * MINIMAP_SCALE)
		return ;
	if (x > WINDOW_H || y > WINDOW_W)
		return ;
	pixel_idx = x * cube->mlx->map_size_line + y * cube->mlx->map_p_bits / 8;
	cube->mlx->map_data[pixel_idx] = color & 0xFF;
	cube->mlx->map_data[pixel_idx + 1] = (color >> 8) & 0xFF;
	cube->mlx->map_data[pixel_idx + 2] = (color >> 16) & 0xFF;
}

void	draw_mini_map_pixel(t_cube *cube, int x, int y, int color)
{
	int	pixel_idx;

	if (x < 0 || y < 0 || x > 10 * MINIMAP_SCALE || y > 10 * MINIMAP_SCALE)
		return ;
	if (x > 150 || y > 150)
		return ;
	pixel_idx = x * cube->mlx->mini_size_line + y * cube->mlx->mini_p_bits / 8;
	cube->mlx->mini_map_data[pixel_idx] = color & 0xFF;
	cube->mlx->mini_map_data[pixel_idx + 1] = (color >> 8) & 0xFF;
	cube->mlx->mini_map_data[pixel_idx + 2] = (color >> 16) & 0xFF;
}

void	draw_main_pixel(t_cube *cube, int x, int y, int color)
{
	int	pixel_idx;

	if (x < 0 || y < 0 || x > WINDOW_H || y > WINDOW_W)
		return ;
	pixel_idx = x * cube->mlx->main_size_line + y * cube->mlx->main_p_bits / 8;
	cube->mlx->main_data[pixel_idx] = color & 0xFF;
	cube->mlx->main_data[pixel_idx + 1] = (color >> 8) & 0xFF;
	cube->mlx->main_data[pixel_idx + 2] = (color >> 16) & 0xFF;
}

void	draw_player(t_cube *cube)
{
	int		x;
	int		y;

	x = -1;
	while (++x < MINIMAP_SCALE)
	{
		y = -1;
		while (++y < MINIMAP_SCALE)
			if (pow(cube->radius * MINIMAP_SCALE - x, 2) + pow(cube->radius * MINIMAP_SCALE - y, 2) <= pow(cube->radius * MINIMAP_SCALE, 2))
				draw_map_pixel(cube,
					(cube->player_x - cube->radius) * MINIMAP_SCALE + x,
					(cube->player_y - cube->radius) * MINIMAP_SCALE + y, GREEN);
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
	while (++i < CLOSE_DOOR)
	{
		draw_map_pixel(cube, xi * MINIMAP_SCALE, yi * MINIMAP_SCALE, 0xffffff);
		xi += (rayx - cube->player_x) / CLOSE_DOOR; //value to be changed based on map dimensions. Defines how many pixels to color on a line
		yi += (rayy - cube->player_y) / CLOSE_DOOR;
	}
}

void	draw_square(t_cube *cube, int x_scaled, int y_scaled, int color)
{
	int		x;
	int		y;
	// float	ratio;
	// ratio = (float)cube->textures->wall_h[color] / MINIMAP_SCALE; if we want to use textures for minimap
	// draw_map_pixel(cube, x + x_scaled, y + y_scaled, cube->textures->wall_data[color][(int)((y * ratio) * cube->textures->wall_h[color] + (x * ratio))]);
	x = -1;
	while (++x < MINIMAP_SCALE)
	{
		y = -1;
		while (++y < MINIMAP_SCALE)
			draw_map_pixel(cube, x + x_scaled, y + y_scaled, color);
	}
}

void	draw_window_map(t_cube *cube)
{
	int x;
	int y;
	int indexx = ((cube->player_x - 4) * MINIMAP_SCALE);
	int indexy = ((cube->player_y - 4) * MINIMAP_SCALE);
	int color;

	x = -1;
	while (++x < 10*MINIMAP_SCALE)
	{
		y = -1;
		while (++y < 10*MINIMAP_SCALE)
		{
			color = 0x4a4a4a;
			if ((x + indexx < cube->map_h * MINIMAP_SCALE && y + indexy < cube->map_w * MINIMAP_SCALE) && (x + indexx > 0 && y + indexy > 0))
				color = *(int *)(cube->mlx->map_data + (x + indexx) * cube->mlx->map_size_line + (y + indexy) * (cube->mlx->map_p_bits / 8));
			draw_mini_map_pixel(cube, x, y, color);
		}
	}
}

void draw_minimap(t_cube *cube)
{
	int x;
	int y;

	x = -1;
	while (++x < cube->map_h)
	{
		y = -1;
		while (++y < cube->map_w)
		{
			if (cube->map[x][y] == '!')
				draw_square(cube, x * MINIMAP_SCALE, y * MINIMAP_SCALE, 0x7b7d79);
			else if (cube->map[x][y] == '#')
				draw_square(cube, x * MINIMAP_SCALE, y * MINIMAP_SCALE, 0x110b47);
			else if (cube->map[x][y] >= CLOSE_DOOR && cube->map[x][y] < OPEN_DOOR)
				draw_square(cube, x * MINIMAP_SCALE, y * MINIMAP_SCALE, 0x520a00);
			else if(cube->map[x][y] == OPEN_DOOR)
				draw_square(cube, x * MINIMAP_SCALE, y * MINIMAP_SCALE, 0xdeeb34);
		}
	}
	draw_player(cube);
	draw_window_map(cube);
}


void	draw_walls(t_cube *cube)
{
	int			y;
	int			x;
	int			txt;
	float		wall_idx;
	float		wall_size;
	float		wall_offset;

	y = -1;
	while (++y < WINDOW_W)
	{
		x = -1;
		wall_idx = 0;
		wall_offset = 0;
		if (cube->ray->hit_door[y])
			txt = 4;
		else
			txt = cube->ray->hit_direction[y];
		wall_size = WINDOW_W / (tan(FOV / 2)) / cube->ray->hit_dist[y] / 2;
		if (wall_size > WINDOW_H)
			wall_offset = (wall_size - WINDOW_H) / 2 * cube->textures->wall_h[txt] / wall_size;
		while (++x < WINDOW_H)
		{
			if (x > WINDOW_H / 2. - (wall_size / 2) && x < WINDOW_H / 2. + (wall_size / 2))
			{
				if (txt == 4)
				{
					draw_main_pixel(cube, x, y, cube->textures->wall_data[txt][(int)(wall_idx + wall_offset) * cube->textures->wall_w[txt]
						+ (int)(cube->ray->hit_coordn[y] * cube->textures->wall_h[txt] + ((1 - cube->ray->hit_door[y] / 100.) * cube->textures->wall_w[txt]))]);
							wall_idx += cube->textures->wall_h[txt] / wall_size;
				}
				else
				{
					draw_main_pixel(cube, x, y, cube->textures->wall_data[txt][(int)(wall_idx + wall_offset) * cube->textures->wall_w[txt]
						+ (int)(cube->ray->hit_coordn[y] * cube->textures->wall_h[txt])]);
							wall_idx += cube->textures->wall_h[txt] / wall_size;
				}
			}
		}
	}
}
