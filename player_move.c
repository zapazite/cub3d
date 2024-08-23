/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:25:05 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/23 15:06:01 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	door_state(int x, int y, t_cube *cube)
{
	int	door_value;

	door_value = cube->map[x][y];
	if (door_value >= CLOSE_DOOR && door_value < OPEN_DOOR)
		return (1);
	return (0);
}

int	check_player_position(float player_x, float player_y, t_cube *cube)
{
	if (cube->map[(int)((player_x + cube->radius))][(int)((player_y
				+ cube->radius))] == '#' || (door_state((int)((player_x
						+ cube->radius)), (int)((player_y + cube->radius)),
				cube)))
		return (0);
	if (cube->map[(int)((player_x + cube->radius))][(int)((player_y
				- cube->radius))] == '#' || (door_state((int)((player_x
						+ cube->radius)), (int)((player_y - cube->radius)),
				cube)))
		return (0);
	if (cube->map[(int)((player_x - cube->radius))][(int)((player_y
				+ cube->radius))] == '#' || (door_state((int)((player_x
						- cube->radius)), (int)((player_y + cube->radius)),
				cube)))
		return (0);
	if (cube->map[(int)((player_x - cube->radius))][(int)((player_y
				- cube->radius))] == '#' || (door_state((int)((player_x
						- cube->radius)), (int)((player_y - cube->radius)),
				cube)))
		return (0);
	return (1);
}

void	key_check(t_cube *cube, float *move_x, float *move_y)
{
	if (cube->keys->key_up)
	{
		*move_x += cube->player_dx * PLAYER_SPEED;
		*move_y += cube->player_dy * PLAYER_SPEED;
	}
	else if (cube->keys->key_down)
	{
		*move_x -= cube->player_dx * PLAYER_SPEED;
		*move_y -= cube->player_dy * PLAYER_SPEED;
	}
	if (cube->keys->key_right)
	{
		*move_x += cube->player_dy * PLAYER_SPEED;
		*move_y -= cube->player_dx * PLAYER_SPEED;
	}
	else if (cube->keys->key_left)
	{
		*move_x -= cube->player_dy * PLAYER_SPEED;
		*move_y += cube->player_dx * PLAYER_SPEED;
	}
}

void	move_player(t_cube *cube)
{
	float	move_x;
	float	move_y;

	move_x = cube->player_x;
	move_y = cube->player_y;
	key_check(cube, &move_x, &move_y);
	if (check_player_position(move_x, cube->player_y, cube))
		cube->player_x = move_x;
	if (check_player_position(cube->player_x, move_y, cube))
		cube->player_y = move_y;
}
