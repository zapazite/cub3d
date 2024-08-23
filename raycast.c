/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:25:41 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/23 13:26:43 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	save_ray_info(t_cube *cube, int i, float d_player_screen)
{
	float	h_hit;
	float	w_hit;

	h_hit = pow(cube->player_x - cube->ray->h_x, 2) + pow(cube->player_y
			- cube->ray->h_y, 2);
	w_hit = pow(cube->player_x - cube->ray->w_x, 2) + pow(cube->player_y
			- cube->ray->w_y, 2);
	if (h_hit <= w_hit)
	{
		cube->ray->hit_door[i] = cube->ray->hit_door_h;
		cube->ray->hit_direction[i] = (cube->ray->dx > 0);
		cube->ray->hit_coordn[i] = cube->ray->h_y - (int)cube->ray->h_y;
		cube->ray->hit_dist[i] = sqrt(h_hit) * d_player_screen
			/ (sqrt(pow(cube->ray->dx, 2) + pow(cube->ray->dy, 2)));
	}
	else
	{
		cube->ray->hit_door[i] = cube->ray->hit_door_w;
		cube->ray->hit_direction[i] = (cube->ray->dy > 0) + 2;
		cube->ray->hit_coordn[i] = cube->ray->w_x - (int)cube->ray->w_x;
		cube->ray->hit_dist[i] = sqrt(w_hit) * d_player_screen
			/ (sqrt(pow(cube->ray->dx, 2) + pow(cube->ray->dy, 2)));
	}
}

void	ray_cast(t_cube *cube)
{
	int		i;
	float	d_player_screen;

	i = 0;
	cube->ray->dx = (WINDOW_W / 2.) / tan(FOV / 2) * cube->player_dx;
	cube->ray->dy = (WINDOW_W / 2.) / tan(FOV / 2) * cube->player_dy;
	d_player_screen = sqrt(pow(cube->ray->dx, 2) + pow(cube->ray->dy, 2));
	cube->ray->dx += WINDOW_W / 2. * -cube->player_dy;
	cube->ray->dy += WINDOW_W / 2. * cube->player_dx;
	while (i < WINDOW_W)
	{
		find_start_h(cube);
		find_start_w(cube);
		save_ray_info(cube, i, d_player_screen);
		cube->ray->hit_door_h = 0;
		cube->ray->hit_door_w = 0;
		cube->ray->dx -= -cube->player_dy;
		cube->ray->dy -= cube->player_dx;
		i++;
	}
}
