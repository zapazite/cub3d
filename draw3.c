/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:20:54 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/23 15:02:33 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_window_map(t_cube *cube)
{
	int	x;
	int	y;
	int	indexx;
	int	indexy;
	int	color;

	indexx = ((cube->player_x - 4) * WINDOW_W / 80);
	indexy = ((cube->player_y - 4) * WINDOW_W / 80);
	x = -1;
	while (++x < 10 * WINDOW_W / 80)
	{
		y = -1;
		while (++y < 10 * WINDOW_W / 80)
		{
			color = 0x4a4a4a;
			if ((x + indexx < cube->map_h * WINDOW_W / 80 && y
					+ indexy < cube->map_w * WINDOW_W / 80) && (x + indexx > 0
					&& y + indexy > 0))
				color = *(int *)(cube->mlx->map_data + (x + indexx)
						* cube->mlx->map_size_line + (y + indexy)
						* (cube->mlx->map_p_bits / 8));
			draw_mini_map_pixel(cube, x, y, color);
		}
	}
}

void	draw_mini_map(t_cube *cube, int x, int y)
{
	if (cube->map[x][y] >= CLOSE_DOOR
		&& cube->map[x][y] < OPEN_DOOR)
		draw_square(cube, x * WINDOW_W / 80, y * WINDOW_W / 80,
			0x520a00);
	else if (cube->map[x][y] == OPEN_DOOR)
		draw_square(cube, x * WINDOW_W / 80, y * WINDOW_W / 80,
			0xdeeb34);
}

void	draw_minimap(t_cube *cube)
{
	int	x;
	int	y;

	if (!BONUS)
		return ;
	x = -1;
	while (++x < cube->map_h)
	{
		y = -1;
		while (++y < cube->map_w)
		{
			if (cube->map[x][y] == '!')
				draw_square(cube, x * WINDOW_W / 80, y * WINDOW_W / 80,
					0x7b7d79);
			else if (cube->map[x][y] == '#')
				draw_square(cube, x * WINDOW_W / 80, y * WINDOW_W / 80,
					0x110b47);
			draw_mini_map(cube, x, y);
		}
	}
	draw_player(cube);
	draw_window_map(cube);
}

void	draw_square(t_cube *cube, int x_scaled, int y_scaled, int color)
{
	int	x;
	int	y;

	x = -1;
	while (++x <= WINDOW_W / 80)
	{
		y = -1;
		while (++y <= WINDOW_W / 80)
			draw_map_pixel(cube, x + x_scaled, y + y_scaled, color);
	}
}
