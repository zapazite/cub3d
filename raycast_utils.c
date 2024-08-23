/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:26:05 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/23 13:26:36 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	cast_h(int rayx, float rayy, t_cube *cube)
{
	while (rayx < cube->map_h && (int)rayy < cube->map_w && rayx > 0 && rayy > 0
		&& cube->map[rayx - (cube->ray->dx < 0)][(int)rayy] != '#'
		&& !door_check(cube, rayx - (cube->ray->dx < 0), rayy, 'h'))
	{
		if (cube->ray->dx > 0)
		{
			rayx++;
			rayy += cube->ray->slope;
		}
		else
		{
			rayx--;
			rayy -= cube->ray->slope;
		}
	}
	cube->ray->hit_door_h = cube->ray->door_check_flag;
	cube->ray->door_check_flag = 0;
	cube->ray->h_x = rayx;
	cube->ray->h_y = rayy;
}

void	cast_w(float rayx, int rayy, t_cube *cube)
{
	while ((int)rayx < cube->map_h && rayy < cube->map_w && rayx > 0 && rayy > 0
		&& cube->map[(int)rayx][rayy - (cube->ray->dy < 0)] != '#'
		&& !door_check(cube, rayx, rayy - (cube->ray->dy < 0), 'w'))
	{
		if (cube->ray->dy > 0)
		{
			rayy++;
			rayx += cube->ray->slope;
		}
		else
		{
			rayy--;
			rayx -= cube->ray->slope;
		}
	}
	cube->ray->hit_door_w = cube->ray->door_check_flag;
	cube->ray->door_check_flag = 0;
	cube->ray->w_x = rayx;
	cube->ray->w_y = rayy;
}

void	find_start_h(t_cube *cube)
{
	int		rayx;
	float	rayy;

	rayx = 0;
	rayy = 0;
	cube->ray->slope = cube->ray->dy / cube->ray->dx;
	if (cube->ray->dx == 0)
		return ;
	if (cube->ray->dx > 0)
		rayx = (int)(cube->player_x + 1);
	else
		rayx = (int)cube->player_x;
	rayy = (rayx - cube->player_x) * cube->ray->slope + cube->player_y;
	cast_h(rayx, rayy, cube);
}

void	find_start_w(t_cube *cube)
{
	int		rayy;
	float	rayx;

	rayx = 0;
	rayy = 0;
	cube->ray->slope = cube->ray->dx / cube->ray->dy;
	if (cube->ray->dy == 0)
		return ;
	if (cube->ray->dy > 0)
		rayy = (int)(cube->player_y + 1);
	else
		rayy = (int)cube->player_y;
	rayx = (rayy - cube->player_y) * cube->ray->slope + cube->player_x;
	cast_w(rayx, rayy, cube);
}
