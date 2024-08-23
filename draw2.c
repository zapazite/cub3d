/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:19:43 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/23 13:20:06 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_map_pixel(t_cube *cube, int x, int y, int color)
{
	int	pixel_idx;

	if (x < 0 || y < 0 || x > cube->map_h * WINDOW_W / 80 || y > cube->map_w
		* WINDOW_W / 80)
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

	if (x < 0 || y < 0 || x > 10 * WINDOW_W / 80 || y > 10 * WINDOW_W / 80)
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
	int	x;
	int	y;

	x = -1;
	while (++x < WINDOW_W / 80)
	{
		y = -1;
		while (++y < WINDOW_W / 80)
			if (pow(cube->radius * WINDOW_W / 80 - x, 2) + pow(cube->radius
					* WINDOW_W / 80 - y, 2) <= pow(cube->radius * WINDOW_W / 80,
					2))
				draw_map_pixel(cube, (cube->player_x - cube->radius)
					* WINDOW_W / 80 + x, (cube->player_y - cube->radius)
					* WINDOW_W / 80 + y, GREEN);
	}
}

void	draw_line(float rayx, float rayy, t_cube *cube)
{
	float	xi;
	float	yi;
	int		i;

	xi = cube->player_x;
	yi = cube->player_y;
	i = -1;
	while (++i < CLOSE_DOOR)
	{
		draw_map_pixel(cube, xi * WINDOW_W / 80, yi * WINDOW_W / 80, 0xffffff);
		xi += (rayx - cube->player_x) / CLOSE_DOOR;
		yi += (rayy - cube->player_y) / CLOSE_DOOR;
	}
}
