/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:21:38 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/23 15:22:19 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	key_handler(int keycode, t_cube *cube)
{
	if (keycode == XK_Escape)
		close_window(cube);
	if (keycode == XK_Up || keycode == XK_w)
		cube->keys->key_up = 1;
	if (keycode == XK_Down || keycode == XK_s)
		cube->keys->key_down = 1;
	if (keycode == XK_Left || keycode == XK_a)
		cube->keys->key_left = 1;
	if (keycode == XK_Right || keycode == XK_d)
		cube->keys->key_right = 1;
	if (keycode == XK_o)
		open_door(cube);
	if (keycode == XK_c)
		close_door(cube);
	return (0);
}

int	is_door(t_cube *cube)
{
	if (cube->map[(int)(cube->player_x + cube->radius
			* cube->player_dx)][(int)(cube->player_y + cube->radius
			* cube->player_dy)] >= 1001)
		return (1);
	return (0);
}

void	open_door(t_cube *cube)
{
	if (cube->map[(int)(cube->player_x + cube->player_dx)][(int)(cube->player_y
			+ cube->player_dy)] == CLOSE_DOOR)
		cube->map[(int)(cube->player_x + cube->player_dx)][(int)(cube->player_y
				+ cube->player_dy)] += DOOR_SPEED;
}

void	close_door(t_cube *cube)
{
	if (is_door(cube))
		return ;
	if (cube->map[(int)(cube->player_x + cube->player_dx)][(int)(cube->player_y
			+ cube->player_dy)] == OPEN_DOOR)
		cube->map[(int)(cube->player_x + cube->player_dx)][(int)(cube->player_y
				+ cube->player_dy)] -= DOOR_SPEED;
}

int	key_release(int keycode, t_cube *cube)
{
	if (keycode == XK_Up || keycode == XK_w)
		cube->keys->key_up = 0;
	if (keycode == XK_Down || keycode == XK_s)
		cube->keys->key_down = 0;
	if (keycode == XK_Left || keycode == XK_a)
		cube->keys->key_left = 0;
	if (keycode == XK_Right || keycode == XK_d)
		cube->keys->key_right = 0;
	return (0);
}
