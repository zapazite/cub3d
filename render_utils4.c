/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:23:36 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/23 15:24:00 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	mouse_click(int button, int x, int y, t_cube *cube)
{
	(void)x;
	(void)y;
	if (button == 1)
		cube->keys->mouse_left = 1;
	return (1);
}

void	draw_gun(t_cube *cube, int index)
{
	int				x;
	int				y;
	unsigned int	color;

	x = -1;
	while (++x < cube->anim->h[index])
	{
		y = -1;
		while (++y < cube->anim->w[index])
		{
			color = cube->anim->data[index][x * cube->anim->w[index] + y];
			if (color != 0xff000000)
				draw_main_pixel(cube, x + (WINDOW_H - cube->anim->h[index]), y
					+ (WINDOW_W - cube->anim->w[index]) / 2, color);
		}
	}
}

void	animation(t_cube *cube)
{
	if (cube->keys->mouse_left)
		draw_gun(cube, cube->anim->counter++);
	else
		draw_gun(cube, 19);
	if (cube->anim->counter == ANIM_FRAMES - 1)
	{
		cube->keys->mouse_left = 0;
		cube->anim->counter = 0;
	}
}
