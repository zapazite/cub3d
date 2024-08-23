/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 08:55:06 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/23 14:55:21 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_walls(t_cube *cube, int x, int y)
{
	if (cube->draw->txt == 4)
	{
		draw_main_pixel(cube, x, y,
			cube->txt->data[4][(int)(cube->draw->wall_idx
				+ cube->draw->wall_offset) * cube->txt->w[4]
			+ (int)(cube->ray->hit_coordn[y] * cube->txt->h[4] + ((1
						- cube->ray->hit_door[y] / 100.) * cube->txt->w[4]))]);
		cube->draw->wall_idx += cube->txt->h[4] / cube->draw->wall_size;
	}
	else
	{
		draw_main_pixel(cube, x, y,
			cube->txt->data[cube->draw->txt][(int)(cube->draw->wall_idx
				+ cube->draw->wall_offset) * cube->txt->w[cube->draw->txt]
			+ (int)(cube->ray->hit_coordn[y] * cube->txt->h[cube->draw->txt])]);
		cube->draw->wall_idx += cube->txt->h[cube->draw->txt]
			/ cube->draw->wall_size;
	}
}

void	draw_world(t_cube *cube)
{
	int	y;
	int	x;

	y = -1;
	while (++y < WINDOW_W)
	{
		x = -1;
		cube->draw->wall_idx = 0;
		cube->draw->wall_offset = 0;
		if (cube->ray->hit_door[y])
			cube->draw->txt = 4;
		else
			cube->draw->txt = cube->ray->hit_direction[y];
		cube->draw->wall_size = WINDOW_W / (tan(FOV / 2))
			/ cube->ray->hit_dist[y] / 2;
		if (cube->draw->wall_size > WINDOW_H)
			cube->draw->wall_offset = (cube->draw->wall_size - WINDOW_H) / 2
				* cube->txt->h[cube->draw->txt] / cube->draw->wall_size;
		while (++x < WINDOW_H)
		{
			if (x > WINDOW_H / 2. - (cube->draw->wall_size / 2) && x < WINDOW_H
				/ 2. + (cube->draw->wall_size / 2))
				draw_walls(cube, x, y);
		}
	}
}

void	draw_basic_walls(t_cube *cube, int x, int y)
{
	if (x < WINDOW_H / 2. - (cube->draw->wall_size / 2))
		draw_main_pixel(cube, x, y, cube->colors[CIELLING]);
	else if (x > WINDOW_H / 2. + (cube->draw->wall_size / 2))
		draw_main_pixel(cube, x, y, cube->colors[FLOOR]);
	else
	{
		draw_main_pixel(cube, x, y,
			cube->txt->data[1][(int)(cube->draw->wall_idx
				+ cube->draw->wall_offset) * cube->txt->w[1]
			+ (int)(cube->ray->hit_coordn[y] * cube->txt->h[1])]);
		cube->draw->wall_idx += cube->txt->h[1] / cube->draw->wall_size;
	}
}

void	draw_basic_world(t_cube *cube)
{
	int	y;
	int	x;

	y = -1;
	while (++y < WINDOW_W)
	{
		x = -1;
		cube->draw->wall_idx = 0;
		cube->draw->wall_offset = 0;
		cube->draw->wall_size = WINDOW_W / (tan(FOV / 2))
			/ cube->ray->hit_dist[y] / 2;
		if (cube->draw->wall_size > WINDOW_H)
			cube->draw->wall_offset = (cube->draw->wall_size - WINDOW_H) / 2
				* cube->txt->h[1] / cube->draw->wall_size;
		while (++x < WINDOW_H)
			draw_basic_walls(cube, x, y);
	}
}
